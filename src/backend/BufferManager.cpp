#include "BufferManager.h"

BufferManager::BufferManager() {
  m_scratch_buffer = std::make_unique<Buffer>();
}
