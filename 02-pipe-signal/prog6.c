#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

	int pipefd[2];

	pipe(pipefd);

	pid_t pid = fork();
	if (pid == 0) {
		// child
		close(pipefd[1]);
		
		char data[6];
		read(pipefd[0], data, 6);  // (de facto, pode ler menos de 6)
		
		printf("Message from parent: %s\n", data);
		
		close(pipefd[0]);
		exit(78);
		
	} else {
		// parent
		close(pipefd[0]);
		
		// pausa
		sleep(2);
		
		// envio de mensagem para o processo filho
		write(pipefd[1], "HELLO", 6);
		
		waitpid(pid, NULL, 0);
		close(pipefd[1]);
	}

	return 0;
}
