#include "Buffer.h"
#include "src/backend/BufferCursor.h"
#include <filesystem>
#include <fstream>

Buffer::Buffer(Window *parent)
    : m_file({}), m_rope(Rope<char>()),
      m_cursor(BufferCursor(this)), m_parent(parent) {
  start_threaded_event_loop();
}

Buffer::Buffer(std::string file_path, Window *parent)
    : m_rope(Rope<char>()), m_cursor(BufferCursor(this)),
      m_parent(parent) {
  open_path(file_path);
  start_threaded_event_loop();
}

Buffer::~Buffer() {}

std::vector<std::string> Buffer::get_text() {
  std::lock_guard<std::mutex> lk(m_queue_mutex
  );    // lock the queue

  std::vector<std::string> to_return = {""};

  for (char ch : m_rope) {
    if (ch == '\n') {
      to_return.push_back("");
    } else {
      to_return.back().push_back(ch);
    }
  }

  return to_return;
}

void Buffer::use_cursor(
  std::function<void(Window *, BufferCursor &, Frame *)>
    func
) {
  std::lock_guard<std::mutex> lk(m_queue_mutex);
  m_event_queue.push(func);
}

void Buffer::start_threaded_event_loop() {
  m_thread = std::thread([this]() {
    while (!m_should_close) {
      std::optional<std::function<
        void(Window *, BufferCursor &, Frame *)>>
        func = {};
      // Extract a single event if possible
      {
        std::lock_guard<std::mutex> lk(m_queue_mutex);
        if (!m_event_queue.empty()) {
          func = m_event_queue.front();
          m_event_queue.pop();
        }
      }

      // Run the event if possible
      if (func.has_value()) {
        auto f = func.value();
	ASSERT(m_current_frame != nullptr, "Current frame should not be nullptr at this point");
        f(m_parent, m_cursor, m_parent->m_frame_tree->m_selected->frame.get());
      }
    }
  });
}

void Buffer::save() {
  if (auto path = m_file) {
    std::ofstream out(path->string());

    for (std::string line: get_text()) {
      out << line << "\n";
    }
  } else {
    LOG("Cannot save: buffer has no file");
  }
}

void Buffer::open_path(std::string file_path) {
  std::filesystem::path path = file_path;

  if (!std::filesystem::exists(path)) {
    LOG("Path does not exist: " + path.string());
    return;
  }

  // Resolve dots etc
  path = std::filesystem::canonical(path);

  // Set the member variable
  m_file = path;

  // Clear the original content
  while (m_rope.size()) {
    m_rope.erase(0);
  }
  // Load the file
  std::ifstream in(path.string());
  std::string temp;
  while (std::getline(in, temp)) {
    if (m_rope.size() != 0) {
      m_rope.insert('\n', m_rope.size());
    }
    for (char ch: temp) {
      m_rope.insert(ch,m_rope.size());
    }
  }
}

void Buffer::close() {
  m_should_close = true;
  m_thread.join();
}

int Buffer::get_cursor_position() const {
  return m_cursor.get_index();
}

bool Buffer::is_minibuffer() const {
  return m_parent->in_minibuffer() && this == m_parent->get_active_buffer().get();
}
