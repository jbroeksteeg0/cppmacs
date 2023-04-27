#include "Frame.h"
#include "Shaders.h"

Frame::Frame(Window *parent) : m_window(parent) {
  glViewport(m_x, m_y, m_width, m_height);
  initialise_opengl_data();
}
Frame::Frame(std::shared_ptr<Buffer> buffer, Window *parent)
    : m_window(parent), m_buffer(buffer) {
  glViewport(m_x, m_y, m_width, m_height);

  initialise_opengl_data();
}

void Frame::draw() {
  glViewport(m_x, m_y, m_width, m_height);

  m_window->get_canvas()->set_ortho_projection(
    m_width, m_height
  );

  draw_border();
  std::shared_ptr<Canvas> canvas = m_window->get_canvas();

  m_window->get_canvas()->text_box_init(m_x,m_y,m_width,m_height);

  int cursor_pos = m_buffer->get_cursor_position();

  for (std::string s: m_buffer->get_text()) {
    m_window->get_canvas()->text_box_write_line(s, cursor_pos);

    if (cursor_pos <= s.size()) {
      cursor_pos = INT_MAX;
    } else {
      cursor_pos -= s.size()+1;
    }
  }
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
