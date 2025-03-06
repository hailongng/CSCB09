#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include"file_proc.h"
#include"linked_list.h"
#include"composite.h"
#include"vnodes.h"
#include"sys_wide.h"
#include"per_process.h"
#include"summary.h"
#include"threshold.h"

bool isnum(char* str) {
	for (int i = 0; str[i] != '\0'; i++) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

void flag_checker(int argc, char** argv, char* str, int* flag) {
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], str) == 0) {
			*flag = 1;
		}
	}
}

void get_threshold_val(int argc, char** argv, int* threshold_value) {
	char str[20];
	for (int i = 1; i < argc; i++) {
		if (strstr(argv[i], "--threshold") != NULL) {
			sscanf(argv[i], "--threshold=%s", str);
			if (isnum(str)) {
				*threshold_value = atoi(str);
			}
		}
	}
}

int main(int argc, char** argv) {
	ll2d_t* file_list = all_files_handling();
	int per_proc_flag = 0, sysWide_flag = 0, Vnode_flag = 0, composite_flag = 0, summary_flag = 0;
	int threshold_value = -1;
	int text_write = 0, binary_write = 0;
	pid_t pid = -1;
	// need threshold as well
	if (argc > 10) {
		perror("Invalid CLA\n");
		return 1;
	}
	if (argc == 1) {
		// Display composite table: my decision
		display_composite(file_list, pid);
		return 0;
	}
	if ((argc == 2) && (isnum(argv[1]))) {
		pid = atoi(argv[1]);
		printf("%d\n", pid);
		display_process(file_list, pid);
		display_system_wide(file_list, pid);
		display_vnodes(file_list, pid);
		display_composite(file_list, pid);
		return 0;
	}
	if (isnum(argv[1])) {
		pid = atoi(argv[1]);
	}
	flag_checker(argc, argv, "--per-process", &per_proc_flag);
	flag_checker(argc, argv, "--systemWide", &sysWide_flag);
	flag_checker(argc, argv, "--Vnodes", &Vnode_flag);
	flag_checker(argc, argv, "--composite", &composite_flag);
	flag_checker(argc, argv, "--summary", &summary_flag);
	get_threshold_val(argc, argv, &threshold_value);
	flag_checker(argc, argv, "--output_TXT", &text_write);
	flag_checker(argc, argv, "--output_binary", &binary_write);
		
		// DEAL WITH WEIRD FLAGS LATER
	// printf("%d %d %d %d %d\n", per_proc_flag, sysWide_flag, Vnode_flag, composite_flag, summary_flag);
	// display_composite(file_list);
	// display_vnodes(file_list);
	// display_process(file_list);
	// display_threshold(file_list, 80);
	if (per_proc_flag == 1) {
		display_process(file_list, pid);
	}
	if (sysWide_flag == 1) {
		display_system_wide(file_list, pid);
	}
	if (Vnode_flag == 1) { 
		display_vnodes(file_list, pid);
	}
	if (composite_flag == 1) { 
		display_composite(file_list, pid);
	}
	if (summary_flag == 1) {
		display_summary(file_list, pid);
	}
	if (threshold_value >= 0) {
		display_threshold(file_list, threshold_value, pid);
	}
	if (text_write == 1) {
		write_to_text(file_list, pid);
	}
	if (binary_write == 1) {
		write_to_bin(file_list, pid);
	}
	free2Dlist(file_list);
	return 0;
}
