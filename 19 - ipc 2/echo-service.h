#pragma once

#include "memchannel.h"

#define ECHO_SERVICE_ID "pt.isel.leic.so.memecho"

#define ECHO_MSG_MAX_LEN 256

typedef struct echomsg {
	int sender;
	char msg[ECHO_MSG_MAX_LEN];
} echomsg_t;

// ============================

// ECHO_CLIENT_ID : "pt.isel.leic.so.memecho.client.PID"

#define ECHO_CLIENT_PREFIX ".client."

#define ECHO_CLIENT_ID_BASE ECHO_SERVICE_ID ECHO_CLIENT_PREFIX

#define PID_MAX_LEN 10

typedef union channel_name {
	struct {
		char base[(sizeof ECHO_CLIENT_ID_BASE) - 1];
		char pid[PID_MAX_LEN];
	} parts;
	char name[1];
} channel_name_t;
