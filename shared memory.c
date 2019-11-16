#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <errno.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>


int main() {
	int shmid;
	char *shmPtr;
	int n;

	if (fork() == 0) {
		sleep(5);
		if ((shmid = shmget(2041, 32, 0)) == -1) {
			exit(1);
		}

		shmPtr = shmat(shmid, 0, 0);
		// if (shmPtr == (char *)-1)
			// exit(2);
		while (shmPtr[0] == 0) {
			printf("waiting...\n");
		}

		int len = 0;
		while (shmPtr[len] != '\0')
			len++;

		printf("len: %d\n", len);

		printf("\nchild reading...\n");
		for (n = 0; n < len + 1; n++)
			putchar(shmPtr[n]);
		putchar('\n');
	}
	else {
		if ((shmid = shmget(2041, 32, 0666 | IPC_CREAT)) == -1)
			exit(1);

		shmPtr = shmat(shmid, 0, 0);
		if (shmPtr == (char *)-1)
			exit(2);

		for (n = 0; n < 26; n++)
			shmPtr[n] = 0;

		sleep(10);

		for (n = 0; n < 26; n++) {
			shmPtr[n] = 'a' + n;
		}

		printf("\nParent writing...\n");
		for (n = 0; n < 26; n++)
			putchar(shmPtr[n]);
		putchar('\n');
		wait(NULL);
		if (shmctl(shmid, IPC_RMID, NULL) == -1) {
			perror("shmctl");
			exit(-1);
		}
	}
	return 0;
}