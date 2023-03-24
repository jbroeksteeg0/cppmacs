#pragma once

#include "../backend/Buffer.h"
#include "FrameTree.h"
#include "Program.h"
#include "Window.h"
#include <memory>

class Window;
namespace __FRAMETREE_IMPL {
struct Node;
};

struct Frame {
  Frame(Window *parent);
  Frame(std::shared_ptr<Buffer> buffer, Window *parent);

  void draw();

private:
  Window *m_window;
  std::shared_ptr<Buffer> m_buffer;
  int m_x, m_y, m_width, m_height;
  Program m_shader;

  unsigned int m_border_vbo;
  unsigned int m_border_vao;

private:
  void initialise_opengl_data();
  void draw_border();

  friend class FrameTree;
  friend struct __FRAMETREE_IMPL::Node;
};
