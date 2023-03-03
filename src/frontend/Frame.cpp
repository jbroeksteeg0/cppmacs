#include "Frame.h"

Frame::Frame(Window *parent) : m_window(parent) {
  update_dimensions();
}

void Frame::draw() {
  update_dimensions();
  
  m_window->set_ortho_projection(m_width, m_height);
  m_window->draw_text("hi from bottom", 0, 0);
}

void Frame::update_dimensions() {
  int data[4];
  glGetIntegerv(GL_VIEWPORT, data);
  m_x = data[0];
  m_y = data[1];
  m_width = data[2];
  m_height = data[3];
}

