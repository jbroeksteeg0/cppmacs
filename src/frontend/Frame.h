#pragma once

#include "../backend/Buffer.h"
#include "../backend/BufferManager.h"
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
  void load_buffer(std::shared_ptr<Buffer> buffer);
  void scroll_down();
  void scroll_up();
  void scroll_to(int line);
private:
  Window *m_window;
  std::shared_ptr<Buffer> m_buffer;
  int m_x, m_y, m_width, m_height;
  Program m_shader;

  int m_lines_scrolled = 0;
  int m_graphical_lines_cache = 0;
  size_t m_text_start_ind;

  unsigned int m_border_vbo;
  unsigned int m_border_vao;
private:
  void initialise_opengl_data();
  void draw_border(bool is_selected = false);
  void ensure_valid_scroll();

  friend class FrameTree;
  friend class InputManager;
  friend struct __FRAMETREE_IMPL::Node;
  friend class Window;
};
