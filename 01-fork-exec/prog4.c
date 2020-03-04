#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

// Run with:
//    ./prog4 3> somefile.txt

int main() {
	int res = write(3, "HELLO, WORLD!\n", 14);
	if (res == -1) {
		perror("write failed: ");
		exit(1);
	} else {
		fprintf(stderr, "res = %d\n", res);
	}
	return 0;
}
