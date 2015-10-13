#include "reply.h"

typedef struct mutex_node {
  endpoint_t owner;
  struct mutex_node* next;
} mutex_node;

typedef struct mutex_queue {
  mutex_node *head;
} mutex_queue;

typedef struct mutex {
  mutex_queue queue;
  struct mutex* next;
  int id;
} mutex;

typedef struct mutex_list {
  mutex* head;
} mutex_list;


void get_mutex(int id, endpoint_t who);

void release_mutex(int id, endpoint_t who, int wake_owner);

int has_mutex(int mutex_id, endpoint_t who);

void init_m();

int remove_m(endpoint_t who, int kill);
