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

#define _EXPAND_COL1 4
#define _EXPAND_COL2 5

#ifndef MATRIX_INPUT_PRESSED_STATE
#    define MATRIX_INPUT_PRESSED_STATE 0
#endif

static uint8_t mcp23018_errors = 0;

static void expander_init(void) {
    mcp23018_init(I2C_ADDR);
}

static void expander_init_cols(void) {
    mcp23018_errors += !mcp23018_set_config(I2C_ADDR, mcp23018_PORTA, ALL_INPUT);
    mcp23018_errors += !mcp23018_set_config(I2C_ADDR, mcp23018_PORTB, ALL_INPUT);
}

static inline void setPinOutput_writeLow(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinLow(pin);
    }
}

static inline void setPinOutput_writeHigh(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinHigh(pin);
    }
}

static inline void setPinInputHigh_atomic(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinInputHigh(pin);
    }
}

static inline uint8_t readMatrixPin(pin_t pin) {
    if (pin != NO_PIN) {
        return (readPin(pin) == MATRIX_INPUT_PRESSED_STATE) ? 0 : 1;
    } else {
        return 1;
    }
}

static bool select_col(uint8_t col)
{
    if (col_pins[col] != NO_PIN) {
        setPinOutput_writeLow(col_pins[col]);
        return true;
    }
    return false;
}

static bool unselect_col(uint8_t col)
{
    if (col_pins[col] != NO_PIN) {
        setPinInputHigh_atomic(col_pins[col]);
        return true;
    }
    return false;
}

static void unselect_cols(void)
{
    for(uint8_t x = 0; x < MATRIX_COLS; x++) {
        if (col_pins[x] != NO_PIN) {
            setPinInputHigh_atomic(col_pins[x]);
        }
    }
}

static matrix_row_t expander_read_row(void) {
    if (mcp23018_errors) {
        return 0;
    }

    uint8_t ret = 0xFF;
    mcp23018_errors += !mcp23018_readPins(I2C_ADDR, mcp23018_PORTA, &ret);

    return ret;
}

static void read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter, matrix_row_t expandA)
{
    bool key_pressed = false;
    // Select col and wait for col selecton to stabilize
    if (!select_col(current_col)) { // Select row
        return;                     // skip NO_PIN row
    }
    matrix_io_delay();

    switch(current_col) {
        case _EXPAND_COL1:
            
            break;
        case _EXPAND_COL2:
            break;
        default:
            // For each row...
            for(uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
                // Check row pin state
                if (readMatrixPin(row_pins[row_index]) == 0)
                {
                    // Pin LO, set col bit
                    current_matrix[row_index] |= row_shifter;
                    key_pressed = true;
            }
            else {
                // Pin HI, clear col bit
                current_matrix[row_index] &= ~row_shifter;
            }
        }
    }

    // Unselect col
    unselect_col(current_col);
    matrix_output_unselect_delay(current_col, key_pressed);
}

void matrix_init_custom(void) {
    unselect_cols();
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        if (row_pins[x] != NO_PIN) {
            setPinInputHigh_atomic(row_pins[x]);
        }
    }
    expander_init_cols();
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    static matrix_row_t temp_matrix[MATRIX_ROWS] = {0};
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    // Set col, read rows
    matrix_row_t expandA = expander_read_row();
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++, row_shifter <<= 1) {
        read_rows_on_col(temp_matrix, current_col, row_shifter, expandA);
    }
    // set_paj7620_to_matrix(temp_matrix);
    bool changed = memcmp(current_matrix, temp_matrix, sizeof(temp_matrix)) != 0;
    if (changed) {
        memcpy(current_matrix, temp_matrix, sizeof(temp_matrix));
    }
    return changed;
}
