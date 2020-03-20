#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char * argv[]) {
	for (int i = 1; i < argc; ++i) {
		open(argv[i], O_CREAT|O_RDWR, 0666);
	}
	for (;;) {
		sleep(1);
	}
}
