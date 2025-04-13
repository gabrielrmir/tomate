#include "task.h"
#include <assert.h>
#include <stdlib.h>

Task *NewTask(int time_sec, const char *title) {
  Task *t = (Task *)malloc(sizeof(Task));

  t->time_sec = time_sec;
  t->title = title;

  t->prev = NULL;
  t->next = NULL;

  return t;
}

Task *NewTasks(int *times, size_t times_size, const char **titles,
               size_t titles_size) {
  assert(times_size == titles_size);
  assert(times_size > 0);

  Task *head = NewTask(*times++, *titles++);
  Task *start = head;

  int i = times_size;
  while (--i > 0) {
    Task *t = NewTask(*times++, *titles++);
    head->next = t;
    t->prev = head;
    head = t;
  }
  head->next = start;
  start->prev = head;

  return start;
}

void FreeTasks(Task *t) {
  while (t != NULL) {
    Task *n = t->next;
    Task *p = t->prev;

    free(t);

    if (p != NULL)
      p->next = NULL;

    if (n != NULL)
      n->prev = NULL;

    t = n;
  }
}
