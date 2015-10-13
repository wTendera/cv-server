#include <minix/type.h>
#include <stdio.h>
#include <minix/sysutil.h>
#include <minix/sef.h>
#include <minix/ipc.h>
#include <minix/endpoint.h>
#include <minix/callnr.h>
#include <stdlib.h>
#include <errno.h>
typedef struct event_node {
  int mutex_id;
  endpoint_t owner;
  struct event_node* next;
} event_node;

typedef struct event_queue {
  event_node *head;
} event_queue;

typedef struct event {
  event_queue queue;
  struct event* next;
  int id;
} event;

typedef struct event_list {
  event* head;
} event_list;

void wait_event(int cond_var_id, int mutex_id, endpoint_t who);

void broadcast(int cond_var_id, endpoint_t who);

void init_cs();

int remove_c(endpoint_t who, int q);