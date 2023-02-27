#pragma once

#include "Window.h"

class Window;

class Frame {
public:
  Frame(Window *parent);
  void draw();
private:
  Window *m_window;
  int m_x, m_y, m_width, m_height;
private:
  void update_dimensions();
};
