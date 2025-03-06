#include"threshold.h"

void display_threshold(ll2d_t* list, int threshold, pid_t pid) {
	printf("## Offending processes:\n");
    ll_t* curr = list->head;
    while (curr != NULL) {
        if (pid == -1) {
            if (curr->size >= threshold) {
                printf("%d (%d), ", curr->pid, curr->size);
            }
        }
        else {
            if (curr->pid == pid) {
                if (curr->size >= threshold) {
                    printf("%d (%d), ", curr->pid, curr->size);
                }
            }
        }
        curr = curr->next;
    }
    printf("\n");
}
