#include "Window.h"
#include "../misc/Logger.h"
#include "src/backend/Buffer.h"
#include "src/frontend/Frame.h"
#include "src/frontend/InputManager.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <ft2build.h>
#include <memory>

#include FT_FREETYPE_H

void key_callback(
  GLFWwindow *window,
  int key,
  int scan_code,
  int action,
  int mods
) {
  Window *this_window =
    (Window *)glfwGetWindowUserPointer(window);

  if (action == GLFW_PRESS || action == GLFW_REPEAT)
    this_window->m_input_manager->press_key(key, mods);
}

Window::Window() {
  glfwInit();

  m_window =
    glfwCreateWindow(800, 600, "window", nullptr, nullptr);
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
  m_buffer_manager = std::make_unique<BufferManager>(this);

  m_frame_tree = std::make_unique<FrameTree>(
    this,
    std::make_unique<Frame>(
      m_buffer_manager->get_scratch_buffer(), this
    )
  );
  m_canvas = std::make_shared<Canvas>(this);

  m_input_manager = std::make_unique<InputManager>(this);

  m_minibuffer = std::make_unique<Frame>(
    std::make_shared<Buffer>(this), this
  );
}

Window::~Window() {
  m_minibuffer->m_buffer->close();
  
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

std::shared_ptr<Canvas> Window::get_canvas() const {
  return m_canvas;
}
void Window::run() {
  while (!glfwWindowShouldClose(m_window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // GET WINDOW WIDTH AND HEIGHT
    int window_width, window_height;
    glfwGetWindowSize(
      m_window, &window_width, &window_height
    );
    if (m_in_minibuffer) {
      m_frame_tree->update_frame_geometry(
        0, 32, window_width, window_height - 32
      );
    } else {
      m_frame_tree->update_frame_geometry(
        0, 0, window_width, window_height
      );
    }
    m_frame_tree->draw_all_frames();

    if (m_in_minibuffer) {
      m_minibuffer->m_x = 0;
      m_minibuffer->m_width = window_width;
      m_minibuffer->m_y = 0;
      m_minibuffer->m_height = 32;
      m_minibuffer->draw();
    }

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }
}

void Window::close() {
  glfwSetWindowShouldClose(this->m_window, GL_TRUE);
}

void Window::horizontal_split() {
  m_frame_tree->create_frame_hsplit(
    m_frame_tree->m_selected, 0.5
  );
}

void Window::vertical_split() {
  m_frame_tree->create_frame_vsplit(
    m_frame_tree->m_selected, 0.5
  );
}

void Window::change_selected_buffer_up() {
  using namespace __FRAMETREE_IMPL;
  std::vector<Node *> all_frames =
    m_frame_tree->get_all_frames();

  Node *take = nullptr,
       *selected = m_frame_tree->m_selected;

  for (Node *node : all_frames) {
    // If the node is in correct position
    if (node->y >= selected->y + selected->height
	&& node->x <= selected->x
	&& node->x+node->width >=  selected->x) {
      // If it is lower than other canditates
      if (take == nullptr || node->y < take->y) {
        take = node;
      }
    }
  }

  if (take != nullptr) {
    m_frame_tree->m_selected = take;
  }
}
void Window::change_selected_buffer_down() {
  using namespace __FRAMETREE_IMPL;
  std::vector<Node *> all_frames =
    m_frame_tree->get_all_frames();

  Node *take = nullptr,
       *selected = m_frame_tree->m_selected;

  for (Node *node : all_frames) {
    if (node == selected)
      continue;
    // If the node is in correct position
    if (node->frame->m_y + node->frame->m_height <= selected->frame->m_y
	&& node->frame->m_x <= selected->frame->m_x
	&& node->frame->m_x+node->frame->m_width >=  selected->frame->m_x) {
      // If it is higher than other canditates
      if (take == nullptr || node->y < take->y) {
        take = node;
      }
    }
  }

  if (take != nullptr) {
    m_frame_tree->m_selected = take;
  }
}

void Window::change_selected_buffer_left() {
  using namespace __FRAMETREE_IMPL;
  std::vector<Node *> all_frames =
    m_frame_tree->get_all_frames();

  Node *take = nullptr,
       *selected = m_frame_tree->m_selected;

  for (Node *node : all_frames) {
    if (node == selected)
      continue;
    // If the node is in correct position
    if (node->frame->m_x + node->frame->m_width <= selected->frame->m_x
	&& node->frame->m_y <= selected->frame->m_y
	&& node->frame->m_y+node->frame->m_height >=  selected->frame->m_y) {
      // If it is higher than other canditates
      if (take == nullptr || node->x > take->x) {
        take = node;
      }
    }
  }

  if (take != nullptr) {
    m_frame_tree->m_selected = take;
  }
}

void Window::change_selected_buffer_right() {
  using namespace __FRAMETREE_IMPL;
  std::vector<Node *> all_frames =
    m_frame_tree->get_all_frames();

  Node *take = nullptr,
       *selected = m_frame_tree->m_selected;

  for (Node *node : all_frames) {
    if (node == selected)
      continue;
    // If the node is in correct position
    if (node->frame->m_x <= selected->frame->m_x + selected->frame->m_width
	&& node->frame->m_y <= selected->frame->m_y
	&& node->frame->m_y+node->frame->m_height >=  selected->frame->m_y) {
      // If it is higher than other canditates
      if (take == nullptr || node->x < take->x) {
        take = node;
      }
    }
  }

  if (take != nullptr) {
    m_frame_tree->m_selected = take;
  }
}

void Window::add_key_combos(
  std::vector<std::string> combos,
  std::function<void(
    Window *window, BufferCursor &cursor, Frame *frame
  )> callback
) {
  for (const std::string &s : combos) {
    add_key_combo(s, callback);
  }
}
void Window::add_key_combo(
  std::string combo,
  std::function<void(
    Window *window, BufferCursor &cursor, Frame *frame
  )> callback
) {
  m_input_manager->add_key_combo(combo, callback);
}

std::shared_ptr<Buffer> Window::get_active_buffer() {
  if (m_in_minibuffer) {
    return m_minibuffer->m_buffer;
  }
  return m_frame_tree->m_selected->frame->m_buffer;
}

void Window::swap_buffers() { glfwSwapBuffers(m_window); }

void Window::open_minibuffer() { m_in_minibuffer = true; }

bool Window::in_minibuffer() const {
  return m_in_minibuffer;
}

void Window::minibuffer_return() {
  // Reset the flag
  m_in_minibuffer = false;

  // Extract the text, without newlined
  std::string gotten_text = "";
  for (std::string line :
       m_minibuffer->m_buffer->get_text()) {
    // Add a space instead of newline
    if (!gotten_text.empty()) {
      gotten_text += " ";
    }
    // Add the text
    gotten_text += line;
  }

  // Erase text
  m_minibuffer->m_buffer->use_cursor(
    [](Window *, BufferCursor &cursor, Frame *) {
      cursor.set_text("");
    }
  );

  process_minibuffer_command(gotten_text);
}

void Window::process_minibuffer_command(std::string command
) {
  if (command.empty())
    return;

  if (command == "w") {
    // Save the buffer
    get_active_buffer()->save();
  } else if (command[0] == 'e') {
    // Load another file

    // Check if the command syntax is wrong
    if (command.size() <= 2 || command[1] != ' ') {
      LOG("Error: must provide file path with after 'e'");
      return;
    }

    // Load the file in the frame
    use_active_frame<void>([&](Frame *f) {
      f->load_buffer(m_buffer_manager->get_files_buffer(
        command.substr(2, command.size() - 2)
      ));
    });
  } else {
    LOG("Cannot parse command '" + command + "'");
  }
}
