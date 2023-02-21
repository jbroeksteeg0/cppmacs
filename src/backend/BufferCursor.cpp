#include "BufferCursor.h"
#include "Buffer.h"

BufferCursor::BufferCursor(Buffer *buffer) : m_buffer(buffer), m_index(0) {}

void BufferCursor::delete_character_before() {
  if (m_index == 0)
    return;

  Rope<char> &buffer_rope = m_buffer->m_rope;
  buffer_rope.erase(m_index - 1);
  m_index--;
}

void BufferCursor::insert_text(std::string text) {
  Rope<char> &buffer_rope = m_buffer->m_rope;

  for (char ch : text) {
    buffer_rope.insert(ch, m_index);
    m_index++;
  }
}

void BufferCursor::insert_newline() { insert_text("\n"); }
