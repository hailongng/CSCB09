#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include"per_proc.h"
#include"linked_list.h"

bool isnum(char* str) {
	return true;
}

void flag_checker(int argc, char** argv, char* str, int* flag) {
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], str) == 0) {
			*flag = 1;
		}
	}
}

int main(int argc, char** argv) {
	int per_proc_flag = 0, sysWide_flag = 0, Vnode_flag = 0, composite_flag = 0, summary_flag = 0;
	// need threshold as well
	if (argc > 7) {
		perror("Invalid CLA\n");
		return 1;
	}
	if (argc == 1) {
		// Display composite table: my decision
		composite_flag = 1;
	}
	else if ((argc == 2) && (isnum(argv[1]))) {
		// Deal with this case
	}
	else {
		flag_checker(argc, argv, "--per-process", &per_proc_flag);
		flag_checker(argc, argv, "--systemWide", &sysWide_flag);
		flag_checker(argc, argv, "--Vnodes", &Vnode_flag);
		flag_checker(argc, argv, "--composite", &composite_flag);
		flag_checker(argc, argv, "--summary", &summary_flag);
		// DEAL WITH WEIRD FLAGS LATER
	}
	all_process_handling();
	return 0;
}
