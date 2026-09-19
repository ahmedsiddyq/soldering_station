#include "display.h"
#include "i2c1.h"

// --- Pseudo-I2C register addresses (7-bit), see display.h for the derivation ---
#define FD650B_ADDR_DISPLAY_CTRL   0x24u
#define FD650B_ADDR_KEY_READ       0x27u

// DIG1..DIG4 map to consecutive addresses 0x34..0x37
static const uint8_t display_digit_addr[FD650B_NUM_DIGITS] = { 0x34u, 0x35u, 0x36u, 0x37u };

const uint8_t display_font[16] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71, // F
};

#define FD650B_DP_BIT   0x80u

// Cached control-register state so on/off and brightness can be
// changed independently without clobbering each other.
static uint8_t s_ctrl_reg = 0;

static bool display_push_ctrl(void)
{
    return I2C1_WriteByte(FD650B_ADDR_DISPLAY_CTRL, s_ctrl_reg);
}

void display_init(void)
{
    // bit0=1 (on), bit3=0 (8-segment/DP mode), bits6:4 = 4 (mid brightness)
    s_ctrl_reg = (uint8_t)((4u << 4) | (0u << 3) | 1u);
    display_push_ctrl();
    display_clear_all();
}

bool display_display_on(bool on)
{
    if (on) {
        s_ctrl_reg |= 0x01u;
    } else {
        s_ctrl_reg &= (uint8_t)~0x01u;
    }
    return display_push_ctrl();
}

bool display_set_brightness(uint8_t brightness)
{
    if (brightness > 7u) brightness = 7u;
    s_ctrl_reg = (uint8_t)((s_ctrl_reg & 0x8Fu) | (brightness << 4));
    return display_push_ctrl();
}

bool display_write_digit_raw(uint8_t position, uint8_t segments)
{
    if (position >= FD650B_NUM_DIGITS) return false;
    return I2C1_WriteByte(display_digit_addr[position], segments);
}

bool display_write_digit_hex(uint8_t position, uint8_t value, bool dot)
{
    if (value > 15u) return false;
    uint8_t segs = display_font[value];
    if (dot) segs |= FD650B_DP_BIT;
    return display_write_digit_raw(position, segs);
}

bool display_clear_digit(uint8_t position)
{
    return display_write_digit_raw(position, 0x00u);
}

bool display_clear_all(void)
{
    bool ok = true;
    for (uint8_t i = 0; i < FD650B_NUM_DIGITS; i++) {
        ok = display_clear_digit(i) && ok;
    }
    return ok;
}

bool display_write_number(uint16_t value)
{
    if (value > 9999u) return false;

    uint8_t digits[FD650B_NUM_DIGITS];
    for (int8_t i = FD650B_NUM_DIGITS - 1; i >= 0; i--) {
        digits[i] = (uint8_t)(value % 10u);
        value /= 10u;
    }

    bool ok = true;
    bool leading = true;
    for (uint8_t i = 0; i < FD650B_NUM_DIGITS; i++) {
        bool isLast = (i == FD650B_NUM_DIGITS - 1);
        if (leading && digits[i] == 0 && !isLast) {
            ok = display_clear_digit(i) && ok;      // blank leading zero
        } else {
            leading = false;
            ok = display_write_digit_hex(i, digits[i], false) && ok;
        }
    }
    return ok;
}

bool display_read_keys(uint8_t *key_code, bool *pressed)
{
    uint8_t raw;
    if (!I2C1_Read(FD650B_ADDR_KEY_READ, &raw, 1)) {
        return false;
    }
    if (key_code) *key_code = (uint8_t)(raw & 0x3Fu);
    if (pressed)  *pressed  = ((raw & 0x40u) != 0u);
    return true;
}
