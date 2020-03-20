#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

char cmd[256] = "./";

int main() {
	
	
	for (;;) {
		printf("> ");
		fgets(&cmd[2], 256-2, stdin); 
		char * nl = strchr(cmd, '\n');
		if (nl) *nl = 0;
		
		if (strcmp(cmd+2, "exit") == 0) {
			break;
		}
		
		printf("command: %s\n", cmd);
	}
	
	return 0;
}
