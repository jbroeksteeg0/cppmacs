#pragma once
#include "Program.h"
#include <array>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

class Window;

struct Canvas {
public:
  Canvas(Window *ptr);

  void draw_text(
    std::string text,
    float x,
    float y,
    std::array<float, 3> color = {1.0f,1.0f,1.0f}
  );
  void draw_rectangle(float x, float y, float width, float height);
  void set_ortho_projection(float width, float height);
private:
  Window *m_ptr;

  struct Character {
    unsigned int id;       // text id
    glm::ivec2 size;       // size
    glm::ivec2 bearing;    // offset to TL corner
    long int
      advance;    // horizontal advance to next origin
  };
  std::vector<Character> m_characters;

  unsigned int m_text_VAO, m_text_VBO;
  unsigned int m_rect_VAO, m_rect_VBO;

  Program m_text_program, m_geometry_program;

private:
  void init_text();
  void init_geometry_buffers();
};
