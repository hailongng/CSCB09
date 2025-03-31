#include "info_fetcher.h"

double grab_memory_info() {
	int p[2];
	if (pipe(p) == -1) {
		perror("PIPE ERROR. Terminating...\n");
		exit(1);
	}
	int result = fork();
	if (result < 0) {
		perror("FORK ERROR. Terminating...\n");
		exit(1);
	}
	else if (result == 0) {
		// Close the reading port for children
		close(p[0]);
		long long free_ram = 0;
		struct sysinfo* si = calloc(1, sizeof(struct sysinfo));
		int ret = sysinfo(si);
		if (ret == 0) {
			free_ram = (long long) si->freeram;
		}
		free(si);
		write(p[1], &free_ram, sizeof(long long));
		close(p[1]);
		exit(0);
	}
	else {
		// Close the writing port for parent
		close(p[1]);
	}
	long long free_ram_in_byte = 0;
	read(p[0], &free_ram_in_byte, sizeof(long long));
	return (double)free_ram_in_byte;
}

long long fetch_total_ram() {
	int p[2];
	if (pipe(p) == -1) {
		perror("PIPE ERROR. Terminating...\n");
		exit(1);
	}
	int result = fork();
	if (result < 0) {
		perror("FORK ERROR. Terminating...\n");
		exit(1);
	}
	else if (result == 0) {
		// Close the reading port for children
		close(p[0]);
		long long total_ram = 0;
		struct sysinfo* si = calloc(1, sizeof(struct sysinfo));
		int ret = sysinfo(si);
		if (ret == 0) {
			total_ram = (long long) si->totalram;
		}
		free(si);
		write(p[1], &total_ram, sizeof(long long));
		close(p[1]);
		exit(0);
	}
	else {
		// Close the writing port for parent
		close(p[1]);
	}
	long long total_ram_in_byte = 0;
	read(p[0], &total_ram_in_byte, sizeof(long long));
	return total_ram_in_byte;
}

void grab_cpu_info(int* cpudata) {
	/*
	int p[7][2], i;
	for (i = 0; i <= 6; i++) {
		if (pipe(p[i]) == -1) {
			perror("PIPE ERROR. Terminating...\n");
			exit(1);
		}
		int result = fork();
		if (result < 0) {
			perror("FORK ERROR. Terminating...\n");
			exit(1);
		}
		else if (result == 0) {
			close(p[i][0]);
			for (int j = 0; j < i; j++) {
				close(p[j][0]);
			}
			char buff[128];
			int num;
			FILE* fp = fopen("/proc/stat", "r");
			if (fp == NULL) {
				printf("ERROR: Can't open file. Terminating...");
				return;
			}
			if ((fscanf(fp, "%s", buff) == 1) && (strcmp(buff, "cpu") == 0)) {
				for (i = 0; i <= 6; i++) {
					fscanf(fp, "%d", &num);
				}
			}
			fclose(fp);
			write(p[i][1], &num, sizeof(int));
			close(p[i][1]);
			exit(0);
		}
		else {
			close(p[i][1]);
		}
	} 
	for (int i = 0; i <= 6; i++) {
		read(p[i][0], cpuinfo[i], sizeof(int));
	}
	*/
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
	
	// Fetch information for the cores

	int p[2][2];
	for (int i = 0; i <= 1; i++) {
		if (pipe(p[i]) == -1) {
			perror("PIPE ERROR. Terminating...\n");
			exit(1);
		}
		int result = fork();
		if (result < 0) {
			perror("FORK ERROR. Terminating...\n");
			exit(1);
		}
		else if (result == 0) {
			// Close the reading port
			close(p[i][0]);
			// Close the reading port for the prior child
			for (int j = 0; j < i; j++) {
				close(p[j][0]);
			}
			if (i == 0) {
				// Write the number of cores in p[0][1]
				int number_of_cores = get_nprocs_conf();
				write(p[0][1], &number_of_cores, sizeof(int));
				close(p[0][1]);
				exit(0);
			}
			else {
				FILE *cpuinfo = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");
				if (!cpuinfo) {
					perror("Failed to open /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
					exit(1);
				}
				char line[256];
				int max_frequency = 0;
				while (fgets(line, sizeof(line), cpuinfo)) {
					max_frequency = atoi(line);
				}
				fclose(cpuinfo);
				// Write the maximum frequency p[1][1]
				write(p[1][1], &max_frequency, sizeof(int));
				exit(0);
			}
		}
		else {
			close(p[i][1]);
		}
	}
	int res1, temp_res2;
	double res2;
	read(p[0][0], &res1, sizeof(int));
	read(p[1][0], &temp_res2, sizeof(int));
	res2 = ((double)temp_res2) / 1000000;
	
	// Display the information for the cores
	
	printf("Number of cores: %d @ %.2f GHz\n", res1, res2);
	while (res1 >= 4) {
		core_row_display(4);
		res1 -= 4;
	}
	if (res1 > 0) {
		core_row_display(res1);
	}
}