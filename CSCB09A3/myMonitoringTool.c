#define _DEFAULT_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/resource.h>
#include<sys/sysinfo.h>
#include<ctype.h>
#include<sys/types.h>
#include<math.h>
#include<unistd.h>
#include "aux.h"
#include "info_fetcher.h"
#include "flag_handler.h"
#include "signal_handler.h"

#ifndef MAXIMUM_CLA
#define MAXIMUM_CLA 6
#endif

#ifndef DEFAULT_SAMPLE_SIZE
#define DEFAULT_SAMPLE_SIZE 20
#endif

#ifndef MAXIMUM_SAMPLE_SIZE
#define MAXIMUM_SAMPLE_SIZE 80
#endif

#ifndef DEFAULT_TIME_DELAY
#define DEFAULT_TIME_DELAY 500000
#endif

#ifndef MINIMUM_TIME_DELAY
#define MINIMUM_TIME_DELAY 100000
#endif

#ifndef MAXIMUM_TIME_DELAY
#define MAXIMUM_TIME_DELAY 10000000
#endif

int sysinfo(struct sysinfo *info);
int get_nprocs_conf(void);

void memory_display(int sample_count, int tdelay) {
	char graph[13][85];
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 85; j++) {
				graph[i][j] = ' ';
		}
	}
	for (int j = 0; j <= sample_count; j++) {
		graph[9][j] = '_';
	}
	double total_ram = 0.0, ram_used = 0.0, freeram = 0.0;
	// int total_ram_in_byte = 0;
	struct sysinfo* si = calloc(1, sizeof(struct sysinfo));
	int ret = sysinfo(si);
	if (ret == 0) {
		total_ram = (double)si->totalram;
	}
	free(si);
	double total_ram_in_gb = total_ram / 1073741824;
	for (int instance = 0; instance < sample_count; instance++) {
		printf("\033[2J");
		printf("\033[H");
		usleep(tdelay);
		grab_memory_info(&freeram);
		ram_used = total_ram - freeram;
		double f_index_to_find = ram_used * 100.0 / total_ram;
		int index_to_find = find_index(f_index_to_find);
		graph[index_to_find][instance] = '#';
		printf("RAM used: %.4f GB\n", ram_used / 1073741824);
		for (int i = 0; i <= 9; i++) {
            if (i == 0) {
                printf("%.1fGB\t|", total_ram_in_gb);
            }
            else if (i == 9) {
            	printf("0GB\t|");
            }
            else {
                printf("\t|");
            }
            for (int j = 0; j < sample_count; j++) {
                printf("%c", graph[i][j]);
        	}
            printf("\n");
        }
	}
}

void cpu_display(int sample_count, int tdelay)  {
	char graph[13][85];
	int cpu_data[7];
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 85; j++) {
			graph[i][j] = ' ';
		}
	}
	for (int j = 0; j <= sample_count; j++) {
		graph[9][j] = '_';
	}
	int total_cpu_util_time_1 = 0, total_cpu_util_time_2 = 0, total_cpu_time_1 = 0, total_cpu_time_2 = 0;
	grab_cpu_info(cpu_data);
	for (int i = 0; i <= 6; i++) {
		total_cpu_time_1 += cpu_data[i];
	}
	total_cpu_util_time_1 = total_cpu_time_1 - cpu_data[3];
	for (int instance = 0; instance < sample_count; instance++) {
		printf("\033[2J");
		printf("\033[H");
		total_cpu_time_2 = 0;
		usleep(tdelay);
		grab_cpu_info(cpu_data);
		for (int i = 0; i <= 6; i++) {
			total_cpu_time_2 += cpu_data[i];
		}
		total_cpu_util_time_2 = total_cpu_time_2 - cpu_data[3];
		double ratio = ((double)100.0 * (total_cpu_util_time_2 - total_cpu_util_time_1)) / 
						((double)(total_cpu_time_2 - total_cpu_time_1));
		int index_to_find = find_index(ratio);
		graph[index_to_find][instance] = ':';
		printf("CPU Usage: %f %% \t\n", ratio);
		for (int i = 0; i <= 9; i++) {
			if (i == 0) {
				printf("100%%\t|");
			}
			else if (i == 9) {
				printf("0%%\t|");
			}
			else {
				printf("\t|");
			}
			for (int j = 0; j < sample_count; j++) {
				printf("%c", graph[i][j]);
			}
			printf("\n");
		}
		total_cpu_time_1 = total_cpu_time_2;
		total_cpu_util_time_1 = total_cpu_util_time_2;
	}
}

void duo_display(int sample_count, int tdelay) {

	char memory_graph[13][85], cpu_graph[13][85];
      	int cpu_data[7];
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 85; j++) {
			memory_graph[i][j] = ' ';
			cpu_graph[i][j] = ' ';
		}
	}
	for (int j = 0; j <= sample_count; j++) {
		memory_graph[9][j] = '_';
		cpu_graph[9][j] = '_';
	}
	double total_ram = 0.0, ram_used = 0.0, freeram = 0.0;
	struct sysinfo* si = NULL;
	si = calloc(1, sizeof(struct sysinfo));
	int ret = sysinfo(si);
	if (ret == 0) {
		total_ram = (double)si->totalram;
	}
	free(si);
	double total_ram_in_gb = total_ram / 1073741824;
	int total_cpu_util_time_1 = 0, total_cpu_util_time_2 = 0, total_cpu_time_1 = 0, total_cpu_time_2 = 0;
	grab_cpu_info(cpu_data);
	for (int i = 0; i <= 6; i++) {
		total_cpu_time_1 += cpu_data[i];
	}
	total_cpu_util_time_1 = total_cpu_time_1 - cpu_data[3];
	for (int instance = 0; instance < sample_count; instance++) {
		printf("\033[2J");
		printf("\033[H");
		usleep(tdelay);
		// STEP 1: MEMORY
		grab_memory_info(&freeram);
		ram_used = total_ram - freeram;
		int memory_index_to_find = find_index(ram_used * 100.0 / total_ram);
		memory_graph[memory_index_to_find][instance] = '#';
		// STEP 2: CPU
		total_cpu_time_2 = 0;
		grab_cpu_info(cpu_data);
		for (int i = 0; i <= 6; i++) {
			total_cpu_time_2 += cpu_data[i];
		}
		total_cpu_util_time_2 = total_cpu_time_2 - cpu_data[3];
		double ratio = ((double)100.0 * (total_cpu_util_time_2 - total_cpu_util_time_1)) / 
						((double)(total_cpu_time_2 - total_cpu_time_1));
		int cpu_index_to_find = find_index(ratio);
		cpu_graph[cpu_index_to_find][instance] = ':';
		printf("RAM used: %.4f GB\n", ram_used / 1073741824);
		for (int i = 0; i <= 9; i++) {
			if (i == 0) {
				printf("%.1fGB\t|", total_ram_in_gb);
			}
			else if (i == 9) {
				printf("0GB\t|");
			}
			else {
				printf("\t|");
			}
			for (int j = 0; j < sample_count; j++) {
				printf("%c", memory_graph[i][j]);
			}
			printf("\n");
		}
		printf("CPU Usage: %f %% \n", ratio);
		for (int i = 0; i <= 9; i++) {
			if (i == 0) {
				printf("100%%\t|");
			}
			else if (i == 9) {
				printf("0%%\t|");
			}
			else {
				printf("\t|");
			}
			for (int j = 0; j < sample_count; j++) {
				printf("%c", cpu_graph[i][j]);
			}
			printf("\n");
		}
        total_cpu_time_1 = total_cpu_time_2;
		total_cpu_util_time_1 = total_cpu_util_time_2;
	}
}

int main (int argc, char** argv) {
	if (signal(SIGTSTP, handler_ctrl_z) == SIG_ERR) {
		perror("Failed to register signal handler for Ctrl Z\n");
		return 1;
	}
	if (signal(SIGINT, handler_ctrl_c) == SIG_ERR) {
		perror("Failed to register signal handler for Ctrl C\n");
		return 1;
	}
	int core_flag = 0, cpu_flag = 0, memory_flag = 0;
	int sample_count = DEFAULT_SAMPLE_SIZE;
	int tdelay = DEFAULT_TIME_DELAY;
	int check_sample_count = 0, check_tdelay = 0;
	if (argc > MAXIMUM_CLA) {
		printf("Too much argument. The program will terminate.\n\n");
		return 1;
	}
	if (argc == 1) {
		core_flag = 1;
		cpu_flag = 1;
		memory_flag = 1;
	}
	else if (argc == 2 && isnumber(argv[1]) == 1) {
		sample_count = atoi(argv[1]);
		core_flag = 1;
		cpu_flag = 1;
		memory_flag = 1;
	}
	else if (argc == 3 && isnumber(argv[1]) == 1 && isnumber(argv[2]) == 1) {
		sample_count = atoi(argv[1]);
		tdelay = atoi(argv[2]);
		core_flag = 1;
		cpu_flag = 1;
		memory_flag = 1;
	}
	else {	
		if (weird_flag_checker(argc, argv) == 1) {
			printf("Weird flag found. Terminating...\n");
			return 1;
		}
		flag_checker(argc, argv, "--memory", &memory_flag);
		flag_checker(argc, argv, "--cpu", &cpu_flag);
		flag_checker(argc, argv, "--cores", &core_flag);
		check_sample_count = sample_count_checker(argc, argv, &sample_count);
		check_tdelay = tdelay_checker(argc, argv, &tdelay);
	}
	if (check_sample_count < 0 || check_tdelay < 0) {
		printf("Invalid value detected. Sample count and tdelay must be positive whole numbers.\n");
		return 0;
	}
	if (sample_count < 0 || tdelay < 0) {
		printf("Invalid value detected. Sample count and tdelay must be positive whole numbers.\n");
		return 0;
	}
	sample_count = min(sample_count, MAXIMUM_SAMPLE_SIZE);
	if (tdelay < MINIMUM_TIME_DELAY) {
		tdelay = MINIMUM_TIME_DELAY;
	}
	else if (tdelay > MAXIMUM_TIME_DELAY) {
		tdelay = MAXIMUM_TIME_DELAY;
	}
	// REFACTORING: Chay sleep va sample count o trong main luon. Trong moi ham chi can print thoi
	// Nhu vay se khong can ham duo_display nua
	if (memory_flag == 1) {
		memory_display(sample_count, tdelay);
	}
	if (core_flag == 1) {
		core_display();
	}
	return 0;
}
