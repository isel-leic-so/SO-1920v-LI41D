#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int a = 1234;

int main() {
	int res;

	printf("PID:  %d\n", getpid());
	printf("PPID: %d\n", getppid());
	
	pid_t pid = fork();
	if (pid == -1) {
		fprintf(stderr, "Fork error.\n");
		exit(1);
	}
	
	if (pid > 0) {
		printf("== PARENT PROCESS ==\n");
		printf("PARENT PID:  %d\n", getpid());
		printf("PARENT PPID: %d\n", getppid());
	} else {
		printf("== CHILD PROCESS ==\n");
		printf("CHILD PID:  %d\n", getpid());
		printf("CHILD PPID: %d\n", getppid());
		a += 1111;
	}

	// COMMON CODE (runs in parent and child processes)
	for (int i = 0; i < 8; ++i) {
		printf("[%d] var a at address %p has value %d\n", getpid(), (void*)&a, a);
		++a;
		sleep(1);
	}
	
	if (pid > 0) {
		waitpid(pid, &res, 0);
		if (WIFEXITED(res)) {
			printf("CHILD RESULT: %d\n", WEXITSTATUS(res));
		}
		return 0;
	} else {
		return 88;
	}
}

