#include "button.h"
#include "raylib.h"
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

int current_time;
const char *current_title;
int title_width;

void ActionStopTimer(Button *b);
void SetButtonStyleStart(Button *b);
void SetButtonStyleStop(Button *b);

void ActionStartTimer(Button *b) {
  SetButtonStyleStop(b);
  current_title = current_time % 2 == 0 ? WORK_TEXT : BREAK_TEXT;
  title_width = MeasureText(current_title, 40);
  timer.time_left = timer.time_sec;
  pthread_create(&thread, NULL, &RunTimer, (void *)(&timer));
}

void ActionStopTimer(Button *b) {
  SetButtonStyleStart(b);
  pthread_cancel(thread);
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

  const int times[] = {1500, 300, 1500, 300, 1500, 300, 1500, 1200};
  const int times_size = ARRAY_SIZE(times);

  current_time = 0;
  current_title = WORK_TEXT;

  char buffer[255];

  InitWindow(screenWidth, screenHeight, "Tomate");
  InitAudioDevice();

  Sound finish_sound = LoadSound("../resources/finish.wav");

  SetTargetFPS(60);

  Button btn = {0};
  Rectangle r = (Rectangle){(float)screenWidth / 2 - 50, y + 100, 100, 25};
  InitButton(&btn, r, START_TEXT, ActionStartTimer);
  SetButtonColors(&btn, GREEN, LIME, GREEN);

  InitTimer(&timer, times[current_time]);

  title_width = MeasureText(current_title, 40);

  while (!WindowShouldClose()) {
    int time_left = TimerTimeLeft(&timer);
    if (time_left <= 0 && timer.state == RUNNING) {
      pthread_join(thread, NULL);
      timer.state = FINISHED;

      PlaySound(finish_sound);
      SetButtonStyleStart(&btn);
      current_time = (current_time + 1) % times_size;
      timer.time_sec = times[current_time];
    }

    HandleButton(&btn);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawButton(&btn);

    int mins = (time_left / 60);
    int secs = time_left % 60;

    DrawText(current_title, (screenWidth / 2) - (title_width / 2), y + 10, 40,
             btn.hcolor);

    snprintf(buffer, 255, "%02d:%02d", mins, secs);
    int time_width = MeasureText(buffer, 40);
    DrawText(buffer, screenWidth / 2 - time_width / 2, y + 50, 40, BLACK);

    EndDrawing();
  }

  UnloadSound(finish_sound);
  CloseAudioDevice();
  CloseWindow();

  return 0;
}
