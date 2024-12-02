/*************************************************************************************************/
/*                                                                                               */
/*  Projekt z predmetu IMP - ARM@FITkit3: Hodiny s budíkem na bázi modulu Real Time Clock (RTC)  */
/*                                                                                               */
/*  Reseni vytvoril a odevzdava: Petr Kaška, xkaska01                                            */
/*                                                                                               */
/*************************************************************************************************/

#include "MK60D10.h"
#include "state_machine.h"

int main(void)
{
    MCUInit();
    PinInit();
    UART5Init();
    RTCInit();

    state_machine();

    return 0;
}
