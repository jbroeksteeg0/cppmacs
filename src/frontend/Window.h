#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

class Window {
public:
  Window();
  ~Window();
  void run();

private:
  struct Character {
    unsigned int id;         // text id
    glm::ivec2 size;         // size
    glm::ivec2 bearing;      // offset to TL corner
    long int advance;    // horizontal advance to next origin
  };
  std::vector<Character> m_characters;
  GLFWwindow *window;
  std::string m_vertex_shader;
  std::string m_fragment_shader;
  void init_freetype();
};
