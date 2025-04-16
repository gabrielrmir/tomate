#include "flags.h"
#include "task.h"
#include <ctype.h>
#include <string.h>

#define ARRAY_SIZE(arr) sizeof(arr) / sizeof(*arr)
#define WORK_TIME 1500
#define BREAK_TIME 300
#define LONG_BREAK_TIME 1200

void LoadDefaultFlags(Context *ctx) {
  ctx->dark = false;
  ctx->task = NULL;
  ctx->sound_path = "../resources/finish.wav";
}

void strtoup(char *str) {
  while (*str) {
    *str = toupper(*str);
    str++;
  }
}

void InitContext(int argc, char **argv, Context *ctx) {
  if (argc == 0)
    return;
  argc--;
  argv++;

  LoadDefaultFlags(ctx);

  Task *head = NULL;
  int tasks = 0;

  while (argc-- > 0) {
    char *arg = *argv++;
    int l = strlen(arg);
    if (*arg == '-') {
      if (strcmp(arg, "-d") == 0) {
        ctx->dark = true;
      } else if (strcmp(arg, "-s") == 0) {
        if (argc-- == 0)
          exit(1);
        char *path = *argv++;
        ctx->sound_path = path;
      }
    } else {
      strtoup(arg);
      Task *t = NewTask(WORK_TIME, arg);
      if (head == NULL) {
        ctx->task = t;
        head = t;
      } else {
        head->next = t;
        t->prev = head;
        head = t;
      }

      int time_sec =
          (tasks != 0 && tasks % 3 == 0) ? LONG_BREAK_TIME : BREAK_TIME;
      tasks++;

      Task *b = NewTask(time_sec, "BREAK");
      t->next = b;
      b->prev = t;
      head = b;
    }
  }

  if (ctx->task == NULL) {
    const char *titles[8];
    for (int i = 0; i < 8; i++) {
      titles[i] = i % 2 == 0 ? "WORK" : "BREAK";
    }

    int times[8];
    for (int i = 0; i < 8; i++) {
      if (i != 0 && i % 7 == 0) {
        times[i] = LONG_BREAK_TIME;
        continue;
      }
      times[i] = i % 2 == 0 ? WORK_TIME : BREAK_TIME;
    }

    ctx->task = NewTasks(times, ARRAY_SIZE(times), titles, ARRAY_SIZE(titles));
  } else {
    ctx->task->prev = head;
    head->next = ctx->task;
  }
}
