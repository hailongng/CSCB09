#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<math.h>
#include<unistd.h>

#ifndef DEFAULT_SAMPLE_SIZE
#define DEFAULT_SAMPLE_SIZE 20
#endif


#ifndef MAXIMUM_SAMPLE_SIZE
#define MAXIMUM_SAMPLE_SIZE 80
#endif

// Unit of time is measured in microseconds

#ifndef DEFAULT_TIME_DELAY
#define DEFAULT_TIME_DELAY 500000
#endif

#ifndef MAXIMUM_TIME_DELAY
#define MAXIMUM_TIME_DELAY 500000
#endif

int min(int a, int b) {
	if (a <= b) {
		return a;
	}
	return b;
}

void grab_memory_info(char* flag, int* value) {
	char buff[128], storage[101][128];
	FILE* fp = fopen("/proc/meminfo", "r");
	if (fp == NULL) {
		printf("ERROR: Can't open file. Terminating...");
		return;
	}
	int i = 0;
	while ((fscanf(fp, "%s", buff) == 1) && (i <= 100)) {
		strcpy(storage[i], buff);
		i += 1;
	}
	fclose(fp);
	for (int i = 0; i <= 100; i++) {
		if (strstr(storage[i], flag) != NULL) {
			*value = atoi(storage[i + 1]);
		}
	}
}

void print_memory(char*** graph) {
	for (int i = 2; i <= 20; i++) {
		strcpy(graph[10][i], "_\0");
	}
	for (int i = 0; i <= 9; i++) {
		strcpy(graph[i][2], "|\0");
	}
	for (int i = 0; i <= 10; i++) {
		for (int j = 0; j <= 20; j++) {
			printf("%s", graph[i][j]);
			if (j == 0) {
				printf("\t");
			}
		}
		printf("\n");
	}
}

void memory_display(int sample_count, int tdelay) {
	char graph[12][85][10];
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 85; j++) {
			strcpy(graph[i][j], "\0");
		}
	}
	int active_memory_found = -1, total_memory_found = -1;
	grab_memory_info("MemTotal", &total_memory_found);
	int total_memory_in_gb = total_memory_found / 1048576;
	int active_memory_in_gb = 2 * active_memory_found / 1048576;
	strcpy(graph[10][0], "0 GB\0");
	sprintf(graph[0][0], "%d", total_memory_in_gb);	
	strcat(graph[0][0], " GB\0");
	for (int i = 2; i <= 20; i++) {
		strcpy(graph[10][i], "_\0");
	}
	for (int i = 0; i <= 10; i++) {
		strcpy(graph[i][2], "|\0");
	}
	for (int instance = 0; instance < sample_count; instance++) {
		printf("\033[2J");
		printf("\033[H");
		sleep(1);
		grab_memory_info("Active", &active_memory_found);
		float f_index_to_find = ((double)active_memory_found * 10.0 / (double)total_memory_found);
		int index_to_find = 10 - (int) f_index_to_find;
		printf("%d \t %f \t %d\n", active_memory_found, f_index_to_find, index_to_find);
		graph[index_to_find][instance + 3][0] = '#';
		graph[index_to_find][instance + 3][1] = '\0';
		// FACTOR THIS LATER
		for (int i = 0; i <= 10; i++) {
			for (int j = 0; j <= 20; j++) {
				printf("%s", graph[i][j]);
				if (j == 0) {
					printf("\t");
				}
			}
			printf("\n");
		}
		// printf("Total Memory: %d kB, Active Memory: %d kB\n", total_memory_found, active_memory_found);
	}
}

void cpu_display(int sample_count, int tdelay) {
	char buff[128];
	for (int instance = 0; instance < sample_count; instance++) {
		int i = 0;
		sleep(1);
		FILE* fp = fopen("/proc/stat", "r");
		if (fp == NULL) {
			printf("ERROR: Can't open file. Terminating...");
			return;
		}
		while ((fscanf(fp, "%s", buff) == 1) && (i < 8)) {
			printf("%s\n", buff);
			i++;
		}
		fclose(fp);
	}
}

void core_row_display(int cores_per_row) {
	for (int i = 0; i < cores_per_row; i++) {
		printf("+--+  ");
	} 
	printf("\n");
	for (int i = 0; i < cores_per_row; i++) {
		printf("|  |  ");
	}
	printf("\n");
	for (int i = 0; i < cores_per_row; i++) {
		printf("+--+  ");
	}
	printf("\n");
}

void core_display(int number_of_cores) {
	printf("Number of cores: %d\n", number_of_cores);
	while (number_of_cores >= 4) {
		core_row_display(4);
		number_of_cores -= 4;
	}
	if (number_of_cores > 0) {
		core_row_display(number_of_cores);
	}
}

void flag_checker(int argc, char** argv, char* str, int* flag) {
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], str) == 0) {
			*flag = 1;
		}
	}
}

int main (int argc, char** argv) {
	printf("\033[2J");
	printf("\033[H");
	int core_flag = 0, cpu_flag = 0, memory_flag = 0;
	int sample_count = DEFAULT_SAMPLE_SIZE;
	int tdelay = DEFAULT_TIME_DELAY;
	if (argc == 1) {
		core_flag = cpu_flag = memory_flag = 1; 																			
	}
	if (argc > 6) {
		return 0;
	}	
	flag_checker(argc, argv, "--memory", &memory_flag);
	flag_checker(argc, argv, "--cpu", &cpu_flag);
	flag_checker(argc, argv, "--cores", &core_flag);
	if (memory_flag == 0 && core_flag == 0 && cpu_flag == 0) {
		// something with positional arguments
	}
	printf("Number of arguments: %d \n", argc);
	printf("Memory flag: %d\t", memory_flag);
	printf("Core flag: %d\t", core_flag);
	printf("CPU flag: %d\t", cpu_flag);
	printf("\n");
	if (memory_flag == 1) {
		memory_display(5, tdelay);
	}
	/*
	if (cpu_flag == 1) {
		cpu_display(sample_count, tdelay);
	}
	*/
	if (core_flag == 1) {
		core_display(22);
	}
	return 0;
}
