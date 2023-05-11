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
  std::filesystem::path path = file_path;

  path = std::filesystem::canonical(path);

  // TODO: can't create files
  if (!std::filesystem::exists(path) || !path.has_filename()) {
    LOG("Invalid path: " + path.string());
  } else {
    m_file = {path};

    std::ifstream in(file_path);
    char ch;
    while (in >> ch) {
      m_rope.insert(ch, m_rope.size());
    }
  }

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

int Buffer::get_cursor_position() const {
  return m_cursor.get_index();
}
