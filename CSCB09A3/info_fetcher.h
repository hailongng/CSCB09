#ifndef INFO_FETCHER_H
#define INFO_FETCHER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/resource.h>
#include<sys/sysinfo.h>
#include<ctype.h>
#include<sys/types.h>
#include<unistd.h>

int sysinfo(struct sysinfo *info);
double grab_memory_info();
void grab_cpu_info(int* cpudata);
void core_display();
void core_row_display(int cores_per_row);
long long fetch_total_ram();

#endif