#define _DEFAULT_SOURCE

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

#ifndef MINIMUM_TIME_DELAY
#define MINIMUM_TIME_DELAY 100000
#endif

int sysinfo(struct sysinfo *info);
int get_nprocs_conf(void);

int min(int a, int b) {
	if (a <= b) {
		return a;
	}
	return b;
}

int max(int a, int b) {
	if (a >= b) {
		return a;
	}
	return b;
}

int find_index(double value) {
	if ((value >= 0) && (value < 10)) {
		return 9;
	}
	else if (value < 20) {
		return 8;
	}
	else if (value < 30) {
		return 7;
	}
	else if (value < 40) {
		return 6;
	}
	else if (value < 50) {
		return 5;
	}
	else if (value < 60) {
		return 4;
	}
	else if (value < 70) {
		return 3;
	}
	else if (value < 80) {
		return 2;
	}
	else if (value < 90) {
		return 1;
	}
	return 0;
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
	struct sysinfo* si = NULL;
	si = calloc(1, sizeof(struct sysinfo));
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
		// FACTOR THIS LATER
		printf("\t* RAM used: %.4f GB\n", ram_used / 1073741824);
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
		printf("CPU Usage: %f %% \t Index: %d\n", ratio, index_to_find);
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
		// FACTOR THIS LATER
		printf("\t* RAM used: %.4f GB\n", ram_used / 1073741824);
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
	FILE *cpuinfo = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");
    if (!cpuinfo) {
        perror("Failed to open /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
        return;
    }
    char line[256];
    double max_frequency = 0.0, curr_freq;
    while (fgets(line, sizeof(line), cpuinfo)) {
	    max_frequency = atoi(line);
    }
	max_frequency /= 1000000;
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

void tdelay_checker(int argc, char** argv, int* val) {
	for (int i = 0; i < argc; i++) {
		if (strstr(argv[i], "--tdelay") != NULL) {
			sscanf(argv[i], "--tdelay=%d", &(*val));
		}
	}
}

int sample_count_checker(int argc, char** argv, int* val) {
	for (int i = 0; i < argc; i++) {
		if (strstr(argv[i], "--samples") != NULL) {
			sscanf(argv[i], "--samples=%d", &(*val));
		}
	}
}

int main (int argc, char** argv) {
	int core_flag = 0, cpu_flag = 0, memory_flag = 0;
	int sample_count = DEFAULT_SAMPLE_SIZE;
	int tdelay = DEFAULT_TIME_DELAY;
	if (argc > 6) {
		return 0;
	}
	if (argc == 1) {
		core_flag = 1;
		cpu_flag = 1;
		memory_flag = 1; 													}
	else {	
		flag_checker(argc, argv, "--memory", &memory_flag);
		flag_checker(argc, argv, "--cpu", &cpu_flag);
		flag_checker(argc, argv, "--cores", &core_flag);
		sample_count_checker(argc, argv, &sample_count);
		tdelay_checker(argc, argv, &tdelay);
		if (memory_flag == 0 && core_flag == 0 && cpu_flag == 0) {
			if (argc == 2) {
				sample_count = atoi(argv[1]);
			}
			else {
				sample_count = atoi(argv[1]);
				tdelay = atoi(argv[2]);
			}
			core_flag = 1;
			cpu_flag = 1;
			memory_flag = 1;
		}
	}
	printf("%d %d\n", sample_count, tdelay);
	if (sample_count <= 0) {
		printf("Invalid value. Sample count must be positive.\n");
		return 0;
	}
	if (tdelay <= 0) {
		printf("Invalid value. Time delay must be positive.\n");
		return 0;
	}
	if (memory_flag == 1 && cpu_flag == 1) {
		duo_display(min(sample_count, MAXIMUM_SAMPLE_SIZE), max(tdelay, MINIMUM_TIME_DELAY));
	}
	else if (memory_flag == 1) {
		memory_display(min(sample_count, MAXIMUM_SAMPLE_SIZE), max(tdelay, MINIMUM_TIME_DELAY));
	}
	else if (cpu_flag == 1) {
		cpu_display(min(sample_count, MAXIMUM_SAMPLE_SIZE), max(tdelay, MINIMUM_TIME_DELAY));
	}
	if (core_flag == 1) {
		core_display();
	}
	return 0;
}
