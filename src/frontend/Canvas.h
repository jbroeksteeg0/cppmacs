#pragma once
#include "Program.h"
#include <array>
#include <chrono>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <vector>

class Window;

struct ColorTheme {
  std::array<float, 3> background = {
    0.176f, 0.164f, 0.18f};
  std::array<float, 3> text = {0.878f, 0.98f, 0.94f};
};

struct Canvas {
public:
  Canvas(Window *ptr);

  void draw_text(
    std::string text,
    float x,
    float y,
    int size,
    std::array<float, 3> color = {1.0f, 1.0f, 1.0f}
  );
  void draw_rectangle(
    float x,
    float y,
    float width,
    float height,
    std::array<float, 3> color = {1.0f, 1.0f, 1.0f}
  );
  void set_ortho_projection(float width, float height);
  void set_matrix_index(size_t index
  );    // TODO: should be private?

  void text_box_init(int x, int y, int width, int height, int lines_scrolled = 0);
  void text_box_write_line(
    const std::string &text,
    int cursor_pos = INT_MAX,
    int size = 32,
    std::array<float, 3> color = {0.0f, 0.0f, 0.0f}
  );
  void text_box_key_pressed();

  bool text_box_is_finished() const;
  std::pair<int, int>
  get_text_dimensions(const std::string &text, int size);
  std::vector<std::string> get_line_broken(const std::string& text, int size = 32);
private:
  Window *m_ptr;

  struct Character {
    unsigned int id;       // text id
    glm::ivec2 size;       // size
    glm::ivec2 bearing;    // offset to TL corner
    long int
      advance;    // horizontal advance to next origin
  };
  std::map<int, std::vector<Character>> m_characters;

  // --------- OPENGL
  unsigned int m_text_VAO, m_text_VBO;
  unsigned int m_rect_VAO, m_rect_VBO;

  Program m_text_program, m_geometry_program;
  glm::mat4 projections[256];
  size_t m_curr_matrix_index = 0;

  // --------- TEXT BOX
  int m_text_box_x, m_text_box_y;
  int m_text_box_width, m_text_box_height;
  int m_text_box_offset_y;
  int m_text_box_lines_skip;

  const int m_line_spacing = 8;
  const int m_horizontal_spacing = 8;

  // --------- CURSOR
  int m_cursor_flash_duration = 800;
  int m_cursor_width = 4;
  std::chrono::milliseconds m_time_text_last_modified;

private:
  void init_text(int size);
  void init_text_buffers();
  void init_geometry_buffers();

  friend class Window;
};
