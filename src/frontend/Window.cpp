#include "Window.h"
#include "../misc/Logger.h"
#include <ft2build.h>

#include FT_FREETYPE_H

Window::Window() {
  glfwInit();
  m_window = glfwCreateWindow(800, 600, "window", nullptr, nullptr);
  glfwMakeContextCurrent(m_window);

  if (glewInit() != GLEW_OK) {
    ERROR("Error initialising glew");
    printf("%s\n", glewGetErrorString(glewInit()));
    exit(1);
  }

  std::shared_ptr<Buffer> temp = std::make_shared<Buffer>();

  m_frame_tree = std::make_unique<FrameTree>(this, std::make_unique<Frame>(temp, this));
  m_frame_tree->create_frame_hsplit(m_frame_tree->m_root.get(), 0.666);
  m_frame_tree->create_frame_vsplit(m_frame_tree->m_root->left.get(), 0.666);

  m_canvas = std::make_shared<Canvas>(this);
}

Window::~Window() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

std::shared_ptr<Canvas> Window::get_canvas() const {
  return m_canvas;
}
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





/*
void Window::set_ortho_projection(float width, float height) {
  glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height);
  glUniformMatrix4fv(
    glGetUniformLocation(m_text_program.get_program(), "projection"),
    1,
    GL_FALSE,
    glm::value_ptr(projection)
  );
}
*/
