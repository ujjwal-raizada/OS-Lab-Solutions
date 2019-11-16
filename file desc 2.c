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
	int pfd1[2];
	int pfd2[2];
	int s;
	pipe(pfd1);
	pipe(pfd2);
	

	if (fork() == 0) {
		printf("child 1\n");
		dup2(pfd1[1], 1);
		close(pfd1[1]);
		close(pfd1[0]);
		if (execlp("ls", "ls", "-l", NULL) == -1)
			perror("execlp ls");
		printf("ls done.\n");
	}
	else {
		if (fork() == 0) {
			printf("child 2\n");
			dup2(pfd1[0], 0);
			close(pfd1[1]);
			close(pfd1[0]);

			dup2(pfd2[1], 1);
			close(pfd2[0]);
			close(pfd2[1]);
			if (execlp("grep", "grep", "16", NULL) == -1)
				perror("execlp grep");
			printf("grep done.\n");
		}
		else if (fork() == 0) {
			printf("child 3\n");
			dup2(pfd2[0], 0);
			close(pfd2[0]);
			close(pfd2[1]);

			close(pfd1[0]);
			close(pfd1[1]);
			if (execlp("wc", "wc", "-l", NULL) == -1)
				perror("execlp wc");
			printf("wc done.\n");
		}
		else {
			close(pfd1[0]);
			close(pfd1[1]);
			close(pfd2[0]);
			close(pfd2[1]);
			printf("parent waiting.\n");
			wait(&s);
			wait(&s);
			wait(&s);
		}
	}
}