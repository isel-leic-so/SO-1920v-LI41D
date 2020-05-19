#include <stdio.h>
#include <unistd.h>
#include "tpool.h"

void print_msg(void * arg) {
	const char * msg = (const char *)arg;
	puts(msg);
}

int main() {
	tpool_t tpool;
	
	tp_init(&tpool, 3);
	
	tp_submit(&tpool, print_msg, "msg01");
	tp_submit(&tpool, print_msg, "msg02");
	tp_submit(&tpool, print_msg, "msg03");
	tp_submit(&tpool, print_msg, "msg04");
	tp_submit(&tpool, print_msg, "msg05");
	tp_submit(&tpool, print_msg, "msg06");
	
	sleep(8); // inapropriado; falta funcao no thread pool para concluir
	
	return 0;
}