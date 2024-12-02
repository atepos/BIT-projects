#ifndef UTILITIES_H
#define UTILITIES_H

#include "MK60D10.h"
#include "string.h"
#include "str.h"
#include "print.h"
#include "stdio.h"

#define D9  0x20
#define D10 0x10
#define D11 0x8
#define D12 0x4
#define D12__D11 0xC
#define D12_D11_D10 0x1C
#define ALL 0x3C


unsigned int final_song;
unsigned int final_led;
unsigned int final_repetition;
unsigned int final_delay;

void Delay(unsigned long long int bound);
void Music(int type);
void Leds(int type);
void Beep();
void handleRTCAlarm();
void RTC_IRQHandler();

#endif // UTILITIES_H
