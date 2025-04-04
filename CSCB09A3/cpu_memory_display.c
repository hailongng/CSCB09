#include "cpu_memory_display.h"

void basic_info_print(int sample_count, int tdelay) {
    double tdelay_in_sec = (double)tdelay / 1000000;
	printf("\033[2J");
	printf("\033[H");
	printf("\n\tNumber of samples: %d -- every %d ms (%.3f secs)\n\n", sample_count, tdelay, tdelay_in_sec);
}

void print_memory(char graph[13][85], int sample_count, int tdelay, double ram_used, double total_ram_in_gb) {
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

void print_cpu(char graph[13][85], int sample_count, int tdelay, double ratio) {
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
}