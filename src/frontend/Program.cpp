#include "Program.h"
#include "../misc/Logger.h"

Program::Program() : m_vertex(""), m_fragment("") {}

// TODO: memory leak with shaders
Program::Program(std::string vertex, std::string fragment)
    : m_vertex(vertex), m_fragment(fragment) {
  // compile vertex
  unsigned int vertex_id = glCreateShader(GL_VERTEX_SHADER);

  const char *const vertex_src = m_vertex.c_str();
  glShaderSource(vertex_id, 1, &vertex_src, nullptr);
  glCompileShader(vertex_id);

  int success;
  char err[512];
  glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_id, 512, NULL, err);
    ERROR(err);
    exit(1);
  }

  // compile fragment
  unsigned int fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
  const char *const fragment_src = m_fragment.c_str();
  glShaderSource(fragment_id, 1, &fragment_src, nullptr);
  glCompileShader(fragment_id);

  glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_id, 512, NULL, err);
    ERROR(err);
    exit(1);
  }

  // setup program
  m_program = glCreateProgram();
  glAttachShader(m_program, vertex_id);
  glAttachShader(m_program, fragment_id);
  glLinkProgram(m_program);
  // check for linking errors
  glGetProgramiv(m_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(m_program, 512, NULL, err);
    ERROR(err);
    exit(1);
  }

  glDeleteShader(vertex_id);
  glDeleteShader(fragment_id);
}

void Program::use() {
  if (m_vertex == "" || m_fragment == "") {
    ERROR("Attempting to use uninitialised program");
    exit(1);
  }
  glUseProgram(m_program);
}

const unsigned int Program::get_program() const { return m_program; }
