#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>

#ifndef LINKED_LIST_H
#define LINKED_LIST_H
typedef struct filenode {
	pid_t pid;
	char name[1024];
	unsigned long long inode;			// could be vnode value
	int fd_num;
	struct filenode* next;
} fnode_t;

typedef struct linked_list {
	int size;
	pid_t pid;
	fnode_t* head;
	struct linked_list* next;
} ll_t;

typedef struct linked_list_2d {
	int total_size;
	ll_t* head;
} ll2d_t;

ll_t* create1DList();
ll2d_t* create2DList();
void insert1D(ll_t* list, int pid, char* name, int inode, int fd_num);
void insert2D(ll2d_t* list_2d, ll_t* list_1d);
void print1Dlist(ll_t* list, int* i);
void print1Dlisttofile(ll_t* list, int* i);
void print2Dlist(ll2d_t* list_2d);
void free1Dlist(ll_t* list);
void free2Dlist(ll2d_t* list);
#endif
