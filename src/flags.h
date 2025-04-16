#ifndef _FLAGS_H
#define _FLAGS_H

#include "task.h"
#include <stdbool.h>

typedef struct Context {
  char *sound_path;
  bool dark;
  Task *task;
} Context;

void InitContext(int argc, char **argv, Context *ctx);

#endif // _FLAGS_H
