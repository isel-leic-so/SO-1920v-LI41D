#include <stdio.h>
#include <unistd.h>

#include "echo-service.h"

int main() {
	puts(":: STARTED ::");

	memch_t * smc = mc_create(sizeof (echomsg_t), 8, ECHO_SERVICE_ID);
	if (!smc) {
		exit(1);
	}

	pid_t pid = getpid();
	channel_name_t resp_ch_name = { .parts = { .base = ECHO_CLIENT_ID_BASE } };

	echomsg_t msg;
	for (;;) {
		puts("++ Waiting for messages ++");
		mc_get(smc, &msg);

		sprintf(resp_ch_name.parts.pid, "%d", msg.sender);

		printf("MSG[%d]: \"%s\" (reply to: %s)\n", msg.sender, msg.msg, resp_ch_name.name);
		
		if (msg.sender) {
			memch_t * rmc = mc_open(resp_ch_name.name);
			if (rmc) {
				msg.sender = pid;
				mc_put(rmc, &msg);
				mc_close(rmc);
			}
		}

		if (msg.msg[0] == 0) {
			puts("++ Stopping ++");
			break;
		}
	}

	puts("++ Cleanup ++");
	mc_destroy(smc);

	puts(":: END ::");
	return 0;
}	
