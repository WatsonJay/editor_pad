BOARD = GENERIC_RP_RP2040

QMK_SETTINGS = yes
KEY_OVERRIDE_ENABLE = yes
TAP_DANCE_ENABLE = yes
MAGIC_ENABLE = yes
SPACE_CADET_ENABLE = yes
WPM_ENABLE = yes
GRAVE_ESC_ENABLE = yes
DYNAMIC_MACRO_ENABLE = yes

DEBOUNCE_TYPE = sym_eager_pr

I2C_DRIVER_REQUIRED = yes

QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS += st7789_spi
QUANTUM_PAINTER_LVGL_INTEGRATION = yes

VPATH += drivers/gpio
SRC += mcp23018.c \
	   matrix.c \
	   ./utils/keycode_string.c \

