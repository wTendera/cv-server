#include <cv.h>
#include <stdio.h>
#include <minix/ipc.h>
#include <minix/endpoint.h>
#include <minix/rs.h>
#include <minix/callnr.h>
#include <lib.h>

int cs_lock(int mutex_id) {
  message m;
  m.m1_i1 = mutex_id;
  endpoint_t cv;
  minix_rs_lookup("cv", &cv);
  int s;
  while((s = _syscall(cv, CS_LOCK, &m)) == -1 && errno == EINTR) {
    m.m1_i1 = mutex_id;
  }
  if(s == 0)
    errno = 0;
  return s;
}

int cs_unlock(int mutex_id) {
  message m;
  m.m1_i1 = mutex_id;
  endpoint_t cv;
  minix_rs_lookup("cv", &cv);
  return _syscall(cv, CS_UNLOCK, &m);
}

int cs_wait(int cond_var_id, int mutex_id) {
  message m;
  m.m1_i1 = cond_var_id;
  m.m1_i2 = mutex_id;
  endpoint_t cv;
  minix_rs_lookup("cv", &cv);
  int s = _syscall(cv, CS_WAIT, &m);
  if(s == -1 && errno == EINTR)
    return cs_lock(mutex_id);
  return s;
}

int cs_broadcast(int cond_var_id) {
  message m;
  m.m1_i1 = cond_var_id;
  endpoint_t cv;
  minix_rs_lookup("cv", &cv);
  return _syscall(cv, CS_BROADCAST, &m);
}