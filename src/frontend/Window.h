#pragma once
#include "Program.h"
#include "Shaders.h"
#include <GL/gl.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

class Window {
public:
  Window();
  ~Window();
  void run();

private:
  struct Character {
    unsigned int id;       // text id
    glm::ivec2 size;       // size
    glm::ivec2 bearing;    // offset to TL corner
    long int advance;      // horizontal advance to next origin
  };
  std::vector<Character> m_characters;
  GLFWwindow *m_window;
  std::string m_vertex_shader;
  std::string m_fragment_shader;
  Program m_text_program = Program();

  unsigned int m_text_VAO, m_text_VBO;
private:
  void init_freetype();
  void init_text();
  void draw_text(std::string text);
};
