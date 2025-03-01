/*
Copyright 2012-2018 JayWatson

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "matrix.h"
#include "quantum.h"
#include "mcp23018.h"
#include "print.h"

#define MCP_ADDR 0x20

#define _EXPAND_ROW1 4
#define _EXPAND_ROW2 5

#if (MATRIX_COLS <= 8)
    #define print_matrix_header()  print("\nr/c 01234567\n")
    #define print_matrix_row(row)  print_bin_reverse8(matrix_get_row(row))
    #define ROW_SHIFTER ((uint8_t)1)
#elif (MATRIX_COLS <= 16)
    #define print_matrix_header()  print("\nr/c 0123456789ABCDEF\n")
    #define print_matrix_row(row)  print_bin_reverse16(matrix_get_row(row))
    #define ROW_SHIFTER ((uint16_t)1)
#elif (MATRIX_COLS <= 32)
    #define print_matrix_header()  print("\nr/c 0123456789ABCDEF0123456789ABCDEF\n")
    #define print_matrix_row(row)  print_bin_reverse32(matrix_get_row(row))
    #define ROW_SHIFTER  ((uint32_t)1)
#endif

void matrix_print1(void)
{
    print_matrix_header();

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        print_hex8(row); print(": ");
        print_matrix_row(row);
        print("\n");
    }
}

// 键盘默认为 row2col
static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

static uint8_t mcp23018_errors = 0;
static uint16_t mcp23018_reset_loop = 0;
static matrix_row_t last_tarck = 0xFF;

static void expander_init_cols(void) {
    mcp23018_errors += !mcp23018_set_config(MCP_ADDR, mcp23018_PORTA, ALL_INPUT);
    if (mcp23018_errors == 0) {
        printf("success");
    } else {
        printf("fail");
    }
}

static matrix_row_t expander_read_porta(void) {
    if (mcp23018_errors) {
        // wait to mimic i2c interactions
        if (++mcp23018_reset_loop > 0x1FFF) {
            dprintf("trying to reset mcp23018\n");
            mcp23018_reset_loop = 0;
            mcp23018_errors     = 0;
            expander_init_cols();
        }
        wait_us(100);
        return 0;
    }

    uint8_t ret = 0xFF;
    mcp23018_errors += !mcp23018_readPins(MCP_ADDR, mcp23018_PORTA, &ret);
    return ret;
}

static bool select_col(uint8_t col)
{
    if (col_pins[col] != NO_PIN) {
        gpio_set_pin_output(col_pins[col]);
        gpio_write_pin_low(col_pins[col]);
        return true;
    }
    return false;
}

static bool unselect_col(uint8_t col)
{
    if (col_pins[col] != NO_PIN) {
        gpio_set_pin_input_high(col_pins[col]);
        return true;
    }
    return false;
}

static void unselect_cols(void)
{
    for(uint8_t col = 0; col < MATRIX_COLS; col++) {
        if (col_pins[col] != NO_PIN) {
            gpio_set_pin_input_high(col_pins[col]);
        }
    }
}

static void read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col)
{
    bool key_pressed = false;
    // Select col and wait for col selecton to stabilize
    if (!select_col(current_col)) { // Select col
        return;                     // skip NO_PIN row
    }
    matrix_io_delay();

    // For each row...
    for(uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
        if (row_index != _EXPAND_ROW1 && row_index != _EXPAND_ROW2 ) {
            // Check row pin state
            if (gpio_read_pin(row_pins[row_index]) == 0)
            {
                // Pin LO, set col bit
                current_matrix[row_index] |= (MATRIX_ROW_SHIFTER << current_col);;
                key_pressed = true;
            }
            else {
                // Pin HI, clear col bit
                current_matrix[row_index] &= ~(MATRIX_ROW_SHIFTER << current_col);;
            }
        }

    }

    // Unselect col
    unselect_col(current_col);
    matrix_output_unselect_delay(current_col, key_pressed);
}

/* mcp23017引脚映射到矩阵
 *    0   1   2   3
 * 4 GP0 GP1 GP2 GP3
 * 5 GP4 GP5 GP6 GP7
 */
static void read_mcp_to_row(matrix_row_t current_matrix[]) {
    matrix_row_t expand = expander_read_porta();
    if (last_tarck == 0xFF) {
        last_tarck = expand & 0b00001111;
    }
    current_matrix[_EXPAND_ROW1] = (expand & 0b00001111) ^ last_tarck;
    last_tarck = expand & 0b00001111;
    current_matrix[_EXPAND_ROW2] = (~ (expand & 0b11110000)) >> 4 & 0b00001111;
}

void matrix_init_custom(void) {
    mcp23018_init(MCP_ADDR);
    unselect_cols();
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        if (row_pins[x] != NO_PIN) {
            gpio_set_pin_input_high(row_pins[x]);
        }
    }
    expander_init_cols();
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    static matrix_row_t temp_matrix[MATRIX_ROWS] = {0};

    // Set col, read rows
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++) {
        read_rows_on_col(temp_matrix, current_col);
    }
    // read mcp23017
    read_mcp_to_row(temp_matrix);

    bool changed = memcmp(current_matrix, temp_matrix, sizeof(temp_matrix)) != 0;
    if (changed) {
        memcpy(current_matrix, temp_matrix, sizeof(temp_matrix));
        matrix_print1();
    }
    return changed;
}
