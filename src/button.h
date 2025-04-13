#ifndef _BUTTON_H
#define _BUTTON_H

#include "raylib.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct Button {
  Rectangle rect;
  const char *text;
  void (*action)(struct Button *);

  bool pressed;
  bool last_pressed;

  Color color;
  Color hcolor;
  Color pcolor;
  Color bcolor;
} Button;

bool IsButtonHovered(Button *button);
bool IsButtonDown(Button *button);
void InitButton(Button *button, Rectangle r, const char *text,
                void (*action)(Button *b));
void SetButtonColors(Button *b, Color c, Color h, Color p);
void HandleButton(Button *button);
void DrawButton(Button *button);

#endif // _BUTTON_H
