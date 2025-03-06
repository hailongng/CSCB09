#include "sys_wide.h"

void display_system_wide(ll2d_t* list, pid_t pid) {
    printf("PID\tFD\tFilename\n");
    printf("====================================================\n");
	ll_t* bigcurr = list->head;
	while (bigcurr != NULL) {
		fnode_t* curr = bigcurr->head;
		while (curr != NULL) {
			if (pid == -1) {
				printf("%d\t%d\t%s\n", curr->pid, curr->fd_num, curr->name);
			}
			else {
				if (curr->pid == pid) {
					printf("%d\t%d\t%s\n", curr->pid, curr->fd_num, curr->name);
				}
			}
			curr = curr->next;
		}
		bigcurr = bigcurr->next;
	}
}
