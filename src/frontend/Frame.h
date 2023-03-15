#pragma once

#include "FrameTree.h"
#include "Window.h"

class Window;

struct Frame {
  Frame(Window *parent);
  void draw();

  Window *m_window;
  int m_x, m_y, m_width, m_height;
};
