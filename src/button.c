#include "button.h"
#include <raylib.h>

bool IsButtonHovered(Button *button) {
  return CheckCollisionPointRec(GetMousePosition(), button->rect);
}

bool IsButtonDown(Button *button) {
  return IsButtonHovered(button) && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
}

void InitButton(Button *button, Rectangle r, const char *text,
                void (*action)(Button *b)) {
  button->pressed = false;
  button->last_pressed = false;
  button->color = LIGHTGRAY;
  button->hcolor = WHITE;
  button->pcolor = GRAY;
  button->rect = r;
  button->text = text;
  button->action = action;
}

void SetButtonColors(Button *b, Color c, Color h, Color p) {
  b->color = c;
  b->hcolor = h;
  b->pcolor = p;
}

void HandleButton(Button *button) {
  bool hover = IsButtonHovered(button);
  button->last_pressed = button->pressed;
  button->pressed = hover && IsMouseButtonDown(MOUSE_BUTTON_LEFT);

  if (button->pressed && button->last_pressed != button->pressed) {
    button->action(button);
  }
}

void DrawButton(Button *button) {
  if (button->pressed) {
    DrawRectangleRec(button->rect, button->pcolor);
  } else if (IsButtonHovered(button)) {
    DrawRectangleRec(button->rect, button->hcolor);
  } else {
    DrawRectangleRec(button->rect, button->color);
  }

  DrawRectangleLinesEx(button->rect, 2, BLACK);

  int width = MeasureText(button->text, 20);
  DrawText(button->text,
           button->rect.x + button->rect.width / 2 - (float)width / 2,
           button->rect.y + (button->rect.height - 20) / 2, 20, BLACK);
}
