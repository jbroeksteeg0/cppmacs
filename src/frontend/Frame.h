#pragma once

#include "FrameTree.h"
#include "../backend/Buffer.h"
#include "Window.h"
#include <memory>

class Window;

struct Frame {
  Frame(Window *parent);
  void draw();

  Frame(std::shared_ptr<Buffer> buffer, Window *parent);

  Window *m_window;
  std::shared_ptr<Buffer> m_buffer;

  int m_x, m_y, m_width, m_height;

  friend class FrameTree;
};
