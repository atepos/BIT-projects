
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "init.h"
#include "utilities.h"
#include "str.h"
#include "date.h"
#include "print.h"

#include "MK60D10.h"


enum State {
	STATE_INPUT_DATE,
    STATE_BEEPER,
    STATE_LIGHTS,
    STATE_REPETITION,
    STATE_DELAY,
    STATE_ALARM,
	STATE_ACTIVE,
	STATE_OFF
};

void state_machine();
int handle_input_date();
int handle_beeper();
int handle_lights();
int handle_repetition();
int handle_delay();
int handle_alarm();
int handle_active();
void handle_off();
void printCurrentTime();
void printAlarmStatus();
void processTimeAndAlarm();
void setLeds(int ledNumber);
void setFinalLed(int ledNumber);
void playMusic(int songNumber);
void setFinalSong(int songNumber);
void handleInvalidInput();

#endif //STATE_MACHINE_H
