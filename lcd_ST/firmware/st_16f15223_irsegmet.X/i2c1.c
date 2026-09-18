 #include "i2c1.h"

// ---- Pin routing (see header comment: default SCL1=RC0, SDA1=RC1) ----
#define I2C1_SCL_TRIS   TRISCbits.TRISC0
#define I2C1_SDA_TRIS   TRISCbits.TRISC1
#define I2C1_SCL_ANSEL  ANSELCbits.ANSC0
#define I2C1_SDA_ANSEL  ANSELCbits.ANSC1

// PPS input port/pin code = (PORT_INDEX << 3) | PIN_NUMBER, PORT A=0,B=1,C=2...
#define PPS_IN_RC0      0x10u   // (2<<3)|0
#define PPS_IN_RC1      0x11u   // (2<<3)|1//data

// PPS output source codes -- from datasheet Table 18-2
// "PPS Output Selection Table" (DS40002195): 0x07 = SCL1/SCK1, 0x08 = SDA1/SDO1
#define PPS_OUT_SCL1SCK1 0x07u
#define PPS_OUT_SDA1SDO1 0x08u



static void I2C1_WaitIdle(void)
{
    // Bus is busy while any of the auto-clearing sequence bits are set,
    // or while the module is actively shifting a byte out (R_nW=1
    // during master transmit).
    while ((SSP1CON2 & 0x1Fu) || SSP1STATbits.R_nW) {
        ;
    }
}

static void I2C1_Start(void)
{
    I2C1_WaitIdle();
    SSP1CON2bits.SEN = 1;   // generate Start condition (self-clears)
    while (SSP1CON2bits.SEN) { ; }
}

static void I2C1_RepeatedStart(void)
{
    I2C1_WaitIdle();
    SSP1CON2bits.RSEN = 1;  // generate Restart condition (self-clears)
    while (SSP1CON2bits.RSEN) { ; }
}

static void I2C1_Stop(void)
{
    I2C1_WaitIdle();
    SSP1CON2bits.PEN = 1;   // generate Stop condition (self-clears)
    while (SSP1CON2bits.PEN) { ; }
}

// Send one byte, return true if the slave ACKed it.
static bool I2C1_PutByte(uint8_t b)
{
    I2C1_WaitIdle();
    SSP1BUF = b;
    I2C1_WaitIdle();                 // wait for the 9 clocks (8 data + ACK) to finish
    return (SSP1CON2bits.ACKSTAT == 0);  // ACKSTAT: 0 = ACK, 1 = NACK
}

// Receive one byte; ACK it if 'ackIt' is true, otherwise NAK it (used
// to terminate a read per the I2C spec).
static uint8_t I2C1_GetByte(bool ackIt)
{
    I2C1_WaitIdle();
    SSP1CON2bits.RCEN = 1;           // enable receive mode (self-clears when byte is in)
    while (SSP1CON2bits.RCEN) { ; }
    uint8_t b = SSP1BUF;

    I2C1_WaitIdle();
    SSP1CON2bits.ACKDT = ackIt ? 0 : 1;  // 0 = send ACK, 1 = send NACK
    SSP1CON2bits.ACKEN = 1;              // initiate ACK/NACK sequence (self-clears)
    while (SSP1CON2bits.ACKEN) { ; }

    return b;
}

void I2C1_Init(void)
{
     // --- Digital I/O + PPS routing ---

     // --- Dedicated I2C pad control (RxyI2C), valid because RC0/RC1
    // are this device's DEFAULT MSSP1 pins (datasheet Table 18-1).
    // If you move SDA/SCL to non-default pins with PPS, delete this
    // block and configure WPUx / ODCONx / SLRCONx / INLVLx by hand
    // instead (see datasheet section "Dedicated I2C Pad Control
    // Registers").

    // NOTE: internal pull-ups are weak (~ tens of kOhm) and mainly a
    // fallback. The FD650B needs proper external pull-ups (2.2k-4.7k
    // to VDD on both SDA and SCL) per standard I2C practice -- add
    // them on the board even with RxyI2C.PU enabled.

    // --- MSSP1 module: I2C Master mode ---
    SSP1CON1 = 0x00;
    SSP1CON1bits.SSPM  = 0b1000;   // I2C Master mode, clock = Fosc/(4*(SSP1ADD+1))
    SSP1STATbits.SMP   = 1;        // slew rate control disabled (100 kHz Standard mode)
    SSP1STATbits.CKE   = 0;        // standard I2C input levels (not SMBus)

    // Baud rate generator: SSP1ADD = (Fosc / (4 * I2C1_CLOCK_HZ)) - 1
    SSP1ADD = (uint8_t)(((_XTAL_FREQ / 4UL) / I2C1_CLOCK_HZ) - 1UL);

    SSP1CON1bits.SSPEN = 1;        // enable MSSP1, pins now under module control
}

bool I2C1_Write(uint8_t addr7, const uint8_t *data, uint8_t len)
{
    bool ok = true;

    I2C1_Start();
    ok = I2C1_PutByte((uint8_t)(addr7 << 1) | 0u); // address + Write bit

    for (uint8_t i = 0; ok && (i < len); i++) {
        ok = I2C1_PutByte(data[i]);
    }

    I2C1_Stop();
    return ok;
}

bool I2C1_WriteByte(uint8_t addr7, uint8_t data)
{
    return I2C1_Write(addr7, &data, 1);
}

bool I2C1_Read(uint8_t addr7, uint8_t *data, uint8_t len)
{
    if (len == 0) return false;

    I2C1_Start();
    bool ok = I2C1_PutByte((uint8_t)(addr7 << 1) | 1u); // address + Read bit
    if (!ok) {
        I2C1_Stop();
        return false;
    }

    for (uint8_t i = 0; i < len; i++) {
        bool lastByte = (i == (len - 1));
        data[i] = I2C1_GetByte(!lastByte); // ACK all but the last byte
    }

    I2C1_Stop();
    return true;
}
