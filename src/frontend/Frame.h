#pragma once

#include "../backend/Buffer.h"
#include "Window.h"
#include <memory>

class Window;

class Frame {
public:
  Frame(std::shared_ptr<Buffer> buffer, Window *parent);
  void draw();

private:
  Window *m_window;
  std::shared_ptr<Buffer> m_buffer;
  int m_x, m_y, m_width, m_height;

private:
  void update_dimensions();

  friend class FrameTree;
};
