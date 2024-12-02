
#include "utilities.h"


void Delay(unsigned long long int bound) {
    for(unsigned long long int i=0; i<bound; i++);
}


void Music(int type) {

	switch(type){
	case 1:
		for(int i = 0; i < 15; i++)
		{
			Beep(1);
			Delay(50000);
		}
		break;

	case 2:
        for(int i = 0; i < 10; i++)
        {
            Beep(2);
            Delay(50000);
        }
        break;

	case 3:
        for(int i = 0; i < 6; i++)
        {
        	Beep(1);
            Delay(500);
            Beep(1);
            Delay(50000);
            Beep(1);
        }
        break;
	}
}

void Beep(int type) {

	if(type == 1){
		for (unsigned int q=0; q<500; q++) {
			PTA->PDOR = GPIO_PDOR_PDO(0x0010);
			Delay(500);
			PTA->PDOR = GPIO_PDOR_PDO(0x0000);
			Delay(500);
		}
	}else if(type == 2){
	    unsigned int delay = 1000;

	    for (unsigned int q = 0; q < 500; q++) {
	        PTA->PDOR = GPIO_PDOR_PDO(0x0010);
	        Delay(delay);

	        PTA->PDOR = GPIO_PDOR_PDO(0x0000);
	        Delay(delay);

	        delay += 10;
	    }
	}
}



void Leds(int type) {

	switch(type){
	case 1:
        for(int i = 0; i < 20; i++)
        {

            PTB->PDOR &= ~GPIO_PDOR_PDO(ALL);
            Delay(200000);
            PTB->PDOR |= GPIO_PDOR_PDO(ALL);
            Delay(200000);

        }
        break;

	case 2:
		for(int i = 0; i < 20; i++)
		{

			GPIOB_PDOR ^= D9;
			PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
			Delay(200000);
			PTB->PDOR |= GPIO_PDOR_PDO(ALL);

			if(i % 2 == 0){
				GPIOB_PDOR ^= D10;
				PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
				Delay(200000);
				PTB->PDOR |= GPIO_PDOR_PDO(ALL);
			}

			GPIOB_PDOR ^= D11;
			PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
			Delay(200000);
			PTB->PDOR |= GPIO_PDOR_PDO(0x3C);

			if(i % 3){
				GPIOB_PDOR ^= D12;
				PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
				Delay(200000);
				PTB->PDOR |= GPIO_PDOR_PDO(ALL);
			}
		}
		break;
	case 3:
        for(int i = 0; i < 15; i++)
        {

			GPIOB_PDOR ^= D12;
			PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
			Delay(200000);
			PTB->PDOR |= GPIO_PDOR_PDO(ALL);

			GPIOB_PDOR ^= D12__D11;
			PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
			Delay(200000);
			PTB->PDOR |= GPIO_PDOR_PDO(ALL);

			GPIOB_PDOR ^= D12_D11_D10;
			PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
			Delay(200000);
			PTB->PDOR |= GPIO_PDOR_PDO(0x3C);

			GPIOB_PDOR ^= ALL;
			PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
			Delay(200000);
			PTB->PDOR |= GPIO_PDOR_PDO(ALL);

        }
        break;
	}
}


void handleRTCAlarm() {
    char tmp[10];
    sprintf(tmp, "%d", final_repetition);

    print("ALARM WORKING: remaining repetitions: ");
    print(tmp);
    print("\r\n");

    Music(final_song);
    Leds(final_led);

    if (final_repetition > 0) {
        final_repetition--;
        RTC_TAR += final_delay;
    } else {
        RTC_TAR = 0;
    }
}

void RTC_IRQHandler() {
    if (RTC_SR & RTC_SR_TAF_MASK) {
        handleRTCAlarm();
    }
}

