#pragma once
#include <iostream>
#include "GL/glew.h"
#include "GL/gl.h"

class Program {
public:
  Program();
  Program(std::string vertex, std::string fragment);
  void use();
  const unsigned int get_program() const;
private:
  std::string m_vertex, m_fragment;
  unsigned int m_program;
};
