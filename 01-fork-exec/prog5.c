#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

	pid_t pid = fork();
	if (pid == 0) {
		int fd = open("zzzz.txt", O_CREAT | O_RDWR, 0666);
		printf("fd: %d\n", fd);
		execlp("./prog4", "prog4", NULL);
		fprintf(stderr, "CANNOT OCCUR\n");
	} else {
		waitpid(pid, NULL, 0);
	}

	return 0;
}
