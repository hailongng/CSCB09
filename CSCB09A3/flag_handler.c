#include "flag_handler.h"

int isnumber(char* str) {
	for (int i = 0; str[i] != '\0'; i++) {
		if (!isdigit(str[i])) {
			return 0;
		}
	}
	return 1;
}

void flag_checker(int argc, char** argv, char* str, int* flag) {
	// Helper function to check what information to display
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], str) == 0) {
			*flag = 1;
		}
	}
}

int tdelay_checker(int argc, char** argv, int* val) {
	char input[12];
	for (int i = 0; i < argc; i++) {
		if (strstr(argv[i], "--tdelay") != NULL) {
			sscanf(argv[i], "--tdelay=%s", input);
			if (isnumber(input) == 0 || strstr(input, ".") != NULL) {
				return -1;
			}
			else {
				sscanf(argv[i], "--tdelay=%d", &(*val));
			}
		}
	}
	return 0;
}

int sample_count_checker(int argc, char** argv, int* val) {
	char input[12];
	for (int i = 0; i < argc; i++) {
		if (strstr(argv[i], "--samples") != NULL) {
			sscanf(argv[i], "--samples=%s", input);
			if (isnumber(input) == 0 || strstr(input, ".") != NULL) {
				return -1;
			}
			else {
				sscanf(argv[i], "--samples=%d", &(*val));
			}
		}
	}
	return 0;
}

int _weird_flag_checker(char* str) {
	char* flags[5] = {"--cores", "--memory", "--cpu", "--samples", "--tdelay"};
	for (int j = 0; j <= 2; j++) {
		if (strcmp(str, flags[j]) == 0) {
			return 0;
		}
	}
	for (int j = 3; j <= 4; j++) {
		if (strstr(str, flags[j]) != NULL) {
			return 0;
		}
	}
	return 1;
}

int weird_flag_checker(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		if (_weird_flag_checker(argv[i]) == 1) {
			return 1;
		}
	}
	return 0;
}


int duplicate_flag_checker(int argc, char** argv) {
	char* flags[5] = {"--cores", "--memory", "--cpu", "--samples", "--tdelay"};
	int counter[5] = {0, 0, 0, 0, 0};
	for (int i = 1; i < argc; i++) {
		for (int j = 0; j <= 2; j++) {
			if (strcmp(argv[i], flags[j]) == 0) {
				counter[j] += 1;
			}
		}
		for (int j = 3; j <= 4; j++) {
			if (strstr(argv[i], flags[j]) != NULL) {
				counter[j] += 1;
			}
		}
	}
	for (int i = 0; i <= 4; i++) {
		if (counter[i] > 1) {
			return 1;
		}
	}
	return 0;
}