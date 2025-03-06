#include "file_proc.h"

int is_pid(char* str) {
	int i = 0;
	while (str[i] != '\0') {
		if (!isdigit(str[i])) {
			return 0;
		}
		i += 1;
	}
	return 1;
}

void fd_process(pid_t pid, ll2d_t* list) {
	char fd_path[256];
	struct dirent* dir_entry;
	DIR* dir;
	char link_target[1024];
	ssize_t len;
	char fd_desc[1024];
	unsigned long long inode_val;
	int fd_num;
	// Initialize a sublist
	ll_t* sublist = NULL;
	sublist = calloc(1, sizeof(ll_t));
	sublist->pid = pid;
	// proc[pid] processing
	snprintf(fd_path, sizeof(fd_path), "/proc/%d/fd", pid);
	dir = opendir(fd_path);
	if (dir == NULL) {
		return;
	}
	while ((dir_entry = readdir(dir)) != NULL) {
		if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0) {
			continue;
		}
		snprintf(fd_desc, sizeof(fd_desc), "%s/%s", fd_path, dir_entry->d_name);
		struct stat file_stat;
		fd_num = atoi(dir_entry->d_name);
		if (stat(fd_desc, &file_stat) == 0) {
			inode_val = (unsigned long long) file_stat.st_ino;
			len = readlink(fd_desc, link_target, sizeof(link_target) - 1);
			if (len != -1) {
				link_target[len] = '\0';
				// printf("FD %s :-> %s\n", dir_entry->d_name, link_target);
				insert1D(sublist, pid, link_target, inode_val, fd_num);
				sublist->size += 1;
			}
		}
	}
	closedir(dir);
	insert2D(list, sublist);
}

ll2d_t* all_files_handling() {
	ll2d_t* filelist = NULL;
	filelist = calloc(1, sizeof(ll2d_t));
	DIR* proc_dir;
	struct dirent* proc_entry;
	proc_dir = opendir("/proc");
	if (proc_dir == NULL) {
		return NULL;
	}
	while ((proc_entry = readdir(proc_dir)) != NULL) {
		if (is_pid(proc_entry->d_name) == 1) {
			pid_t pid = atoi(proc_entry->d_name);
			fd_process(pid, filelist);
		}
	}
	closedir(proc_dir);
	return filelist;
}
