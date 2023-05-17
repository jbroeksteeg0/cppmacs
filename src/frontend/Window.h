#pragma once
#include "FrameTree.h"
#include "./InputManager.h"
#include "Frame.h"
#include "Canvas.h"
#include "Frame.h"
#include "FrameTree.h"
#include "Program.h"
#include "Shaders.h"
#include "src/backend/Buffer.h"
#include "src/backend/BufferManager.h"
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
  void change_selected_frame_up();
  void change_selected_frame_down();
  void change_selected_frame_left();
  void change_selected_frame_right();
  void close_current_frame();

  void add_key_combo(
    std::string combo,
    std::function<void(
      Window *window, BufferCursor &cursor, Frame *frame
    )> callback
  );
  void add_key_combos(
    std::vector<std::string> combos,
    std::function<void(
      Window *window, BufferCursor &cursor, Frame *frame
    )> callback
  );

  void open_minibuffer();
  void minibuffer_return();
  void process_minibuffer_command(std::string command);
  bool in_minibuffer() const;
  std::shared_ptr<Canvas> get_canvas() const;
private:
  GLFWwindow *m_window;

  std::unique_ptr<FrameTree> m_frame_tree;
  std::unique_ptr<InputManager> m_input_manager;
  std::unique_ptr<BufferManager> m_buffer_manager;
  std::shared_ptr<Canvas> m_canvas;

  int m_width, m_height;

  bool m_in_minibuffer = false;
  std::unique_ptr<Frame> m_minibuffer;
private:
  std::shared_ptr<Buffer> get_active_buffer();

  template<typename T> T use_active_frame(std::function<T(Frame*)> func) {
    return func(m_frame_tree->m_selected->frame.get());
  }

  void swap_buffers();
  friend void key_callback(
    GLFWwindow *window,
    int key,
    int scan_code,
    int action,
    int mods
  );
  friend class InputManager;
  friend struct Frame;
  friend class Buffer;
};
