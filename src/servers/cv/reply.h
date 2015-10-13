#include <minix/type.h>
#include <stdio.h>
#include <minix/sysutil.h>
#include <minix/sef.h>
#include <minix/ipc.h>
#include <minix/endpoint.h>
#include <minix/callnr.h>
#include <stdlib.h>
#include <errno.h>

void reply(endpoint_t who, int result);
void reply_err(endpoint_t who, int result);