#pragma once
#include "./Window.h"
#include <functional>
#include <iostream>
#include <vector>

struct Frame;
struct GLFWwindow;
class Window;
struct BufferCursor;

class InputManager {
public:
  InputManager(Window *parent);

  void add_key_combo(
    std::string combo,
    std::function<
      void(Window *window, BufferCursor &cursor, Frame *)>
      function
  );
  void press_key(int key_code, int mods);

private:
  std::vector<std::pair<
    std::string,
    std::function<
      void(Window *window, BufferCursor &cursor, Frame *)>>>
    m_candidates;
  std::string m_current_combo;
  Window *m_parent;

  friend void key_callback(
    GLFWwindow *window,
    int key,
    int scan_code,
    int action,
    int mods
  );
};
