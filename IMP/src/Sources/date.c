#include "date.h"

void time_to_str(unsigned int* source_time) {
    time_t tmp_time = *source_time;
    struct tm ts_time = *localtime(&tmp_time);

    for (unsigned int i=0; i<MEM_SIZE; i++) {
        mem[i]='\0';
    }
    strftime(mem, MEM_SIZE, "%Y-%m-%d_%H:%M:%S", &ts_time);
}


bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool validateDate(int year, int month, int day) {
    if (year < 1970 || year > 2050) {
        return true; // Error: Wrong year!
    }

    if (month < 1 || month > 12) {
        return true; // Error: Wrong month!
    }

    if (month == 2) { // February
        if (isLeapYear(year)) {
            if (day < 1 || day > 29) return true; // Error: Wrong day!
        } else {
            if (day < 1 || day > 28) return true; // Error: Wrong day!
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day < 1 || day > 30) return true; // Error: Wrong day!
    } else {
        if (day < 1 || day > 31) return true; // Error: Wrong day!
    }

    return false; // Date is valid
}

bool validateTime(int hour, int minute, int second) {
    if (hour < 0 || hour > 23) return true; // Error: Wrong hour!
    if (minute < 0 || minute > 59) return true; // Error: Wrong minute!
    if (second < 0 || second > 59) return true; // Error: Wrong second!
    return false; // Time is valid
}

bool input_time(unsigned int* final_time) {
    int len = strlen(mem);
    struct tm t;


    if (len != 19) {
    	print_error("length not 19");
    	 return true;
    }

    if(sscanf(mem, "%d-%d-%d_%d:%d:%d", &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec) != 6){
    	print_error("parsing date");
    	 return true;
    }


    if (validateDate(t.tm_year, t.tm_mon, t.tm_mday) || validateTime(t.tm_hour, t.tm_min, t.tm_sec)) {
        return true; // Date or time validation error
    }

    t.tm_year -= 1900;
    t.tm_mon--;
    t.tm_isdst = -1;

    time_t tmp = mktime(&t);
    *final_time = (unsigned int)tmp;

    return false; // No errors
}

