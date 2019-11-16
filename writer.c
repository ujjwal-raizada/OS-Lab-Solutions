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

	printf("Enter lines of text: ^D to quit.\n");

	buf.mtype = 1;

	while (fgets(buf.mtext, sizeof(buf.mtext), stdin) != NULL) {
		int len = strlen(buf.mtext);

		if (buf.mtext[len - 1] == '\n')
			buf.mtext[len - 1] == '\0';

		if (msgsnd(msgid, &buf, len + 1, 0) == -1)
			perror("msgsnd");
	}
	if (msgctl(msgid, IPC_RMID, NULL) == -1) {
		perror("msgctl");
		exit(1);
	}
	return 0;
}
