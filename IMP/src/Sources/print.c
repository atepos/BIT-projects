
#include "print.h"

void print_success(unsigned char *text) {
    SendStr(text);
    SendStr("\r\n");
    SendStr("\033[32m------------------------------\033[0m\n");
    SendStr("\r\n");
}

void print_repeat(unsigned char *text){
	SendStr("\033[31m!!!!!!!\033[0m");
    SendStr(text);
    SendStr("\033[31m!!!!!!!\033[0m\r\n");
}

void print_new_line(unsigned char *msg) {
    SendStr(msg);
    SendStr("\r\n");
}

void print(unsigned char *msg){
	SendStr(msg);
}

void print_error(unsigned char *text){
	SendStr("\033[31mERROR: \033[0m");
    SendStr(text);
    SendStr("\r\n");
}

void print_start(){
	SendStr("\033[32mWelcome in Alarm APP  \033[0m");
	SendStr("\r\n");
}

void print_end(){
    SendStr("\r\n");
    SendStr("\033[32m------------------------------\033[0m\n");
    SendStr("\r\n");
    SendStr("\033[32m------------------------------\033[0m\n");
    SendStr("\r\n");
}
