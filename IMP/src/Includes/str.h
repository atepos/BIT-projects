#ifndef STR_H
#define STR_H

#include "MK60D10.h"
#include "utilities.h"
#include "string.h"
#include "print.h"

#define MEM_SIZE 100

char mem[MEM_SIZE];

void SendCh(char c);
void SendStr(char* s);
unsigned char ReceiveCh();
void ReceiveStr();


#endif //STR_H
