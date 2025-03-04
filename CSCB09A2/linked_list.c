#include "linked_list.h"

ll_t* createList() {
	ll_t* list = calloc(1, sizeof(ll_t));
	list->size = 0;
	return list;
}

void insert(ll_t* list, int pid, char* name, int inode, int fd_num) {
	fnode_t* newNode = calloc(1, sizeof(fnode_t));
	newNode->pid = pid;
	strcpy(newNode->name, name);
	newNode->inode = inode;
	newNode->fd_num = fd_num;
	newNode->next = NULL;
	if (list->head == NULL) {
		list->head = newNode;
		return;
	}
	else {
		fnode_t* curr = list->head;
		while (curr->next != NULL) {
			curr = curr->next;
		}
		curr->next = newNode;
	}
	list->size += 1;
}

void freelist(ll_t* list) {
	fnode_t* curr = list->head;
	fnode_t* next = curr->next;
	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}
	free(list);
}

void printlist(ll_t* list) {
	fnode_t* curr = list->head;
	while (curr != NULL) {
		printf("PID: %d \t FD: %d\t Name: %s INODE: %d\n", curr->pid, curr->fd_num, curr->name, curr->inode);
		curr = curr->next;
	}
}
