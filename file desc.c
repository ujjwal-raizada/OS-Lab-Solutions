// 0 -> stdin
// 1 -> stdout
// 2 -> stderr

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <errno.h>

int main() {
	pid_t pid;
	int pfd[2];
	int s;
	pipe(pfd);
	pid = fork();

	if (pid == 0) {
		printf("child 1\n");
		dup2(pfd[1], 1);
		close(pfd[0]);
		close(pfd[1]);
		if (execlp("ls", "ls", NULL) == -1)
			perror("execlp ls");
	}
	else {
		if (fork() == 0) {
			printf("child 2\n");
			dup2(pfd[0], 0);
			close(pfd[0]);
			close(pfd[1]);
			if (execlp("wc", "wc", "-l", NULL) == -1)
				perror("execlp wc");
		}
		else {
			close(pfd[0]);
			close(pfd[1]);
			wait(&s);
			wait(&s);
		}
	}
}