#include "button.h"
#include "raylib.h"
#include "timer.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

const char *START_TEXT = "Start";
const char *STOP_TEXT = "Stop";

pthread_t thread;
Timer timer;

void ActionStopTimer(Button *b);

void ActionStartTimer(Button *b) {
  b->action = ActionStopTimer;
  b->text = STOP_TEXT;
  SetButtonColors(b, RED, MAROON, RED);

  timer.time_left = timer.time_sec;

  pthread_create(&thread, NULL, &RunTimer, (void *)(&timer));
}

void ActionStopTimer(Button *b) {
  b->action = ActionStartTimer;
  b->text = START_TEXT;
  SetButtonColors(b, GREEN, LIME, GREEN);
  pthread_cancel(thread);
  pthread_mutex_lock(&(timer.mutex));
  pthread_mutex_unlock(&(timer.mutex));
}

int main(void) {

  const int screenWidth = 300;
  const int screenHeight = 200;
  const int y = 30;

  char buffer[255];

  InitWindow(screenWidth, screenHeight, "Tomate");

  SetTargetFPS(60);

  Button btn = {0};
  Rectangle r = (Rectangle){(float)screenWidth / 2 - 50, y + 100, 100, 25};
  InitButton(&btn, r, START_TEXT, ActionStartTimer);
  SetButtonColors(&btn, GREEN, LIME, GREEN);

  InitTimer(&timer, 25 * 60);

  int title_width = MeasureText("TOMATE", 40);

  while (!WindowShouldClose()) {
    int time_left = TimerTimeLeft(&timer);
    if (time_left <= 0 && timer.state == RUNNING) {
      pthread_join(thread, NULL);
      timer.state = FINISHED;
      printf("Timer finished\n");
    }

    HandleButton(&btn);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawButton(&btn);

    int mins = (time_left / 60);
    int secs = time_left % 60;

    DrawText("TOMATE", (screenWidth / 2) - (title_width / 2), y + 10, 40,
             btn.hcolor);

    snprintf(buffer, 255, "%02d:%02d", mins, secs);
    int time_width = MeasureText(buffer, 40);
    DrawText(buffer, screenWidth / 2 - time_width / 2, y + 50, 40, BLACK);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
