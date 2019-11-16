#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {

	pid_t pid;
	int status;
	pid = fork();

	if (pid < 0) {

		perror("fork error");
	}
	else if (pid == 0) {
		printf("Child process PID: %d\n", getpid());
		sleep(5);
		printf("Child done.\n");
	}
	else {
		printf("This is parent process\n");
		printf("PID of child: %d, PID of parent: %d\n", pid, getpid());
		sleep(15);  // zombie process
		wait(&status);
		printf("parent done.\n");
	}
	return 0;
}