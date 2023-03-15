#include "Frame.h"

Frame::Frame(Window *parent) : m_window(parent) {}
Frame::Frame(std::shared_ptr<Buffer> buffer, Window *parent): m_window(parent), m_buffer(buffer) {}

void Frame::draw() {
  glViewport(m_x, m_y, m_width, m_height);
  m_window->set_ortho_projection(m_width, m_height);
  m_window->draw_text("hi from bottom", 0, 0);
}
