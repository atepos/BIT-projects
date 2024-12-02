#include "state_machine.h"

unsigned char date[11];
unsigned char c;
unsigned int seconds_init;
unsigned int seconds_alarm;
unsigned int sec;
unsigned int tmp;

void state_machine() {

    int STATE = STATE_INPUT_DATE;


    Delay(10000);
    while (1) {
        switch (STATE) {
            case STATE_INPUT_DATE:
            	STATE = handle_input_date();
                break;
            case STATE_BEEPER:
            	STATE = handle_beeper();
                break;
            case STATE_LIGHTS:
            	STATE = handle_lights();
                break;
            case STATE_REPETITION:
            	STATE = handle_repetition();
                break;
            case STATE_DELAY:
            	STATE = handle_delay();
                break;
            case STATE_ALARM:
            	STATE = handle_alarm();
                break;
            case STATE_ACTIVE:
            	STATE = handle_active();
                break;
            case STATE_OFF:
                handle_off();
                break;
            default:
                break;
        }
    }
}

int handle_input_date() {
	bool is_err = false;
	print_start();

    print_new_line("Enter date in format \"YYYY-MM-DD_HH:MM:SS\".");
    print("Entry: ");
    ReceiveStr();
    print_new_line(mem);
    is_err = input_time(&seconds_init);

    if (is_err == false) {
        RTC_SR &= ~RTC_SR_TCE_MASK;
        RTC_TSR = seconds_init;
        RTC_SR |= RTC_SR_TCE_MASK;
        print_success("You have successfully input the time");
        return STATE_BEEPER;
    } else {
        print_repeat("Please, repeat the INIT process.");
    }
    Delay(10000);
    return STATE_INPUT_DATE;
}

void playMusic(int songNumber) {
    Music(songNumber);
}

void setFinalSong(int songNumber) {
    final_song = songNumber;
    print_success("You have successfully chosen the music");
}

void handleInvalidInput() {
    print_error("Not valid input");
}

int handle_beeper() {
	print_new_line("\033[36mMusic selection: \033[0m");
	print_new_line("Try which music is best for you : \"TRY[1-3]\"");
	print_new_line("Select music that you liked the most : \"FINAL[1-3]\"");
	print_new_line("You can go back to previous choosing : \"BACK\"");
	print("Entry: ");
	ReceiveStr();

    if (!strcmp(mem, "TRY1")) {
        playMusic(1);
    } else if (!strcmp(mem, "TRY2")) {
        playMusic(2);
    } else if (!strcmp(mem, "TRY3")) {
        playMusic(3);
    } else if (!strcmp(mem, "FINAL1")) {
        setFinalSong(1);
        return STATE_LIGHTS;
    } else if (!strcmp(mem, "FINAL2")) {
        setFinalSong(2);
        return STATE_LIGHTS;
    } else if (!strcmp(mem, "FINAL3")) {
        setFinalSong(3);
        return STATE_LIGHTS;
    } else if (!strcmp(mem, "BACK")) {
        print_success("You went back to choosing Clock date & time");
        return STATE_INPUT_DATE;
    } else {
        handleInvalidInput();
    }
	Delay(10000);
	return STATE_BEEPER;
}

void setLeds(int ledNumber) {
    Leds(ledNumber);
}

void setFinalLed(int ledNumber) {
    final_led = ledNumber;
    print_success("You have successfully chosen the Lights");
}

int handle_lights() {
	print_new_line("\033[36mLights selection: \033[0m");
	print_new_line("Try which light show is best for you : \"TRY[1-3]\"");
	print_new_line("And the select it : \"FINAL[1-3]\"");
	print_new_line("You can go back to previous choosing : \"BACK\"");
	print("Entry: ");
	ReceiveStr();

    if (!strcmp(mem, "TRY1")) {
        setLeds(1);
    } else if (!strcmp(mem, "TRY2")) {
        setLeds(2);
    } else if (!strcmp(mem, "TRY3")) {
        setLeds(3);
    } else if (!strcmp(mem, "FINAL1")) {
        setFinalLed(1);
        return STATE_REPETITION;
    } else if (!strcmp(mem, "FINAL2")) {
        setFinalLed(2);
        return STATE_REPETITION;
    } else if (!strcmp(mem, "FINAL3")) {
        setFinalLed(3);
        return STATE_REPETITION;
    } else if (!strcmp(mem, "BACK")) {
        print_success("You went back to choosing Beeper");
        return STATE_BEEPER;
    } else {
        handleInvalidInput();
    }

	Delay(10000);
	return STATE_LIGHTS;
}

int handle_repetition() {
	print_new_line("\033[36mRepetition selection: \033[0m");
	print_new_line("select the number of alarm repeats from 0 to 10");
	print_new_line("You can go back to previous choosing : \"BACK\"");
	print("Entry: ");
	ReceiveStr();

	int is_OK = sscanf(mem,"%d", &final_repetition);

	if(!strcmp(mem, "BACK")){
		print_success("You went back to choosing Lights");
		return STATE_LIGHTS;
	}

	if(is_OK == 1){
		if(final_repetition < 0 || final_repetition > 10){
			print_repeat("repetition must be in interval <0 to 10>");
		}else{
			if(final_repetition == 0){
				print_success("You have successfully chosen the repetition count");
				return STATE_ALARM;
			}else{
				print_success("You have successfully chosen the repetition count");
				return STATE_DELAY;
			}
		}
	}else{
		print_repeat("repetition must be in interval <0 to 10>");
	}

	Delay(10000);
	return STATE_REPETITION;
}

int handle_delay() {
	print_new_line("\033[36mDelay selection: \033[0m");
	print_new_line("select the time in seconds between alarm repetition from 20 to 1000");
	print_new_line("You can go back to previous choosing : \"BACK\"");
	print("Entry: ");
	ReceiveStr();

	if(!strcmp(mem, "BACK")){
		print_success("You went back to choosing repetition count");
		return STATE_REPETITION;
	}

	int is_OK = sscanf(mem,"%d", &final_delay);
	if(is_OK == 1){
		if (final_delay < 20 || 1000 < final_delay) {
			print_repeat("delay must be in interval <20 to 1000>");
		}
		else {
			print_success("You have successfully chosen the Delay time");
			return STATE_ALARM;
		}
	}else{
		print_repeat("repetition must be in interval <0 to 10>");
	}

	Delay(10000);
    return STATE_DELAY;
}

int handle_alarm() {
	bool is_err = false;

	print_new_line("\033[36mAlarm time selection: \033[0m");

	print_new_line("Enter the Alarm time in format \"YYYY-MM-DD_HH:MM:SS\".");
    print("Current date & time: ");
    sec = RTC_TSR;

    time_to_str(&sec);///////
    print_new_line(mem);

    print("Entry: ");
    ReceiveStr();


    is_err = input_time(&seconds_alarm);
    if ((is_err == false) && (RTC_TSR < seconds_alarm)) {
        RTC_TAR = seconds_alarm;
        print_success("You have successfully set the alarm");
        return STATE_ACTIVE;
    }
    else {
    	print_repeat("Please, repeat the ALARM INIT process.");
    }

	Delay(10000);
    return STATE_ALARM;
}


void printCurrentTime() {
    print("\033[104mCurrent time: \033[0m");
    sec = RTC_TSR;
    time_to_str(&sec);
    print_new_line(mem);
}

void printAlarmStatus() {
    sec = RTC_TAR;
    if (sec == 0) {
        print_new_line("OFF");
        print_end();
        print_new_line("Type \"HELP\" to find out what commands you can use now");
    } else {
        print("\033[103mAlarm set on: \033[0m");
        time_to_str(&sec);
        print_new_line(mem);
    }
}

void processTimeAndAlarm() {
    printCurrentTime();
    printAlarmStatus();
}

int handle_active() {

	processTimeAndAlarm();

	print("Entry: ");
	ReceiveStr();

	if(!strcmp(mem,"OFF")) {
		RTC_TAR = 0;
		return STATE_OFF;
	}
	else if(!strcmp(mem,"REBOOT")) {
		RTC_TAR = 0;
		return STATE_INPUT_DATE;
	}
	else if(!strcmp(mem,"DISABLE")) {
		RTC_TAR = 0;
		print_new_line("Alarm disabled.");
	}
	else if(!strcmp(mem,"NEW_ALL")) {
		RTC_TAR = 0;
		return STATE_BEEPER;
	}else if(!strcmp(mem, "NEW")){
		RTC_TAR = 0;
		return STATE_ALARM;
	}
	else if(!strcmp(mem,"HELP")) {
		print_new_line("\033[32m HELP: \"OFF\" / \"REBOOT\" / \"DISABLE\" / \"NEW\" / \"NEW_ALL\" / \"HELP\" \033[0m");
		print_new_line("Commands:");
		print_new_line("\"OFF\" - turns of the alarm");
		print_new_line("\"REBOOT\" - reboot the clock completely");
		print_new_line("\"DISABLE\" - disable the alarm");
		print_new_line("\"NEW\" - you can set new alarm");
		print_new_line("\"NEW_ALL\" - you can set new alarm and select alarm setting");
		print_new_line("\"HELP\" - prints help text");
		print_new_line("");
	}

	return STATE_ACTIVE;
	Delay(10000);
}

void handle_off() {

    print_new_line("System is off-line");
    while (1) {};
}
