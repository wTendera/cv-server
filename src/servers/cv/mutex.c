#include "mutex.h"
mutex_list list;

mutex* find_mutex(int mutex_id) {
  mutex* cur = list.head;
  while(cur && cur->id != mutex_id)
    cur = cur->next;
  return cur;
}

void pop_queue(int mutex_id, endpoint_t who, int wake_owner) {
  mutex* cur = find_mutex(mutex_id);
  if(!cur){
    reply(who, -EPERM);
    return;
  }
  mutex_queue* queue = &cur->queue;
  if(!queue) {
    reply(who, -EPERM);
    return;
  }
  mutex_node* old = queue->head;
  if(!old) {
    reply(who, -EPERM);
    return;
  }
  if(old->owner == who) {
    queue->head = old->next;
    if(wake_owner)
      reply(old->owner, 0);
    if(queue->head)
      reply(queue->head->owner, 0);
    free(old);
  } else {
    reply(who, -EPERM);
  }
}

int enqueue(mutex* m, endpoint_t who) {
  mutex_node* m_node = malloc(sizeof(mutex_node));
  m_node->owner = who;
  m_node->next = NULL;
  mutex_node* cur = m->queue.head;

  if(!cur) {
    m->queue.head = m_node;
    return 1;
  }

  while(cur->next){
    cur = cur->next;
  }

  cur->next = m_node;
  return 0;
}

mutex* create_mutex(int mutex_id) {
  mutex_queue* m_queue = malloc(sizeof(mutex_queue));
  m_queue->head = NULL;
  mutex* m = malloc(sizeof(mutex));
  m->queue = *m_queue;
  m->next = NULL;
  m->id = mutex_id;

  mutex* cur = list.head;
  if(!cur) {
    list.head = m;
  } else {
    while(cur->next)
      cur = cur->next;

    cur->next = m;
  }
  return m;
}

void find_or_create_mutex(int mutex_id, endpoint_t who) {
  mutex* cur = list.head;
  while(cur && cur->id != mutex_id)
    cur = cur->next;
  int res;
  if(!cur)  {
    cur = create_mutex(mutex_id);
  }
  res = enqueue(cur, who);
  if(res == 1) {
    reply(who, 0);
  }

}

int has_mutex(int mutex_id, endpoint_t who) {
  mutex* cur = find_mutex(mutex_id);
  if(!cur || !cur->queue.head)
    return 0;
  if(cur->queue.head->owner == who)
    return 1;
  return 0;
}

int remove_m(endpoint_t who, int kill) {
  mutex* cur = list.head;
  while(cur) {
    mutex_node* current = cur->queue.head;
    mutex_node* previous = NULL;
    while(current) {
      if(current->owner == who && current != cur->queue.head) {
          previous->next = current->next;
          free(current);
          return 1;
      } else if (current->owner == who && kill == 1){
        pop_queue(cur->id, current->owner, 0);
        return 1;
      }
      previous = current;
      current = current->next;
    }

    cur = cur->next;
  }
  return 0;
}

void get_mutex(int mutex_id, endpoint_t who) {
  find_or_create_mutex(mutex_id, who);
}

void release_mutex(int mutex_id, endpoint_t who, int wake_owner) {
  pop_queue(mutex_id, who, wake_owner);
}

void init_m() {
  list.head = NULL;
}