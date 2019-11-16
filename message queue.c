#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <errno.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>
#include<sys/msg.h>

struct msgbuf {
	long mtype;
	char mtext[200];
};

int main(void) {
	int msgid;
	key_t key;

	if ((key = ftok("writer.c", 'B')) == -1) {
		perror("ftok");
		exit(1);
	}

	if (fork() == 0) {
		struct msgbuf buf;
		// writer
		if ((msgid = msgget(key, 0644 | IPC_CREAT)) == -1) {
			perror("msgget");
			exit(1);
		}

		printf("Enter lines of text: ^D to quit.\n");

		buf.mtype = 1;

		while (fgets(buf.mtext, sizeof(buf.mtext), stdin) != NULL) {
			printf("1");
			int len = strlen(buf.mtext);

			if (buf.mtext[len - 1] == '\n')
				buf.mtext[len - 1] == '\0';

			if (msgsnd(msgid, &buf, len + 1, 0) == -1)
				perror("msgsnd");
			sleep(2);
			printf("2");
		}
		if (msgctl(msgid, IPC_RMID, NULL) == -1) {
			perror("msgctl");
			exit(1);
		}
		printf("3");

	}
	else {
		struct msgbuf buf;
		// reader
		if ((msgid = msgget(key, 0644 | IPC_CREAT)) == -1) {
			perror("msgget");
			exit(1);
		}

		printf("Reader: ready to receive messages, boss.\n");

		while (1) {
			printf("4");
			if (msgrcv(msgid, &buf, sizeof(buf.mtext), 0, 0) == -1) {
				perror("msgrcv");
				exit(1);
			}
			printf("5");
			printf("reader: \"%s\"\n", buf.mtext);

		}
	}

	
	return 0;
}
