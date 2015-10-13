#include "reply.h"
void reply(endpoint_t who, int result) {
  message m;
  m.m_type = result;
  send(who, &m);
}

void reply_err(endpoint_t who, int result) {
  message m;
  m.m_type = -result;
  send(who, &m);
}