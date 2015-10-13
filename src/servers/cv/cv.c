#include <stdio.h>
#include <minix/sysutil.h>
#include <minix/sef.h>
#include <minix/ipc.h>
#include <minix/endpoint.h>
#include <minix/callnr.h>
#include "mutex.h"
#include "condition_variables.h"

int main() {
  sef_startup();
  init_m();
  init_cs();
  while (1) {
    message m;
    int s = sef_receive(ANY, &m);
    int res1, res2;

    switch(m.m_type) {

      case(CS_LOCK) :
        get_mutex(m.m1_i1, m.m_source);
        break;

      case(CS_UNLOCK) :
        release_mutex(m.m1_i1, m.m_source, 1);
        break;

      case(CS_WAIT) :
        wait_event(m.m1_i1, m.m1_i2, m.m_source);
        break;

      case(CS_BROADCAST) :
        broadcast(m.m1_i1, m.m_source);
        break;

      case(CS_SIGNALLED) :
        if(remove_m(m.PM_PROC, 0) == 1 || remove_c(m.PM_PROC, 1) == 1){
          reply_err(m.PM_PROC, EINTR);
        }
        break;

      case(CS_KILLED) :
        remove_m(m.PM_PROC, 1);
        remove_c(m.PM_PROC, 0);
        break;
    }
  }
}
