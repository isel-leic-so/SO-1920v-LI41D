#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	printf("PID:  %d\n", getpid());
	printf("PPID: %d\n", getppid());
	
	pid_t pid = fork();
	if (pid == -1) {
		fprintf(stderr, "ERROR\n");
		exit(1);
	}
	
	if (pid > 0) {
		int res;
		printf("== PARENT PROCESS ==\n");
		printf("PARENT PID:  %d\n", getpid());
		printf("PARENT PPID: %d\n", getppid());
		waitpid(pid, &res, 0);
		if (WIFEXITED(res)) {
			printf("RESULT: %d\n", WEXITSTATUS(res));
		}
		return 0;
	} else {
		printf("== CHILD PROCESS ==\n");
		printf("CHILD PID:  %d\n", getpid());
		printf("CHILD PPID: %d\n", getppid());
		return 88;
	}
}

