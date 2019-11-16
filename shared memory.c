#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <errno.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int main() {
	int shmid;
	char *shmPtr;
	int n;

	if (fork() == 0) {
		sleep(5);
		if ((shmid = shmget(2041, 32, 0)) == -1) {
			exit();
		}
	}
}