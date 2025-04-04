#include "info_fetcher.h"

double grab_memory_info() {
	long long freeram = 0;
	struct sysinfo* si = calloc(1, sizeof(struct sysinfo));
	int ret = sysinfo(si);
	if (ret == 0) {
		freeram = si->freeram;
	}
	free(si);
	return (double) freeram;
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

void core_fetcher(int sample_count, int tdelay, int p[2]) {
	int child_pid, grandchild_pid;
	int number_of_cores;
	double max_frequency;

	child_pid = fork();
	if (child_pid == -1) {
		perror("Fork failed\n");
		close(p[0]);
		close(p[1]);
		exit(0);
	}
	else if (child_pid == 0) {
		close(p[0]);
		number_of_cores = get_nprocs_conf();
		grandchild_pid = fork();
		if (grandchild_pid == -1) {
			perror("Fork failed\n");
			close(p[1]);
			exit(EXIT_FAILURE);
		}
		else if (grandchild_pid == 0) {
			FILE *cpuinfo = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");
			if (!cpuinfo) {
				perror("Failed to open /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
				return;
			}
			char line[256];
			double max_frequency = 0.0;
			while (fgets(line, sizeof(line), cpuinfo)) {
				max_frequency = atoi(line);
			}
			max_frequency /= 1000000;
			fclose(cpuinfo);
			write(p[1], &max_frequency, sizeof(double));
			close(p[1]);
			exit(EXIT_SUCCESS);
		}
		wait(NULL);
		write(p[1], &number_of_cores, sizeof(int));
		close(p[1]);
		exit(EXIT_SUCCESS);
	}
	else {
		// Parent process
		close(p[1]);
		wait(NULL);
		read(p[0], &max_frequency, sizeof(double));
		read(p[0], &number_of_cores, sizeof(int));
		core_display(sample_count, tdelay, number_of_cores, max_frequency);
		close(p[0]);
	}
}

void core_display(int sample_count, int tdelay, int number_of_cores, double max_frequency) {
	printf("Number of cores: %d @ %.2f GHz\n", number_of_cores, max_frequency);
	while (number_of_cores >= 4) {
		core_row_display(4);
		number_of_cores -= 4;
	}
	if (number_of_cores > 0) {
		core_row_display(number_of_cores);
	}
}