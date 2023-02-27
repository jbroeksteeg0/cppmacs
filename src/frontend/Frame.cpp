#include "Frame.h"

Frame::Frame(Window *parent) : m_window(parent) {
  update_dimensions();
}

void Frame::draw() {
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
