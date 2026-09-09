
void pins (){
TRISAbits.TRISA5 = 1;   // TEMP input
TRISAbits.TRISA4 = 1;   // temp_set input
TRISCbits.TRISC5 = 0;   // heater_gate output
TRISCbits.TRISC3 = 1;   // current input
TRISCbits.TRISC1 = 0;   // clk output
TRISCbits.TRISC0 = 0;   // dat output
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