#include "Frame.h"
#include "Shaders.h"

Frame::Frame(Window *parent) : m_window(parent) {
  initialise_opengl_data();
}
Frame::Frame(std::shared_ptr<Buffer> buffer, Window *parent)
    : m_window(parent), m_buffer(buffer) {
  initialise_opengl_data();
}

void Frame::draw() {
  glViewport(m_x, m_y, m_width, m_height);

  m_window->get_canvas()->set_ortho_projection(
    m_width, m_height
  );

  draw_border();

  m_window->get_canvas()->draw_text("hi from bottom", 0, 0);
}

void Frame::initialise_opengl_data() {
  glGenBuffers(1, &this->m_border_vbo);
  glGenBuffers(1, &this->m_border_vbo);

  m_shader = Program(
    standard_vertex_shader, standard_fragment_shader
  );
}

void Frame::draw_border() {
  auto canvas = m_window->get_canvas();

  const float thickness = 1.0f;

  canvas->draw_rectangle(
    0.0f, 0.0f, thickness, (float)m_height
  );
  canvas->draw_rectangle(
    (float)m_width - thickness,
    0.0f,
    thickness,
    (float)m_height
  );

  canvas->draw_rectangle(0.0f, (float)m_height - thickness, (float)m_width, thickness);
  canvas->draw_rectangle(0.0f, 0.0f, (float)m_width, thickness);
  
}
