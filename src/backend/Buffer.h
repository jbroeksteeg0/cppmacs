#pragma once
#include "BufferCursor.h"
#include "Module.h"
#include "Rope.h"
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <vector>

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

  void use_cursor(std::function<void(BufferCursor &)> func
  );    // add to the queue

private:
  std::optional<std::string> m_file;
  Rope<char> m_rope;
  BufferCursor m_cursor;
  std::vector<Module> m_modules; // TODO: go from here, unimplemented
  
  std::thread m_thread;
  std::mutex m_queue_mutex;
  std::queue<std::function<void(BufferCursor &)>> m_event_queue;

private:
  void start_threaded_event_loop();
  friend struct BufferCursor;
};
