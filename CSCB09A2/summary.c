#include"summary.h"

void display_summary(ll2d_t* list, pid_t pid) {
    printf("Summary Table\n");
    printf("===========\n");
    ll_t* curr = list->head;
    while (curr != NULL) {
        if (pid == -1) {
            printf("%d (%d), ", curr->pid, curr->size);
        }
        else {
            if (curr->pid == pid) {
                printf("%d (%d), ", curr->pid, curr->size);
            }
        }
        curr = curr->next;
    }
    printf("\n");
}