#include "info_fetcher.h"

void grab_memory_info(double* freeram) {
	struct sysinfo* si = NULL;
	si = calloc(1, sizeof(struct sysinfo));
	int ret = sysinfo(si);
	if (ret == 0) {
		*freeram = si->freeram;
	}
	free(si);
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

void core_display() {
	
	// Fetch information for the cores

	int number_of_cores = get_nprocs_conf();
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
	
	// Display the information for the cores
	
	printf("Number of cores: %d @ %.2f GHz\n", number_of_cores, max_frequency);
	while (number_of_cores >= 4) {
		core_row_display(4);
		number_of_cores -= 4;
	}
	if (number_of_cores > 0) {
		core_row_display(number_of_cores);
	}
}