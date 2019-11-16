#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>

int main() {

	int pfds[2];
	pid_t pid;
	char buf[30];
	int pipe_id = pipe(pfds);
	int status;

	if (pipe_id == -1) {
		perror ("pipe");
		exit(1);
	}

	pid = fork();

	if (pid == 0) {
		close(pfds[0]);
		printf("writing to file descriptor #%d\n", pfds[1]);
		write(pfds[1], "test", 5);
		wait(&status);
		printf("status: %d\n", status);
	}
	else {
		close(pfds[1]);
		printf("reading from the file descriptor #%d\n", pfds[0]);
		read(pfds[0], buf, 5);
		printf("read \"%s\"\n", buf);
	}
	
	return 0;
}