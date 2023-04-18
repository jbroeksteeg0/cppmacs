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

class BufferManager;
class InputManager;

class Buffer {
public:
  explicit Buffer();
  Buffer(std::string file_path);
  ~Buffer();

  std::vector<std::string> get_text();

  void use_cursor(std::function<void(BufferCursor &)> func
  );    // add to the queue

private:
  std::optional<std::string> m_file;
  Rope<char> m_rope;
  BufferCursor m_cursor;
  
  std::thread m_thread;
  std::mutex m_queue_mutex;
  std::queue<std::function<void(BufferCursor &)>> m_event_queue;

private:
  void start_threaded_event_loop();
  friend struct BufferCursor;
  friend class BufferManager;
  friend class InputManager;
};
