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
	struct msgbuf buf;
	int msgid;
	key_t key;

	if ((key = ftok("writer.c", 'B')) == -1) {
		perror("ftok");
		exit(1);
	}

	if ((msgid = msgget(key, 0644 | IPC_CREAT)) == -1) {
		perror("msgget");
		exit(1);
	}

	printf("Reader: ready to receive messages, boss.\n");

	while (1) {

		if (msgrcv(msgid, &buf, sizeof(buf.mtext), 0, 0) == -1) {
			perror("msgrcv");
			exit(1);
		}
		printf("reader: \"%s\"\n", buf.mtext);

	}
	return 0;
}
