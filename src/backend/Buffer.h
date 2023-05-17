#pragma once
#include "BufferCursor.h"
#include "Module.h"
#include "Rope.h"
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <vector>
#include "../frontend/Frame.h"

class BufferManager;
class InputManager;
struct Frame;
class Window;

class Buffer {
public:
  explicit Buffer(Window *parent);
  Buffer(std::string file_path, Window *parent);
  ~Buffer();

  std::vector<std::string> get_text();

  void use_cursor(
    std::function<void(Window *, BufferCursor &, Frame *)>
      func
  );

  void save();
  void open_path(std::string file_path);
  void close();
  
  int get_cursor_position() const;
  bool is_minibuffer() const;
private:
  std::optional<std::filesystem::path> m_file;
  Rope<char> m_rope;
  BufferCursor m_cursor;

  std::thread m_thread;
  std::mutex m_queue_mutex;
  std::atomic_bool m_should_close = false;
  std::queue<
    std::function<void(Window *w, BufferCursor &, Frame *)>>
    m_event_queue;
  Window *m_parent;
  Frame *m_current_frame = nullptr;
private:
  void start_threaded_event_loop();

  friend struct BufferCursor;
  friend struct Frame;
  friend class BufferManager;
  friend class InputManager;
};
