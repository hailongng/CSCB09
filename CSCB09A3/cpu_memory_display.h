#ifndef CPU_MEMORY_DISPLAY_H
#define CPU_MEMORY_DISPLAY_H

#include <stdio.h>

void basic_info_print(int sample_count, int tdelay);
void print_memory(char graph[13][85], int sample_count, int tdelay, double ram_used, double total_ram_in_gb);
void print_cpu(char graph[13][85], int sample_count, int tdelay, double ratio);

#endif