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
#include "./InputManager.h"
#include "src/backend/Buffer.h"
#include "src/backend/BufferManager.h"
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <vector>

struct Frame;
class FrameTree;
class InputManager;
class BufferManager;

class Window {
public:
  ColorTheme color_theme;
public:
  Window();
  ~Window();
  void run();
  void close();
  void horizontal_split();
  void vertical_split();

  std::shared_ptr<Canvas> get_canvas() const;
  void add_key_combo(std::string combo, std::function<void(Window *window, BufferCursor& cursor)> callback);
  void add_key_combos(std::vector<std::string> combos, std::function<void(Window *window, BufferCursor& cursor)> callback);
private:
  GLFWwindow *m_window;

  std::unique_ptr<FrameTree> m_frame_tree;
  std::unique_ptr<InputManager> m_input_manager;
  std::unique_ptr<BufferManager> m_buffer_manager;
  std::shared_ptr<Canvas> m_canvas;

  int m_width, m_height;
private:
  std::shared_ptr<Buffer> get_active_buffer();
  void swap_buffers();
  friend void key_callback(GLFWwindow *window, int key, int scan_code, int action, int mods);
  friend class InputManager;
  friend class Frame;
};
