#include <cv.h>
#include <stdio.h>
#include <minix/ipc.h>
#include <minix/endpoint.h>
#include <minix/rs.h>
#include <minix/callnr.h>
#include <lib.h>

void tell_cv(message *m);
