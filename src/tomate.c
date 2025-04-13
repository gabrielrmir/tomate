#include "button.h"
#include "raylib.h"
#include "task.h"
#include "timer.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define ARRAY_SIZE(arr) sizeof(arr) / sizeof(*arr)

const char *START_TEXT = "Start";
const char *STOP_TEXT = "Stop";
const char *WORK_TEXT = "WORK";
const char *BREAK_TEXT = "BREAK";

pthread_t thread;
Timer timer;

int title_width;
Task *current_task;

void ActionStopTimer(Button *b);
void SetButtonStyleStart(Button *b);
void SetButtonStyleStop(Button *b);

void ActionStartTimer(Button *b) {
  SetButtonStyleStop(b);

  pthread_mutex_lock(&timer.mutex);

  if (timer.state == FINISHED || timer.state == IDLE) {
    if (timer.state == FINISHED)
      current_task = current_task->next;

    timer.time_left = current_task->time_sec;
    title_width = MeasureText(current_task->title, 40);

    pthread_create(&thread, NULL, &RunTimer, (void *)(&timer));
  } else if (timer.state == PAUSED) {
    pthread_cond_signal(&timer.cond);
  }
  timer.state = RUNNING;

  pthread_mutex_unlock(&timer.mutex);
}

void ActionStopTimer(Button *b) {
  SetButtonStyleStart(b);
  pthread_mutex_lock(&timer.mutex);
  timer.state = PAUSED;
  pthread_mutex_unlock(&timer.mutex);
}

void SetButtonStyleStart(Button *b) {
  b->action = ActionStartTimer;
  b->text = START_TEXT;
  SetButtonColors(b, GREEN, LIME, GREEN);
}

void SetButtonStyleStop(Button *b) {
  b->action = ActionStopTimer;
  b->text = STOP_TEXT;
  SetButtonColors(b, RED, MAROON, RED);
}

int main(void) {

  const int screenWidth = 300;
  const int screenHeight = 200;
  const int y = 30;

  int times[] = {1500, 300, 1500, 300, 1500, 300, 1500, 1200};
  const char *titles[] = {
      WORK_TEXT, BREAK_TEXT, WORK_TEXT, BREAK_TEXT,
      WORK_TEXT, BREAK_TEXT, WORK_TEXT, BREAK_TEXT,
  };

  current_task = NewTasks(times, ARRAY_SIZE(times), titles, ARRAY_SIZE(titles));

  char buffer[255];

  InitWindow(screenWidth, screenHeight, "Tomate");
  InitAudioDevice();

  Sound finish_sound = LoadSound("../resources/finish.wav");

  SetTargetFPS(60);

  Button btn = {0};
  Rectangle r = (Rectangle){(float)screenWidth / 2 - 50, y + 100, 100, 25};
  InitButton(&btn, r, START_TEXT, ActionStartTimer);
  SetButtonColors(&btn, GREEN, LIME, GREEN);

  InitTimer(&timer, current_task->time_sec);

  title_width = MeasureText(current_task->title, 40);

  while (!WindowShouldClose()) {
    int time_left = TimerTimeLeft(&timer);
    if (time_left <= 0 && timer.state == RUNNING) {
      pthread_join(thread, NULL);
      timer.state = FINISHED;

      PlaySound(finish_sound);
      SetButtonStyleStart(&btn);
    }

    HandleButton(&btn);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawButton(&btn);

    int mins = (time_left / 60);
    int secs = time_left % 60;

    DrawText(current_task->title, (screenWidth / 2) - (title_width / 2), y + 10,
             40, btn.hcolor);

    // Shows grayed out titles of previous and next tasks

    // DrawText(current_task->next->title,
    //         (screenWidth / 2) + (title_width / 2) + 10, y + 20, 20,
    //         LIGHTGRAY);

    // TODO: align text to the right
    // DrawText(current_task->prev->title,
    //          (screenWidth / 2) - (title_width / 2) - 80, y + 20, 20,
    //          LIGHTGRAY);

    snprintf(buffer, 255, "%02d:%02d", mins, secs);
    int time_width = MeasureText(buffer, 40);
    DrawText(buffer, screenWidth / 2 - time_width / 2, y + 50, 40, BLACK);

    EndDrawing();
  }

  FreeTasks(current_task);

  UnloadSound(finish_sound);
  CloseAudioDevice();

  CloseWindow();

  return 0;
}
