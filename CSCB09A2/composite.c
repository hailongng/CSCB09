#include "composite.h"

void display_composite(ll2d_t* list_2d, pid_t pid) {
	printf("#\tPID\tFD\tFilename\tInode\n");
	printf("===================================================\n");
	int i = 0;
	ll_t* bigcurr = list_2d->head;
	while (bigcurr != NULL) {
		fnode_t* curr = bigcurr->head;
		while (curr != NULL) {
			if (pid == -1) {
				printf("%d\t%d\t%d\t%s\t%llu\n", i, curr->pid, curr->fd_num, curr->name, curr->inode);
				i++;
			}
			else {
				if (curr->pid == pid) {
					printf("%d\t%d\t%d\t%s\t%llu\n", i, curr->pid, curr->fd_num, curr->name, curr->inode);
					i++;
				}
			}
			curr = curr->next;
		}
		bigcurr = bigcurr->next;
	}
}

void write_to_text(ll2d_t* list_2d, pid_t pid) {
	FILE* fp = fopen("compositeTable.txt", "w");
	if (fp == NULL) {
		printf("Unable to open text file. Terminating...\n");
		return;
	}
	ll_t* bigcurr = list_2d->head;
	int i = 0;
	while (bigcurr != NULL) {
		fnode_t* curr = bigcurr->head;
		while (curr != NULL) {
			if (pid == -1) {
				fprintf(fp, "%d\t%d\t%d\t%s\t%llu\n", i, curr->pid, curr->fd_num, curr->name, curr->inode);
				i++;
			}
			else {
				if (curr->pid == pid) {
					fprintf(fp, "%d\t%d\t%d\t%s\t%llu\n", i, curr->pid, curr->fd_num, curr->name, curr->inode);
					i++;
				}
			}
			curr = curr->next;
		}
		bigcurr = bigcurr->next;
	}
	fclose(fp);
}

void write_to_bin(ll2d_t* list_2d, pid_t pid) {
	FILE* fp = fopen("compositeTable.bin", "wb");
	if (fp == NULL) {
		printf("Unable to open binary file. Terminating...\n");
		return;
	}
	ll_t* bigcurr = list_2d->head;
	int i = 0;
	while (bigcurr != NULL) {
		fnode_t* curr = bigcurr->head;
		while (curr != NULL) {
			if (pid == -1) {
				fwrite(&i, sizeof(int), 1, fp);
				fwrite("\t", sizeof(char), 1, fp);
				fwrite(&(curr->pid), sizeof(int), 1, fp);
				fwrite("\t", sizeof(char), 1, fp);
				fwrite(&(curr->fd_num), sizeof(int), 1, fp);
				fwrite("\t", sizeof(char), 1, fp);
				fwrite(curr->name, sizeof(char), strlen(curr->name), fp);
				fwrite("\t", sizeof(char), 1, fp);
				fwrite(&(curr->inode), sizeof(unsigned long long), 1, fp);
				fwrite("\n", sizeof(char), 1, fp);
				i++;
			}
			else {
				if (curr->pid == pid) {
					fwrite(&i, sizeof(int), 1, fp);
					fwrite("\t", sizeof(char), 1, fp);
					fwrite(&(curr->pid), sizeof(int), 1, fp);
					fwrite("\t", sizeof(char), 1, fp);
					fwrite(&(curr->fd_num), sizeof(int), 1, fp);
					fwrite("\t", sizeof(char), 1, fp);
					fwrite(curr->name, sizeof(char), strlen(curr->name), fp);
					fwrite("\t", sizeof(char), 1, fp);
					fwrite(&(curr->inode), sizeof(unsigned long long), 1, fp);
					fwrite("\n", sizeof(char), 1, fp);
					i++;
				}
			}
			curr = curr->next;
		}
		bigcurr = bigcurr->next;
	}
	fclose(fp);
}
