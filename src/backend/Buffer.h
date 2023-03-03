#pragma once
#include "BufferCursor.h"
#include "Rope.h"
#include <iostream>
#include <memory>
#include <optional>
#include <vector>
#include <functional>
#include <mutex>
#include <thread>
#include <queue>

struct FormattedText {
  std::string text;
};

class Buffer {
public:
  explicit Buffer();
  Buffer(std::string file_path);
  ~Buffer();

  // Generate the raw text, run hooks, output
  std::unique_ptr<std::vector<FormattedText>> get_formatted_text();

  void use_cursor(std::function<void(BufferCursor&)> func); // add to the queue
private:
  std::optional<std::string> m_file;
  Rope<char> m_rope;
  BufferCursor m_cursor;

  std::thread m_thread;
  std::mutex m_queue_mutex;
  std::queue<std::function<void(BufferCursor&)>> m_event_queue;
  
private:
  void start_threaded_event_loop();
  friend struct BufferCursor;
};
