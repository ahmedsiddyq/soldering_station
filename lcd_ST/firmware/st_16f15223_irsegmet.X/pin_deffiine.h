#define I2C1_SCL_TRIS   TRISCbits.TRISC0
#define I2C1_SDA_TRIS   TRISCbits.TRISC1
#define I2C1_SCL_ANSEL  ANSELCbits.ANSC0
#define I2C1_SDA_ANSEL  ANSELCbits.ANSC1

void pins (){
TRISAbits.TRISA5 = 1;   // TEMP input
TRISAbits.TRISA4 = 1;   // temp_set input
TRISCbits.TRISC5 = 0;   // heater_gate output
TRISCbits.TRISC3 = 1;   // current input
//TRISCbits.TRISC1 = 0;   // clk output
//TRISCbits.TRISC0 = 0;   // dat output
TRISCbits.TRISC2 = 0;   // UART TX output
TRISAbits.TRISA2 = 1;   // UART RX input

ANSELCbits.ANSC5 = 0;
ANSELCbits.ANSC2 = 0;
ANSELCbits.ANSC0 = 0;
ANSELCbits.ANSC1 = 0;
ANSELAbits.ANSA2 = 0;

    // Unlock PPS
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;
	
    // =========================
    // UART1
    // =========================

    // UART TX1 -> RC2
    RC2PPS = 0x05;

    // UART RX1 <- RA2

    RXPPS  = 0x02;
    RC5PPS = 0x03;     
    
    I2C1_SCL_ANSEL = 0;      // digital, not analog
    I2C1_SDA_ANSEL = 0;
    I2C1_SCL_TRIS  = 1;      // start as inputs; MSSP takes over the
    I2C1_SDA_TRIS  = 1;      // output drive automatically when SSPEN=1

    SSP1CLKPPS = 0x10; // RC0 -> SSP1 clock input
    SSP1DATPPS = 0x11; // RC1 -> SSP1 data input
    RC0PPS     = 0x07; // RC0 <- SCL1 output
    RC1PPS     = 0x08; // RC1 <- SDA1 output
    RC0I2Cbits.PU = 1;       // weak pull-up on SCL1
    RC1I2Cbits.PU = 1;       // weak pull-up on SDA1
    // =========================
    // GPIO pins
    // =========================

    // RA5 = TEMP
    // No PPS -> GPIO

    // RA4 = temp_set
    // No PPS -> GPIO

    // RC5 = heater_gate
    // No PPS -> GPIO

    // RC3 = current
    // No PPS -> GPIO

    // RC1 = clk
    // No PPS -> GPIO

    // RC0 = dat
    // No PPS -> GPIO


    // Lock PPS
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;
    
}