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
class Window;

class Buffer {
public:
  explicit Buffer(Window *parent);
  Buffer(std::string file_path, Window *parent);
  ~Buffer();

  std::vector<std::string> get_text();

  void use_cursor(std::function<void(Window *w, BufferCursor &)> func
  );    // add to the queue

  int get_cursor_position() const;

  void start_threaded_event_loop();
private:
  std::optional<std::string> m_file;
  Rope<char> m_rope;
  BufferCursor m_cursor;
  
  std::thread m_thread;
  std::mutex m_queue_mutex;
  std::queue<std::function<void(Window* w, BufferCursor &)>> m_event_queue;
  Window *m_parent;
private:
  friend struct BufferCursor;
  friend class BufferManager;
  friend class InputManager;
};
