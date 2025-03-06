#include "per_process.h"

void display_process(ll2d_t* list, pid_t pid) {
	printf("PID\tFD\n");
    printf("=============\n");
	ll_t* bigcurr = list->head;
	while (bigcurr != NULL) {
		fnode_t* curr = bigcurr->head;
		while (curr != NULL) {
			if (pid == -1) {
				printf("%d\t%d\n", curr->pid, curr->fd_num);
			}
			else {
				if (curr->pid == pid) {
					printf("%d\t%d\n", curr->pid, curr->fd_num);
				}
			}
			curr = curr->next;
		}
		bigcurr = bigcurr->next;
	}
}
