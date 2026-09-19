/*
 * display.h - Driver for the FD650B 4-digit 7-segment LED + keyboard
 *            scan controller, over its pseudo-I2C interface.
 *
 * The FD650B is a hardware/software clone of the Titan Micro TM1650
 * (confirmed: it "resembles very strongly the popular TM1650" per
 * multiple independent teardown/driver write-ups). It is NOT a
 * compliant I2C slave: instead of one device address + internal
 * register pointer, it exposes each internal register as its OWN
 * fixed 7-bit I2C address, matching the TM1650 memory map:
 *
 *   7-bit addr   8-bit (addr+R/W) byte   Function
 *   ----------   ---------------------   --------------------------
 *   0x24 (W)     0x48                    Display control register
 *   0x34 (W)     0x68                    Digit 1 (leftmost) data
 *   0x35 (W)     0x6A                    Digit 2 data
 *   0x36 (W)     0x6C                    Digit 3 data
 *   0x37 (W)     0x6E                    Digit 4 (rightmost) data
 *   0x27 (R)     0x4F                    Key-scan data (read-only)
 *
 * Cross-checked against: the TM1650 datasheet's "68H/6AH/6CH/6EH"
 * display-memory address table (Titan Micro Electronics TM1650 V1.0),
 * a working STM32 HAL example driving an FD650B/TM1650-class part at
 * I2C addresses 0x24/0x34/0x35/0x36, and independent forum reports of
 * an I2C bus scan on an FD650B ACKing at 0x48/0x68/0x6A (which are the
 * 8-bit forms of 0x24 and 0x34/0x35).
 *
 * Because each register is its own bus address, you cannot put a
 * second FD650B on the same bus -- both parts would answer to the
 * same six addresses and collide.
 *
 * Display control byte (sent to 0x24), bit layout per the TM1650
 * "Display command settings" table:
 *   bit 0     : Display ON (1) / OFF (0)
 *   bit 3     : Segment mode -- 0 = 8-segment (digit + DP), 1 = 7-segment only
 *   bits 6:4  : Brightness, 3-bit field, 8 steps
 *   bit 7     : unused / write 0
 * NOTE: the brightness step ordering (whether 0b000 is the brightest
 * or dimmest end of the scale) is inconsistently translated across
 * available copies of this datasheet. This driver treats 0 = dimmest,
 * 7 = brightest, which is the convention used by most open FD650B/
 * TM1650 libraries -- if your unit's brightest/dimmest ends are
 * swapped, invert the value you pass to display_set_brightness().
 *
 * Digit data byte bit layout (bit -> segment), per the TM1650 memory
 * map table (A/KI1..G/KI7, DP/KP -> B0..B7):
 *   bit0=a  bit1=b  bit2=c  bit3=d  bit4=e  bit5=f  bit6=g  bit7=dp
 * This is the standard common-cathode 7-segment bit order.
 *
 * Key-scan byte (read from 0x27): bits 5:0 = key code, bit 6 = a key
 * is currently pressed (1) / no key pressed (0), per the TM1650/FD650B
 * "returns an 8-bit word ... bits 0-5 give the key code, bit 6
 * indicates press/release" behavior reported for this chip family.
 * Exact key-code-to-button mapping depends on your key matrix wiring
 * (KI/KS lines) -- check the table in your specific datasheet copy.
 */

#ifndef FD650B_H
#define FD650B_H

#include <stdint.h>
#include <stdbool.h>

#define FD650B_NUM_DIGITS   4

// 7-segment font, index 0-15 = '0'-'9','A'-'F', bit layout a..g,dp
// (bit0=a ... bit6=g, bit7=dp). No decimal point set by default.
extern const uint8_t display_font[16];

// Bring up the display: must be called after I2C1_Init().
// Leaves the display ON at mid brightness, all digits blank.
void display_init(void);

// Display on/off (keeps last-set brightness/segment-mode).
bool display_display_on(bool on);

// brightness: 0 (dimmest) .. 7 (brightest) -- see header note above.
bool display_set_brightness(uint8_t brightness);

// Write raw segment bits to one digit position (0 = leftmost .. 3 = rightmost).
bool display_write_digit_raw(uint8_t position, uint8_t segments);

// Convenience: render a hex nibble (0-15) with optional decimal point.
bool display_write_digit_hex(uint8_t position, uint8_t value, bool dot);

// Turn a digit fully off (blank).
bool display_clear_digit(uint8_t position);

// Blank all four digits.
bool display_clear_all(void);

// Display an unsigned 16-bit value (0-9999) right-justified across
// all 4 digits, with leading zeros blanked. Returns false if value > 9999.
bool display_write_number(uint16_t value);

// Read the key-scan register. Returns true and fills *key_code /
// *pressed on a successful bus transaction; false on I2C failure.
bool display_read_keys(uint8_t *key_code, bool *pressed);

#endif // FD650B_H
