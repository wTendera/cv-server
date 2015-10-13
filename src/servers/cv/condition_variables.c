#include "condition_variables.h"
#include "mutex.h"
#include "reply.h"
event_list list2;

event* create_event(int cond_var_id) {
  event_queue* e_queue = malloc(sizeof(event_queue));
  e_queue->head = NULL;
  event* e = malloc(sizeof(event));
  e->queue = *e_queue;
  e->next = NULL;
  e->id = cond_var_id;
  event* cur = list2.head;
  if(!cur) {
    list2.head = e;
  } else {
    while(cur->next)
      cur = cur->next;

    cur->next = e;
  }
  return e;
}

void enqueue_event(event* e, endpoint_t who, int mutex_id) {
  event_node* e_node = malloc(sizeof(event_node));
  e_node->mutex_id = mutex_id;
  e_node->owner = who;
  e_node->next = NULL;
  event_node* cur = e->queue.head;
  if(!cur) {
    e->queue.head = e_node;
    return;
  }
  while(cur->next) 
    cur = cur->next;
  cur->next = e_node;
}

void find_or_create_event(int cond_var_id, int mutex_id, endpoint_t who) {
  event* cur = list2.head;
  while(cur && cur->id != cond_var_id)
    cur = cur->next;
  if(!cur)  {
    cur = create_event(cond_var_id);
  }
  enqueue_event(cur, who, mutex_id);
}

void clear_queue(int cond_var_id) {
  event* cur = list2.head;

  while(cur && cur->id != cond_var_id)
    cur = cur->next;
  
  if(!cur)
    return;
  event_node* current = cur->queue.head;
  event_node* old;
  while(current) {
    old = current;
    get_mutex(current->mutex_id, current->owner);
    current = current->next;
    cur->queue.head = current;
    free(old);
  }
}

int remove_c(endpoint_t who, int q) {
  event* cur = list2.head;
  while(cur) {
    event_node* current = cur->queue.head;
    event_node* previous = NULL;
    while(current) {
      if(current->owner == who) {
        if(previous) {
          previous->next = current->next;
        } else {
          cur->queue.head = current->next;
        }
        free(current);
        return 1;
      }
      previous = current;
      current = current->next;
    }
    cur = cur->next;
  }
  return 0;
}

void wait_event(int cond_var_id, int mutex_id, endpoint_t who) {
  if(!has_mutex(mutex_id, who)) {
    reply(who, EINVAL);
    return;
  }
  release_mutex(mutex_id, who, 0);
  find_or_create_event(cond_var_id, mutex_id, who);
}

void broadcast(int cond_var_id, endpoint_t who) {
  clear_queue(cond_var_id);
  reply(who, 0);
}

void init_cs() {
  list2.head = NULL;
}