#include "BufferManager.h"

BufferManager::BufferManager(Window *window) {
  m_scratch_buffer = std::make_unique<Buffer>(window);
}
