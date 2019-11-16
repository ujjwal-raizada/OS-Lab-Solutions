#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {

	pid_t pid;
	int status;
	pid = fork();

	if (pid == 0)
		execlp("ls", "ls", "-la", NULL);
	else
		wait(&status);

	return 0;
}