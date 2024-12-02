#ifndef DATE_H
#define DATE_H

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include "str.h"
#include "print.h"

void time_to_str(unsigned int* source_time);
bool input_time(unsigned int* final_time);
bool validateTime(int hour, int minute, int second);
bool validateDate(int year, int month, int day);
bool isLeapYear(int year);

#endif // DATE_H
