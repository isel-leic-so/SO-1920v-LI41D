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
		
		pid_t pid = fork();
		if (pid == 0) {
			if (output[0]) { // redir
				int fd = open(output, O_CREAT|O_RDWR, 0666);
				if (fd >= 0) {
					dup2(fd, 1);
					close(fd);
				} else {
					perror("open failed");
				}
			}
			
			execlp(cmd, cmd+2, NULL);
			perror("exec failed");
			exit(1);
		} else {
			int status;
			waitpid(pid, &status, 0);
			if (WIFEXITED(status)) {
				printf("res: %d\n", WEXITSTATUS(status));
			} else {
				puts("end");
			}
		}
	}
	
	return 0;
}
