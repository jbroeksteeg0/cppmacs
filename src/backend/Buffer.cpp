#include "Buffer.h"

Buffer::Buffer()
    : m_file({}), m_rope(Rope<char>()), m_cursor(BufferCursor(this)) {}

// TODO: make this actually load the file lmao
Buffer::Buffer(std::string file_path)
    : m_file(file_path), m_rope(Rope<char>()), m_cursor(BufferCursor(this)) {}

Buffer::~Buffer() {
  m_thread.std::thread::~thread(); // cancel thread
}

std::vector<std::string> Buffer::get_text() {
  std::lock_guard<std::mutex> lk(m_queue_mutex); // lock the queue

  std::vector<std::string> to_return = {""};

  for (const char ch: m_rope) {
    if (ch == '\n') {
      to_return.push_back("");
    } else {
      to_return.back().push_back(ch);
    }
  }

  return to_return;
}

void Buffer::use_cursor(std::function<void(BufferCursor&)> func) {
  std::lock_guard<std::mutex> lk(m_queue_mutex);
  m_event_queue.push(func);
}

void Buffer::start_threaded_event_loop() {
  m_thread = std::thread([this]() {
    while (true) {
      std::optional<std::function<void(BufferCursor&)>> func = {};

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
	f(m_cursor);
      }
    }
  });
}
