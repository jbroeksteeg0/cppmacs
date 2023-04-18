#include "Window.h"
#include "../misc/Logger.h"
#include <ft2build.h>

#include FT_FREETYPE_H

void key_callback(
  GLFWwindow *window, int key, int scan_code, int action, int mods
) {
  Window *this_window = (Window *)glfwGetWindowUserPointer(window);

  if (action == GLFW_PRESS)
    this_window->m_input_manager->press_key(key, mods);
}

Window::Window() {
  glfwInit();

  m_window = glfwCreateWindow(800, 600, "window", nullptr, nullptr);
  m_width = 800;
  m_height = 600;

  glfwMakeContextCurrent(m_window);
  glfwSetWindowUserPointer(m_window, this);
  glfwSetKeyCallback(m_window, key_callback);

  if (glewInit() != GLEW_OK) {
    ERROR("Error initialising glew");
    printf("%s\n", glewGetErrorString(glewInit()));
    exit(1);
  }

  std::shared_ptr<Buffer> temp = std::make_shared<Buffer>();

  m_frame_tree =
    std::make_unique<FrameTree>(this, std::make_unique<Frame>(temp, this));
  // m_frame_tree->create_frame_hsplit(m_frame_tree->m_root.get(), 0.666);
  //   m_frame_tree->create_frame_vsplit(m_frame_tree->m_root->left.get(),
  //   0.666);

  m_canvas = std::make_shared<Canvas>(this);
  m_input_manager = std::make_unique<InputManager>(this);
}

Window::~Window() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

std::shared_ptr<Canvas> Window::get_canvas() const { return m_canvas; }
void Window::run() {
  while (!glfwWindowShouldClose(m_window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // GET WINDOW WIDTH AND HEIGHT
    int window_width, window_height;
    glfwGetWindowSize(m_window, &window_width, &window_height);

    m_frame_tree->update_frame_geometry(0, 0, window_width, window_height);
    m_frame_tree->draw_all_frames();

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }
}

void Window::add_key_combo(std::string combo, std::function<void(Window *window, BufferCursor& cursor)> callback) {
  m_input_manager->add_key_combo(combo, callback);
}

std::shared_ptr<Buffer> Window::get_active_buffer() {
  return m_frame_tree->m_selected->frame->m_buffer;
}
