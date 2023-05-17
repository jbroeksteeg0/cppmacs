#include "BufferManager.h"

BufferManager::BufferManager(Window *window)
    : m_window(window) {
  m_scratch_buffer = std::make_shared<Buffer>(window);
  m_buffers = {m_scratch_buffer};
}

BufferManager::~BufferManager() {
  for (std::shared_ptr<Buffer> buffer : m_buffers) {
    buffer->close();
  }
}

std::shared_ptr<Buffer>
BufferManager::get_files_buffer(std::string file_name) {
  // Check if the file already exists in a buffer
  for (std::shared_ptr<Buffer> buffer : m_buffers) {
    // If the buffers file is what we want, return it
    if (auto file = buffer->m_file) {
      if (file == std::filesystem::path(file_name)) {
        return buffer;
      }
    }
  }

  // Create the thing if it does not exist
  m_buffers.push_back(
    std::make_shared<Buffer>(file_name, m_window)
  );

  m_buffers.back()->m_file = file_name;

  // Return it
  return m_buffers.back();
}

std::shared_ptr<Buffer>
BufferManager::get_scratch_buffer() {
  return m_scratch_buffer;
}
