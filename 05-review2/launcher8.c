#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define BUFSIZE 256

char cmd[BUFSIZE] = "./";
char output[BUFSIZE];
char cmd2[BUFSIZE] = "./";

char * trim(char * str) {
	while (*str && isspace(*str)) ++str;
	for (int idx = strlen(str) - 1; idx >= 0; --idx) {
		if (!isspace(str[idx])) {
			str[idx + 1] = '\0';
			break;
		}
	}
	return str;
}

void split(char * fullLine, char * splitPoint, char * extracted) {
	char * outf = trim(splitPoint + 1);
	strcpy(extracted, outf);
	*splitPoint = '\0';
	trim(fullLine);
}

int main() {
	int pipefd[2];

	for (;;) {
		printf("> ");
		fgets(&cmd[2], BUFSIZE-2, stdin); 
		char * nl = strchr(cmd, '\n');
		if (nl) *nl = 0;
		
		if (strcmp(cmd+2, "exit") == 0) {
			break;
		}
		
		char * redir = strchr(cmd, '>');
		if (redir) {
			split(cmd, redir, output);
		}

		char * pp = strchr(cmd, '|');
		if (pp) {
			split(cmd, pp, cmd2+2);
		}

		if (cmd2[2]) {  // pipe
			pipe(pipefd);
			// pipefd[0] : read end of pipe
			// pipefd[1] : write end of pipe
		}
		
		pid_t pid1 = fork();
		if (pid1 == 0) {
			if (output[0]) { // redir
				int fd = open(output, O_CREAT|O_RDWR, 0666);
				if (fd >= 0) {
					dup2(fd, 1);
					close(fd);
				} else {
					perror("open failed");
				}
			}
			
			if (cmd2[2]) {  // pipe
				dup2(pipefd[1], 1);
				close(pipefd[0]);
				close(pipefd[1]);
			}
			
			execlp(cmd, cmd+2, NULL);
			perror("exec failed");
			exit(1);
		}

		pid_t pid2 = -1;
		if (cmd2[2]) {  // pipe
			pid2 = fork();
			if (pid2 == 0) {
				dup2(pipefd[0], 0);
				close(pipefd[0]);
				close(pipefd[1]);
				
				execlp(cmd2, cmd2+2, NULL);
				perror("exec failed");
				exit(1);
			}
		}

		close(pipefd[0]);
		close(pipefd[1]);

		int status;
		if (pid1 > 0) {
			waitpid(pid1, &status, 0);
			if (WIFEXITED(status)) {
				printf("res: %d\n", WEXITSTATUS(status));
			} else {
				puts("end");
			}
		}
		if (pid2 > 0) {
			waitpid(pid2, &status, 0);
			if (WIFEXITED(status)) {
				printf("res2: %d\n", WEXITSTATUS(status));
			} else {
				puts("end2");
			}
		}
	}
	
	return 0;
}
