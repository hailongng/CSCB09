#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/types.h>

typedef struct filenode {
	pid_t pid;
	char name[1024];
	int inode;			// could be vnode value
	int fd_num;
	struct filenode* next;
} fnode_t;

typedef struct ll_node {
	int size;
	fnode_t* head;
}ll_t;

ll_t* createList();
void insert(ll_t* list, int pid, char* name, int inode, int fd_num);
void printlist(ll_t* list);
void freelist(ll_t* list);

