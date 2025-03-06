#include "linked_list.h"

ll_t* create1DList() {
	ll_t* list = calloc(1, sizeof(ll_t));
	list->size = 0;
	return list;
}

ll2d_t* create2DList() {
	ll2d_t* list = calloc(1, sizeof(ll2d_t));
	list->total_size = 0;
	return list;
} 

void insert1D(ll_t* list, int pid, char* name, int inode, int fd_num) {
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

void insert2D(ll2d_t* list_2d, ll_t* list_1d) {
	if (list_2d->head == NULL) {
		list_2d->head = list_1d;
	}
	else {
		ll_t* curr = list_2d->head;
		while (curr->next != NULL) {
			curr = curr->next;
		}
		curr->next = list_1d;
	}
	list_2d->total_size += list_1d->size;
}

void free1Dlist(ll_t* list) {
	fnode_t* curr = list->head;
	fnode_t* next = curr->next;
	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}
	free(list);
}

void free2Dlist(ll2d_t* list_2d) {
	ll_t* curr = list_2d->head;
	ll_t* next = curr->next;
	while (curr != NULL) {
		next = curr->next;
		free1Dlist(curr);
		curr = next;
	}
	free(list_2d);
}

void print1Dlist(ll_t* list, int* i) {
	fnode_t* curr = list->head;
	while (curr != NULL) {
		printf("%d:\t", *i);
		printf("%d\t%d\t%s\t%lld\n", curr->pid, curr->fd_num, curr->name, curr->inode);
		(*i)++;
		curr = curr->next;
	}
}

void print1Dlisttofile(ll_t* list, int* i) {
	FILE* fptr = fopen("sample.txt", "w");
	fnode_t* curr = list->head;
	while (curr != NULL) {
		fprintf(fptr, "%d:\t%d\t%d\t%s\t%lld\n", *i, curr->pid, curr->fd_num, curr->name, curr->inode);
		(*i)++;
		curr = curr->next;
	}
}

void print2Dlist(ll2d_t* list_2d) {
	ll_t* bigcurr = list_2d->head;
	int i = 0;
	while (bigcurr != NULL) {
		print1Dlist(bigcurr, &i);
		//printf("%d:\t", i);
		//printf("%d\t%d\n", curr->pid, curr->size);
		//i++;
		bigcurr = bigcurr->next;
	}
}
