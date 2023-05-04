#pragma once

#include "Buffer.h"
#include <memory>
#include <vector>

class BufferManager {
  BufferManager(Window *window);
private:
  std::vector<std::unique_ptr<Buffer>> m_buffers;
  std::unique_ptr<Buffer> m_scratch_buffer;
};
