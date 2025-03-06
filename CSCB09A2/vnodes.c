#include "vnodes.h"

void display_vnodes(ll2d_t* list, pid_t pid) {
    ll_t* bigcurr = list->head;
	printf("FD\tInode\n");
	printf("=========================\n");
	while (bigcurr != NULL) {
		fnode_t* curr = bigcurr->head;
		while (curr != NULL) {
			if (pid == -1) {
				printf("%d\t%lld\n", curr->fd_num, curr->inode);
			}
			else {
				if (curr->pid == pid) {
					printf("%d\t%lld\n", curr->fd_num, curr->inode);
				}
			}
			curr = curr->next;
		}
		bigcurr = bigcurr->next;
	}
}