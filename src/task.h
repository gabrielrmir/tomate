#ifndef _TASK_H
#define _TASK_H

#include <stdlib.h>

typedef struct Task {
  const char *title;
  int time_sec;

  struct Task *prev;
  struct Task *next;
} Task;

Task *NewTask(int time_sec, const char *title);
Task *NewTasks(int *times, size_t times_size, const char **titles,
               size_t titles_size);
void FreeTasks(Task *t);

#endif // _TASK_H
