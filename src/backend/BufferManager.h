#pragma once

#include "Buffer.h"
#include <memory>
#include <vector>

class Window;

class BufferManager {
public:
  BufferManager(Window *window);
  ~BufferManager();
  std::shared_ptr<Buffer> get_scratch_buffer();
  std::shared_ptr<Buffer> get_files_buffer(std::string file_name);
private:
  Window* m_window;
  std::vector<std::shared_ptr<Buffer>> m_buffers;
  std::shared_ptr<Buffer> m_scratch_buffer;
};
