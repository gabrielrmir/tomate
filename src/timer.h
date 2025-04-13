#ifndef _TIMER_H
#define _TIMER_H

#include <pthread.h>
#include <stdbool.h>

typedef enum TimerState { IDLE, RUNNING, PAUSED, FINISHED } TimerState;

typedef struct Timer {
  int time_sec;

  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int time_left;

  TimerState state;
} Timer;

void InitTimer(Timer *t, int secs);
int TimerTimeLeft(Timer *t);
void *RunTimer(void *arg);

#endif // _TIMER_H
