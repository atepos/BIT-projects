
#include "init.h"

void MCUInit(void) {
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}


void UART5Init(void) {
    UART5->C2  &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

    UART5->BDH =  0x00;
    UART5->BDL =  0x1A; // Baud rate 115 200 Bd, 1 stop bit
    UART5->C4  =  0x0F; // Oversampling ratio 16, match address mode disabled

    UART5->C1  =  0x00; // 8 data bitu, bez parity
    UART5->C3  =  0x00;
    UART5->MA1 =  0x00; // no match address (mode disabled in C4)
    UART5->MA2 =  0x00; // no match address (mode disabled in C4)
    UART5->S2  |= 0xC0;

    UART5->C2  |= ( UART_C2_TE_MASK | UART_C2_RE_MASK ); // Zapnout vysilac i prijimac

}

void PinInit(void) {
    SIM->SCGC1 = SIM_SCGC1_UART5_MASK;
    SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK;
    SIM->SCGC6 = SIM_SCGC6_RTC_MASK;

    // PORT A
    PORTA->PCR[4] = PORT_PCR_MUX(0x01);

    // PORT B
    PORTB->PCR[5] = PORT_PCR_MUX(0x01);
    PORTB->PCR[4] = PORT_PCR_MUX(0x01);
    PORTB->PCR[3] = PORT_PCR_MUX(0x01);
    PORTB->PCR[2] = PORT_PCR_MUX(0x01);

    // PORT E
    PORTE->PCR[8]  = PORT_PCR_MUX(0x03);
    PORTE->PCR[9]  = PORT_PCR_MUX(0x03);
    PORTE->PCR[10] = PORT_PCR_MUX(0x01);
    PORTE->PCR[12] = PORT_PCR_MUX(0x01);
    PORTE->PCR[27] = PORT_PCR_MUX(0x01);
    PORTE->PCR[26] = PORT_PCR_MUX(0x01);
    PORTE->PCR[11] = PORT_PCR_MUX(0x01);

    // set ports as output
    PTA->PDDR =  GPIO_PDDR_PDD(0x0010);
    PTB->PDDR =  GPIO_PDDR_PDD(0x3C);
    PTB->PDOR |= GPIO_PDOR_PDO(0x3C);
}

void RTCInit(void) {
    RTC_CR |= RTC_CR_SWR_MASK;
    RTC_CR &= ~RTC_CR_SWR_MASK;

    RTC_TCR = 0x0000;

    RTC_CR |= RTC_CR_OSCE_MASK;

    Delay(0x600000);

    RTC_SR &= ~RTC_SR_TCE_MASK;

    RTC_TSR = 0x00000000;
    RTC_TAR = 0xFFFFFFFF;

    RTC_IER |= RTC_IER_TAIE_MASK;

    NVIC_ClearPendingIRQ(RTC_IRQn);
    NVIC_EnableIRQ(RTC_IRQn);

    RTC_SR |= RTC_SR_TCE_MASK;
}

