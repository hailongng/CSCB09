#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/resource.h>
#include<sys/sysinfo.h>
#include<sys/types.h>
#include<math.h>
#include<unistd.h>

#ifndef DEFAULT_SAMPLE_SIZE
#define DEFAULT_SAMPLE_SIZE 20
#endif

#ifndef MAXIMUM_SAMPLE_SIZE
#define MAXIMUM_SAMPLE_SIZE 80
#endif

#ifndef DEFAULT_TIME_DELAY
#define DEFAULT_TIME_DELAY 500000
#endif

#ifndef MAXIMUM_TIME_DELAY
#define MAXIMUM_TIME_DELAY 500000
#endif

int sysinfo(struct sysinfo *info);
int get_nprocs_conf(void);

int min(int a, int b) {
	if (a <= b) {
		return a;
	}
	return b;
}

void grab_memory_info(double* freeram) {
	struct sysinfo* si = NULL;
	si = calloc(1, sizeof(struct sysinfo));
	int ret = sysinfo(si);
	if (ret == 0) {
		*freeram = si->freeram;
	}
	free(si);
}

void memory_display(int sample_count, int tdelay) {
	// Grab upper bound
	double total_ram = 0.0, ram_used = 0.0, freeram = 0.0;
	struct sysinfo* si = NULL;
	si = calloc(1, sizeof(struct sysinfo));
	int ret = sysinfo(si);
	if (ret == 0) {
		total_ram = (double)si->totalram;
	}
	free(si);
	char graph[12][85][10];
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 85; j++) {
			strcpy(graph[i][j], "\0");
		}
	}
	strcpy(graph[10][0], "0 GB\0");
	sprintf(graph[0][0], "%.2f", total_ram / 1073741824);	
	strcat(graph[0][0], " GB\0");
	for (int i = 2; i <= 80; i++) {
		strcpy(graph[10][i], "_\0");
	}
	for (int i = 0; i <= 10; i++) {
		strcpy(graph[i][1], "|\0");
	}
	for (int instance = 0; instance < sample_count; instance++) {
		printf("\033[2J");
		printf("\033[H");
		sleep(1.5);
		grab_memory_info(&freeram);
		ram_used = total_ram - freeram;
		double f_index_to_find = (ram_used * 10.0 / total_ram);
		int index_to_find = 10 - round(f_index_to_find);
		graph[index_to_find][instance + 2][0] = '#';
		graph[index_to_find][instance + 2][1] = '\0';
		// FACTOR THIS LATER
		printf("RAM used: %.4f GB\n", ram_used / 1073741824);
		for (int i = 0; i <= 10; i++) {
			for (int j = 0; j <= 22; j++) {
				printf("%s", graph[i][j]);
				if (j == 0) {
					printf("\t");
				}
			}
			printf("\n");
		}
	}
}

void grab_cpu_info(int* cpudata) {
	char buff[128];
	FILE* fp = fopen("/proc/stat", "r");
	if (fp == NULL) {
		printf("ERROR: Can't open file. Terminating...");
		return;
	}
	if ((fscanf(fp, "%s", buff) == 1) && (strcmp(buff, "cpu") == 0)) {
		for (int i = 0; i <= 6; i++) {
			fscanf(fp, "%d", &cpudata[i]);
		}
	}
	fclose(fp);
}

void cpu_display(int sample_count, int tdelay)  {
	char graph[12][85][10];
	int cpu_data[7];
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 85; j++) {
			strcpy(graph[i][j], "\0");
		}
	}
	strcpy(graph[10][0], "0\%\0");
	strcpy(graph[0][0], "100\%\0");
	for (int i = 1; i <= 80; i++) {
		strcpy(graph[10][i], "_\0");
	}
	for (int i = 0; i <= 10; i++) {
		strcpy(graph[i][1], "|\0");
	}
	int total_cpu_util_time_1 = 0, total_cpu_util_time_2 = 0, total_cpu_time_1 = 0, total_cpu_time_2 = 0;
	grab_cpu_info(cpu_data);
	for (int i = 0; i <= 6; i++) {
		total_cpu_time_1 += cpu_data[i];
	}
	total_cpu_util_time_1 = total_cpu_time_1 - cpu_data[3];
	for (int instance = 2; instance < sample_count + 2; ++instance) {
		printf("\033[2J");
		printf("\033[H");
		total_cpu_time_2 = 0;
		sleep(1.5);
		grab_cpu_info(cpu_data);
		for (int i = 0; i <= 6; i++) {
			total_cpu_time_2 += cpu_data[i];
		}
		total_cpu_util_time_2 = total_cpu_time_2 - cpu_data[3];
		double ratio = ((double)10.0 * (total_cpu_util_time_2 - total_cpu_util_time_1)) / 
						((double)(total_cpu_time_2 - total_cpu_time_1));
		int index_to_find = 10 - (int)round(ratio);
		printf("%.4f%% \n", 10 * ratio);
		graph[index_to_find][instance][0] = ':';
		graph[index_to_find][instance][1] = '\0';
		for (int i = 0; i <= 10; i++) {
			for (int j = 0; j <= 22; j++) {
				printf("%s", graph[i][j]);
				if (j == 0) {
					printf("\t");
				}
			}
			printf("\n");
		}
		total_cpu_time_1 = total_cpu_time_2;
		total_cpu_util_time_1 = total_cpu_util_time_2;
	}
}

void duo_display(int sample_count, int tdelay) {
	double total_ram = 0.0, ram_used = 0.0, freeram = 0.0;
	struct sysinfo* si = NULL;
	si = calloc(1, sizeof(struct sysinfo));
	int ret = sysinfo(si);
	if (ret == 0) {
		total_ram = (double)si->totalram;
	}
	free(si);
	int cpu_data[7];
	char memory_graph[12][85][10], cpu_graph[12][85][10];
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 85; j++) {
			strcpy(memory_graph[i][j], "\0");
			strcpy(cpu_graph[i][j], "\0");
		}
	}
	strcpy(memory_graph[10][0], "0 GB\0");
	sprintf(memory_graph[0][0], "%.2f", total_ram / 1073741824);	
	strcat(memory_graph[0][0], " GB\0");
	strcpy(cpu_graph[10][0], "0\%\0");
	strcpy(cpu_graph[0][0], "100\%\0");
	for (int i = 1; i <= 80; i++) {
		strcpy(memory_graph[10][i], "_\0");
		strcpy(cpu_graph[10][i], "_\0");
	}
	for (int i = 0; i <= 10; i++) {
		strcpy(memory_graph[i][1], "|\0");
		strcpy(cpu_graph[i][1], "|\0");
	}
	int total_cpu_util_time_1 = 0, total_cpu_util_time_2 = 0, total_cpu_time_1 = 0, total_cpu_time_2 = 0;
	grab_cpu_info(cpu_data);
	for (int i = 0; i <= 6; i++) {
		total_cpu_time_1 += cpu_data[i];
	}
	total_cpu_util_time_1 = total_cpu_time_1 - cpu_data[3];
	for (int instance = 2; instance < sample_count + 2; instance++) {
		printf("\033[2J");
		printf("\033[H");
		sleep(1.5);
		// STEP 1: MEMORY
		grab_memory_info(&freeram);
		ram_used = total_ram - freeram;
		double f_memory_index_to_find = (ram_used * 10.0 / total_ram);
		int memory_index_to_find = 10 - round(f_memory_index_to_find);
		memory_graph[memory_index_to_find][instance][0] = '#';
		memory_graph[memory_index_to_find][instance][1] = '\0';
		// STEP 2: CPU
		total_cpu_time_2 = 0;
		grab_cpu_info(cpu_data);
		for (int i = 0; i <= 6; i++) {
			total_cpu_time_2 += cpu_data[i];
		}
		total_cpu_util_time_2 = total_cpu_time_2 - cpu_data[3];
		double ratio = ((double)10.0 * (total_cpu_util_time_2 - total_cpu_util_time_1)) / 
						((double)(total_cpu_time_2 - total_cpu_time_1));
		int cpu_index_to_find = 10 - (int)round(ratio);
		cpu_graph[cpu_index_to_find][instance][0] = ':';
		cpu_graph[cpu_index_to_find][instance][1] = '\0';
		// STEP 3: PRINT
		printf("RAM used: %.4f GB\n", ram_used / 1073741824);
		for (int i = 0; i <= 10; i++) {
			for (int j = 0; j <= 22; j++) {
				printf("%s", memory_graph[i][j]);
				if (j == 0) {
					printf("\t");
				}
			}
			printf("\n");
		}
		printf("CPU Usage: %.4f%% \n", 10 * ratio);
		for (int i = 0; i <= 10; i++) {
			for (int j = 0; j <= 22; j++) {
				printf("%s", cpu_graph[i][j]);
				if (j == 0) {
					printf("\t");
				}
			}
			printf("\n");
		}
		// Remember to reassign the value for future use
		total_cpu_time_1 = total_cpu_time_2;
		total_cpu_util_time_1 = total_cpu_util_time_2;
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

void core_display() {
	int number_of_cores = get_nprocs_conf();
	FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (!cpuinfo) {
        perror("Failed to open /proc/cpuinfo");
        return;
    }
    char line[256];
    double max_frequency = 0.0, curr_freq;
    while (fgets(line, sizeof(line), cpuinfo)) {
        if (strncmp(line, "cpu MHz", 7) == 0) {
            if (sscanf(line, "cpu MHz\t: %lf", &curr_freq) == 1) {
                if (curr_freq > max_frequency) {
					max_frequency = curr_freq;
				}
            }
        }
	}
	max_frequency /= 1000;
	printf("Number of cores: %d @ %.2f GHz\n", number_of_cores, max_frequency);
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

int tdelay_checker(int argc, char** argv, int* flag) {
	int val = 0;
	for (int i = 0; i < argc; i++) {
		if (strstr(argv[i], "--tdelay") != NULL) {
			sscanf(argv[i], "--tdelay=%d", &val);
		}
	}
	return val;
}

int sample_count_checker(int argc, char** argv, int* flag) {
	int val = 0;
	for (int i = 0; i < argc; i++) {
		if (strstr(argv[i], "--samples") != NULL) {
			sscanf(argv[i], "--samples=%d", &val);
		}
	}
	return val;
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
		if (argc == 2) {
			sample_count = atoi(argv[1]);
		}
		else {
			sample_count = atoi(argv[1]);
			tdelay = atoi(argv[2]);
		}
	}
	// cpu_display(4, tdelay);
	// memory_display(20, tdelay);
	// duo_display(6, tdelay);
	core_display(14);
	return 0;
}
