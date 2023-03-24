#pragma once
#include "Canvas.h"
#include "Frame.h"
#include "FrameTree.h"
#include "Program.h"
#include "Shaders.h"
#include <GL/gl.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <vector>

struct Frame;
class FrameTree;

class Window {
public:
  Window();
  ~Window();
  void run();

  std::shared_ptr<Canvas> get_canvas() const;
private:
  GLFWwindow *m_window;

  std::unique_ptr<FrameTree> m_frame_tree;
  std::shared_ptr<Canvas> m_canvas;
};
