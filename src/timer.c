#include "timer.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

void InitTimer(Timer *t, int secs) {
  pthread_mutex_init(&(t->mutex), NULL);
  t->time_sec = secs;
  t->time_left = secs;
  t->state = IDLE;
}

int TimerTimeLeft(Timer *t) {
  int time_left;
  pthread_mutex_lock(&(t->mutex));
  time_left = t->time_left;
  pthread_mutex_unlock(&(t->mutex));
  return time_left;
}

void *RunTimer(void *arg) {
  Timer *t = (Timer *)arg;

  pthread_mutex_lock(&t->mutex);
  t->state = RUNNING;
  pthread_mutex_unlock(&t->mutex);

  while (t->time_left > 0) {
    sleep(1);
    pthread_mutex_lock(&t->mutex);
    while (t->state == PAUSED) {
      pthread_cond_wait(&t->cond, &t->mutex);
    }
    t->time_left--;
    pthread_mutex_unlock(&t->mutex);
  }

  pthread_exit(NULL);
}
