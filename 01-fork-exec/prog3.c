#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	printf("PID:  %d\n", getpid());
	printf("PPID: %d\n", getppid());

	printf("MESSAGE FROM PARENT");
	
	pid_t pid = fork();
	if (pid == -1) {
		fprintf(stderr, "ERROR\n");
		exit(1);
	}
	
	puts("");
	if (pid > 0) {
		printf("== PARENT PROCESS ==\n");
		printf("PARENT PID:  %d\n", getpid());
		printf("PARENT PPID: %d\n", getppid());
		waitpid(pid, NULL, 0);
	} else {
		printf("== CHILD PROCESS ==\n");
		printf("CHILD PID:  %d\n", getpid());
		printf("CHILD PPID: %d\n", getppid());
	}
	return 0;
}

