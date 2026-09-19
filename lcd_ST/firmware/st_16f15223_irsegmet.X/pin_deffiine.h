// PIC16F15223T-I/SL pin map (from schematic)
//
// RA5 (pin 2)  = TEMP
// RA4 (pin 3)  = temp_set
// RA3 (pin 4)  = MCLR/VPP
// RC5 (pin 5)  = current
// RC4 (pin 6)  = heater_gate
// RC3 (pin 7)  = not connected
// RC2 (pin 8)  = clk  -> I2C SCL
// RC1 (pin 9)  = dat  -> I2C SDA
// RC0 (pin 10) = TX
// RA2 (pin 11) = RX
// RA1 / RA0    = ICSPCLK / ICSPDAT (leave alone)

#define I2C1_SCL_TRIS   TRISCbits.TRISC2
#define I2C1_SDA_TRIS   TRISCbits.TRISC1
#define I2C1_SCL_ANSEL  ANSELCbits.ANSC2
#define I2C1_SDA_ANSEL  ANSELCbits.ANSC1


void pins(void)
{
    // =========================
    // GPIO direction
    // =========================
    TRISAbits.TRISA5 = 1;   // TEMP        = RA5, input
    TRISAbits.TRISA4 = 1;   // temp_set    = RA4, input

    TRISCbits.TRISC4 = 0;   // heater_gate = RC4, output
    TRISCbits.TRISC5 = 1;   // current     = RC5, input

    TRISCbits.TRISC0 = 0;   // TX          = RC0, output
    TRISAbits.TRISA2 = 1;   // RX          = RA2, input

    // RC3 is unconnected: drive it low as an output so it doesn't float
    LATCbits.LATC3   = 0;
    TRISCbits.TRISC3 = 0;


    // =========================
    // Analog / Digital
    // =========================
 
    ANSELAbits.ANSA5 = 1;   // TEMP
    ANSELAbits.ANSA4 = 1;   // temp_set
    ANSELAbits.ANSA2 = 0;   // RX

    ANSELCbits.ANSC5 = 1;   // current
    ANSELCbits.ANSC4 = 0;   // heater_gate
    ANSELCbits.ANSC3 = 0;   // unused
    ANSELCbits.ANSC0 = 0;   // TX
    I2C1_SCL_ANSEL   = 0;   // RC2 (SCL)
    I2C1_SDA_ANSEL   = 0;   // RC1 (SDA)


    // =========================
    // Unlock PPS
    // =========================

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;


    // =========================
    // UART1
    // =========================

    // TX1 -> RC0
    RC0PPS = 0x05;

    // RX1 <- RA2
    RXPPS = 0x02;
    
    
// PWM3 -> RC4 (heater_gate)
RC4PPS = 0x03;      // PWM3

    // =========================
    // I2C1
    // =========================

    // Both pins stay inputs; the MSSP drives them open-drain as needed
    I2C1_SCL_TRIS = 1;
    I2C1_SDA_TRIS = 1;

    // I2C inputs
    SSP1CLKPPS = 0x12;      // RC2 -> SCL1 input
    SSP1DATPPS = 0x11;      // RC1 -> SDA1 input

    // I2C outputs (must be on the same pins as the inputs)
    RC2PPS = 0x07;          // SCL1 -> RC2
    RC1PPS = 0x08;          // SDA1 -> RC1

    // Weak pull-ups (verify RC2I2C / RC1I2C exist on this device;
    // otherwise use WPUC, and prefer external resistors for real I2C)
WPUCbits.WPUC2 = 1;     // SCL (RC2)
WPUCbits.WPUC1 = 1;     // SDA (RC1)
    // =========================
    // Lock PPS
    // =========================

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;
}