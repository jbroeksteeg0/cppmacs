#include "Frame.h"
#include "Shaders.h"

Frame::Frame(Window *parent) : m_window(parent) {
  glViewport(m_x, m_y, m_width, m_height);
  initialise_opengl_data();
}
Frame::Frame(std::shared_ptr<Buffer> buffer, Window *parent)
    : m_window(parent), m_buffer(buffer) {
  glViewport(m_x, m_y, m_width, m_height);
  m_buffer->m_current_frame = this;

  initialise_opengl_data();
}

void Frame::load_buffer(std::shared_ptr<Buffer> buffer) {
  m_buffer = buffer;
  m_buffer->m_current_frame = this;
  m_lines_scrolled = 0;
}

void Frame::draw() {
  glViewport(m_x, m_y, m_width, m_height);
  m_window->get_canvas()->set_ortho_projection(m_width, m_height);

  std::shared_ptr<Canvas> canvas = m_window->get_canvas();

  m_window->get_canvas()->draw_rectangle(
    0, 0, m_width, m_height, this->m_window->color_theme.background
  );

  draw_border(this == m_window->m_frame_tree->m_selected->frame.get());

  ensure_valid_scroll();

  if (!m_buffer->is_minibuffer()) {
    canvas->text_box_init(0, 28, m_width, m_height - 28, m_lines_scrolled);
    std::string file_text = m_buffer->m_file.has_value()
                              ? m_buffer->m_file->string()
                              : "Scratch Buffer";
    canvas->draw_rectangle(
      0.0f, 0.0f, (float)m_width, 28.0f, m_window->color_theme.text
    );
    canvas->draw_text(
      "Open: " + file_text, 8, 8, 18, m_window->color_theme.background
    );
  } else {
    m_lines_scrolled = 0;
    canvas->text_box_init(0, 6, m_width, m_height, m_lines_scrolled);
  }

  int cursor_pos = m_buffer->get_cursor_position();

  for (std::string s : m_buffer->get_text()) {
    canvas->text_box_write_line(s, cursor_pos, 24, m_window->color_theme.text);

    if (cursor_pos <= (int)s.size()) {
      cursor_pos = INT_MAX;
    } else {
      cursor_pos -= s.size() + 1;
    }
  }
}

void Frame::initialise_opengl_data() {
  glGenBuffers(1, &this->m_border_vbo);
  glGenBuffers(1, &this->m_border_vbo);

  m_shader = Program(standard_vertex_shader, standard_fragment_shader);
}

void Frame::draw_border(bool is_selected) {
  auto canvas = m_window->get_canvas();

  const float thickness = 2.0f * (is_selected ? 3.0f : 1.0f);

  canvas->draw_rectangle(
    0.0f, 0.0f, thickness, (float)m_height, this->m_window->color_theme.text
  );
  canvas->draw_rectangle(
    (float)m_width - thickness,
    0.0f,
    thickness,
    (float)m_height,
    this->m_window->color_theme.text
  );

  canvas->draw_rectangle(
    0.0f,
    (float)m_height - thickness,
    (float)m_width,
    thickness,
    this->m_window->color_theme.text
  );
  canvas->draw_rectangle(
    0.0f, 0.0f, (float)m_width, thickness, this->m_window->color_theme.text
  );
}

void Frame::ensure_valid_scroll() {
  std::shared_ptr<Canvas> canvas = m_window->get_canvas();

  int num_graphical_lines = 0;
  for (const std::string &line : m_buffer->get_text()) {
    num_graphical_lines += canvas->get_line_broken(line).size();
  }

  m_graphical_lines_cache = num_graphical_lines;
  if (m_graphical_lines_cache == 0) {
    m_lines_scrolled = 0;
  } else if (m_lines_scrolled + 1 >= m_graphical_lines_cache) {
    m_lines_scrolled = m_graphical_lines_cache - 1;
  }

  // find the lowest graphical line that is still rendered
  int lowest_graphical = INT_MAX;
  int y_offset = 0;
  int cursor_line = 0;
  int chars_to_cursor = m_buffer->get_cursor_position();
  int graphical_line_ind = 0;
  for (const std::string &line : m_buffer->get_text()) {
    for (const std::string &gline : canvas->get_line_broken(line)) {
      if (graphical_line_ind >= m_lines_scrolled)
        y_offset += canvas->get_text_dimensions(gline, 32).second;

      if (chars_to_cursor >= 0 && chars_to_cursor < ((int)gline.size()) + 1) {
        cursor_line = graphical_line_ind;
        chars_to_cursor = -1;
      } else {
        chars_to_cursor -= gline.size() + 1;
      }

      if (y_offset + 32 >= m_height) {
        lowest_graphical = graphical_line_ind;
        break;
      }

      graphical_line_ind++;
    }
  }

  lowest_graphical--;

  if (cursor_line > lowest_graphical) {
    m_lines_scrolled++;
  }

  if (cursor_line < m_lines_scrolled) {
    m_lines_scrolled--;
  }
}

void Frame::scroll_down() {
  if (m_lines_scrolled + 1 < m_graphical_lines_cache) {
    m_lines_scrolled++;
  }
}

void Frame::scroll_up() {
  if (m_lines_scrolled > 0) {
    m_lines_scrolled--;
  }
}

void Frame::scroll_to(int line) {
  m_lines_scrolled = line;
  ensure_valid_scroll();
}
