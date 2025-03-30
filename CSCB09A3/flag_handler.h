#ifndef FLAG_HANDLER_H
#define FLAG_HANDLER_H

#include <stdio.h>
#include<ctype.h>
#include <string.h>

void flag_checker(int argc, char** argv, char* str, int* flag);
int tdelay_checker(int argc, char** argv, int* val);
int sample_count_checker(int argc, char** argv, int* val);
int weird_flag_checker(int argc, char** argv);
int isnumber(char* str);
int duplicate_flag_checker(int argc, char** argv);

#endif