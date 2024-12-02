#ifndef PRINT_H
#define PRINT_H

#include "MK60D10.h"
#include "str.h"
#include <stdbool.h>

void print_success(unsigned char *text);
void print_repeat(unsigned char *text);
void print(unsigned char *msg);
void print_error(unsigned char *text);
void print_new_line(unsigned char *msg);
void print_start();
void print_end();

#endif //PRINT_H
