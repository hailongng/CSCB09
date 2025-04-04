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
#include<sys/wait.h>
#include "cpu_memory_display.h"
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

void sth(int p[2]) {
    int pid = fork();
    char buff[128], data[128] = {0}, temp[64];
    int val;
    if (pid == 0) {
        close(p[0]);
        FILE* fp = fopen("/proc/stat", "r");
        if (fp && (fscanf(fp, "%s", buff) == 1) && (strcmp(buff, "cpu") == 0)) {
            for (int i = 0; i <= 6; i++) {
                fscanf(fp, "%d", &val);
                sprintf(temp, "%d ", val);
                strcat(data, temp);
            }
            fclose(fp);
        }
        write(p[1], data, strlen(data) + 1);
        close(p[1]);
        exit(0);
    }
    close(p[1]);
}

long long fetch_total_ram_with_pipe(int pipe[2]) {
	int pid = fork();
	if (pid < 0) {
		exit(1);
	}
	else if (pid == 0) {
		close(pipe[0]);
		long long total_ram = 0;
		struct sysinfo* si = calloc(1, sizeof(struct sysinfo));
		int ret = sysinfo(si);
		if (ret == 0) {
			total_ram = (long long) si->totalram;
		}
		free(si);
		write(pipe[1], &total_ram, sizeof(long long));
		close(pipe[1]);
		exit(0);
	}
	return 0;
}

long long fetch_free_ram_with_pipe(int pipe[2]) {
	// Because I am reusing the same pipe, I will clean out the pipe first
	int pid = fork();
	if (pid < 0) {
		exit(1);
	}
	else if (pid == 0) {
		close(pipe[0]);
		long long free_ram = grab_memory_info();
		write(pipe[1], &free_ram, sizeof(long long));
		close(pipe[1]);
		exit(0);
	}
	return 0;
}

void memory_display(int sample_count, int tdelay, int p[2], long long total_ram_in_byte) {
	char graph[13][85];
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 85; j++) {
			graph[i][j] = ' ';
		}
	}
	for (int j = 0; j <= sample_count; j++) {
		graph[9][j] = '_';
	}
	long long temp = 0;
	double ram_used = 0.0, freeram = 0.0;
	double total_ram_in_gb = (double)total_ram_in_byte / 1073741824;
	for (int instance = 0; instance < sample_count; instance++) {
		usleep(tdelay);
		//freeram = grab_memory_info();										// Note: ram_used (byte)
		close(p[0]);
		close(p[1]);
		if (pipe(p) == -1) {
			exit(1);
		}
		fetch_free_ram_with_pipe(p);
		wait(NULL);
		read(p[0], &temp, sizeof(long long));
		freeram = (double) temp;
		ram_used = total_ram_in_byte - freeram;
		double f_index_to_find = ram_used * 100.0 / total_ram_in_byte;
		int index_to_find = find_index(f_index_to_find);
		graph[index_to_find][instance] = '#';
		basic_info_print(sample_count, tdelay);
		print_memory(graph, sample_count, tdelay, ram_used, total_ram_in_gb);
	}
}

void cpu_display(int sample_count, int tdelay, int p[2])  {
	char graph[13][85], rawdata[128];
	int cpu_data[7], nbytes;
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 85; j++) {
			graph[i][j] = ' ';
		}
	}
	for (int j = 0; j <= sample_count; j++) {
		graph[9][j] = '_';
	}
	int total_cpu_util_time_1 = 0, total_cpu_util_time_2 = 0, total_cpu_time_1 = 0, total_cpu_time_2 = 0;
	if (pipe(p) == -1) {
		perror("No way out\n");
		exit(1);
	}
	sth(p);
	wait(NULL);
	nbytes = read(p[0], rawdata, sizeof(rawdata) - 1);
	if (nbytes > 0) {
		rawdata[nbytes] = '\0'; // Ensure null termination
		sscanf(rawdata, "%d %d %d %d %d %d %d", &cpu_data[0],&cpu_data[1],&cpu_data[2],
				&cpu_data[3],&cpu_data[4],&cpu_data[5],&cpu_data[6]);
	}
	// Need to have comment on the method
	for (int i = 0; i <= 6; i++) {
		total_cpu_time_1 += cpu_data[i];
	}
	total_cpu_util_time_1 = total_cpu_time_1 - cpu_data[3];
	for (int instance = 0; instance < sample_count; instance++) {
		usleep(tdelay);
		close(p[0]); close(p[1]);
		if (pipe(p) == -1) {
			perror("No way out\n");
			exit(1);
		}
		sth(p);
		wait(NULL);
		nbytes = read(p[0], rawdata, sizeof(rawdata) - 1);
		if (nbytes > 0) {
			rawdata[nbytes] = '\0'; // Ensure null termination
			sscanf(rawdata, "%d %d %d %d %d %d %d", &cpu_data[0],&cpu_data[1],&cpu_data[2],
					&cpu_data[3],&cpu_data[4],&cpu_data[5],&cpu_data[6]);
		} 
		total_cpu_time_2 = 0;
		for (int i = 0; i <= 6; i++) {
			total_cpu_time_2 += cpu_data[i];
		}
		total_cpu_util_time_2 = total_cpu_time_2 - cpu_data[3];
		double ratio = ((double)100.0 * (total_cpu_util_time_2 - total_cpu_util_time_1)) / 
						((double)(total_cpu_time_2 - total_cpu_time_1));
		int index_to_find = find_index(ratio);
		graph[index_to_find][instance] = ':';
		basic_info_print(sample_count, tdelay);
		print_cpu(graph, sample_count, tdelay, ratio);
		total_cpu_time_1 = total_cpu_time_2;
		total_cpu_util_time_1 = total_cpu_util_time_2;
	}
}

void duo_display(int sample_count, int tdelay, int memory_pipe[2], int cpu_pipe[2], long long total_ram_in_byte) {
	char memory_graph[13][85], cpu_graph[13][85], rawdata[128];
    int cpu_data[7], nbytes;
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
	double ram_used = 0.0, freeram = 0.0;
	long long temp;
	double total_ram_in_gb = (double)total_ram_in_byte / 1073741824;
	int total_cpu_util_time_1 = 0, total_cpu_util_time_2 = 0, total_cpu_time_1 = 0, total_cpu_time_2 = 0;
	sth(cpu_pipe);
	wait(NULL);
	nbytes = read(cpu_pipe[0], rawdata, sizeof(rawdata) - 1);
	if (nbytes > 0) {
		rawdata[nbytes] = '\0'; // Ensure null termination
		sscanf(rawdata, "%d %d %d %d %d %d %d", &cpu_data[0],&cpu_data[1],&cpu_data[2],
				&cpu_data[3],&cpu_data[4],&cpu_data[5],&cpu_data[6]);
	}
	for (int i = 0; i <= 6; i++) {
		total_cpu_time_1 += cpu_data[i];
	}
	total_cpu_util_time_1 = total_cpu_time_1 - cpu_data[3];
	
	for (int instance = 0; instance < sample_count; instance++) {
		usleep(tdelay);
		// STEP 0: PIPING
		close(memory_pipe[0]);
		close(memory_pipe[1]);
		close(cpu_pipe[0]);
		close(cpu_pipe[1]);
		if (pipe(memory_pipe) == -1 || pipe(cpu_pipe) == -1) {
			exit(1);
		}
		fetch_free_ram_with_pipe(memory_pipe);
		sth(cpu_pipe);
		wait(NULL);
		// STEP 1: MEMORY
		read(memory_pipe[0], &temp, sizeof(long long));
		freeram = (double) temp;
		ram_used = total_ram_in_byte - freeram;
		double f_index_to_find = ram_used * 100.0 / total_ram_in_byte;
		int index_to_find = find_index(f_index_to_find);
		memory_graph[index_to_find][instance] = '#';
		// STEP 2: CPU
		nbytes = read(cpu_pipe[0], rawdata, sizeof(rawdata) - 1);
		if (nbytes > 0) {
			rawdata[nbytes] = '\0'; // Ensure null termination
			sscanf(rawdata, "%d %d %d %d %d %d %d", &cpu_data[0],&cpu_data[1],&cpu_data[2],
					&cpu_data[3],&cpu_data[4],&cpu_data[5],&cpu_data[6]);
		}
		total_cpu_time_2 = 0;
		for (int i = 0; i <= 6; i++) {
			total_cpu_time_2 += cpu_data[i];
		}
		total_cpu_util_time_2 = total_cpu_time_2 - cpu_data[3];
		double ratio = ((double)100.0 * (total_cpu_util_time_2 - total_cpu_util_time_1)) / 
						((double)(total_cpu_time_2 - total_cpu_time_1));
		int cpu_index_to_find = find_index(ratio);
		cpu_graph[cpu_index_to_find][instance] = ':';
		basic_info_print(sample_count, tdelay);
		print_memory(memory_graph, sample_count, tdelay, ram_used, total_ram_in_gb);
		printf("\n");
		print_cpu(cpu_graph, sample_count, tdelay, ratio);
        total_cpu_time_1 = total_cpu_time_2;
		total_cpu_util_time_1 = total_cpu_util_time_2;
	}
}

int main(int argc, char** argv) {
	
	// Establishing pipes

	int cpu_pipe[2];
	int memory_pipe[2];
	int core_pipe[2];

	if (pipe(cpu_pipe) == -1) {
		exit(1);
	}
	if (pipe(memory_pipe) == -1) {
		exit(1);
	}
	if (pipe(core_pipe) == -1) {
		exit(1);
	}
	
	// Establish signals
	if (signal(SIGTSTP, handler_ctrl_z) == SIG_ERR) {
		perror("Failed to register signal handler for Ctrl Z\n");
		return 1;
	}
	if (signal(SIGINT, handler_ctrl_c) == SIG_ERR) {
		perror("Failed to register signal handler for Ctrl C\n");
		return 1;
	}

	// Handle CLA
	int core_flag = 0, cpu_flag = 0, memory_flag = 0;
	int sample_count = DEFAULT_SAMPLE_SIZE;
	int tdelay = DEFAULT_TIME_DELAY;
	int check_sample_count = 0, check_tdelay = 0;
	if (argc > MAXIMUM_CLA) {
		printf("Too much argument. The program will terminate.\n");
		return 1;
	}
	if (argc == 1) {
		core_flag = 1;
		cpu_flag = 1;
		memory_flag = 1;
	}
	else if (argc == 2 && (isnumber(argv[1]) == 1)) {
		sample_count = atoi(argv[1]);
		core_flag = 1;
		cpu_flag = 1;
		memory_flag = 1;
	}
	else if (argc == 3 && (isnumber(argv[1]) == 1) && (isnumber(argv[2]) == 1)) {
		sample_count = atoi(argv[1]);
		tdelay = atoi(argv[2]);
		core_flag = 1;
		cpu_flag = 1;
		memory_flag = 1;
	}
	else if (argc == 3 && sample_count_checker(argc, argv, &sample_count) >= 0 && tdelay_checker(argc, argv, &tdelay) >= 0) {
		check_sample_count = sample_count_checker(argc, argv, &sample_count);
		check_tdelay = tdelay_checker(argc, argv, &tdelay);
		core_flag = 1;
		cpu_flag = 1;
		memory_flag = 1;
	}
	else {	
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

	fetch_total_ram_with_pipe(memory_pipe);
	long long total_ram_in_byte;
	read(memory_pipe[0], &total_ram_in_byte, sizeof(long long));
	wait(NULL);

	
	if (memory_flag == 1 && cpu_flag == 1) {
		write(memory_pipe[1], &total_ram_in_byte, sizeof(long long));
		duo_display(sample_count, tdelay, memory_pipe, cpu_pipe, total_ram_in_byte);
	}
	else if (memory_flag == 1) {
		write(memory_pipe[1], &total_ram_in_byte, sizeof(long long));
		memory_display(sample_count, tdelay, cpu_pipe, total_ram_in_byte);
	}
	else if (cpu_flag == 1) {
		cpu_display(sample_count, tdelay, cpu_pipe);
	}
	
	if (core_flag == 1) {
		core_fetcher(sample_count, tdelay, core_pipe);
	}

	close(cpu_pipe[0]); close(cpu_pipe[1]);
	close(memory_pipe[0]); close(memory_pipe[1]);
	close(core_pipe[0]); close(core_pipe[1]);
	return 0;
}
