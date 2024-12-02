
#include "str.h"

/* vyslani jednoho znaku (ch) pres UART - funkce vycka, az je vysilaci buffer prazdny, pak posle */
void SendCh(char c) {
    while( !(UART5->S1 & UART_S1_TDRE_MASK) && !(UART5->S1 & UART_S1_TC_MASK) );
    UART5->D = c;
}

/* vysilani retezce ukonceneho 0 */
void SendStr(char* s) {
    unsigned int i = 0;
    while (s[i] != '\0') {
        SendCh(s[i++]);
    }
}


/* prijeti jednoho znaku pres UART - funkce ceka na prichozi znak a ten vrati jako vysledek */
unsigned char ReceiveCh() {
    while( !(UART5->S1 & UART_S1_RDRF_MASK) );
    return UART5->D;
}


void ReceiveStr() {

    unsigned int i = 0;

    memset(mem, '\0', MEM_SIZE);

    for (; i  < (MEM_SIZE - 1) ;) {

    	unsigned char c = ReceiveCh();
        SendCh(c);

        if (c == '\r') {
            break;
        }

        mem[i] = c;
        i++;
    }

    mem[i] = '\0';
    print(" ");

}
