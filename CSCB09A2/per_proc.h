#define _GNU_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<ctype.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "linked_list.h"

int is_pid(char* str);
void fd_process(pid_t pid, ll_t* list);
void all_process_handling();
