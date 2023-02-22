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

void BufferCursor::move_left() {
  if (m_index > 0)
    m_index--;
}

void BufferCursor::move_right() {
  if (m_index < (int)m_buffer->m_rope.size())
    m_index++;
}

void BufferCursor::move_up() {
  // calculate char's position on line
  int pos_on_line = 0;
  int temp_index = m_index;
  while (temp_index > 0 && m_buffer->m_rope.at(temp_index-1) != '\n') {
    temp_index--;
    pos_on_line++;
  }

  // find index of pos_on_line but on next line
  // if there is no new line, keep as -1 and do nothing
  int start_of_prev_line = -1;
  int newlines_found = 0; // number of newlines or the beginning of the line

  temp_index = m_index;
  while (temp_index > 0) {
    temp_index--;
    if (temp_index == 0 || m_buffer->m_rope.at(temp_index) == '\n')
      newlines_found++;

    // has found the beginning of the prev line
    if (newlines_found == 2) {
      start_of_prev_line = temp_index;
      break;
    }
  }


  // if there is a prev line move cursor
  if (newlines_found == 2) {
    temp_index = start_of_prev_line;
    // while the cursor can and should be moved forward
    while (m_buffer->m_rope.at(temp_index) != '\n' && temp_index + 1 < (int)m_buffer->m_rope.size() && (temp_index - start_of_prev_line) < pos_on_line)
      temp_index++;

    m_index = temp_index;
  }
}

void BufferCursor::move_down() {
  // calculate char's position on line
  int pos_on_line = 0;
  int temp_index = m_index;
  while (temp_index > 0 && m_buffer->m_rope.at(temp_index-1) != '\n') {
    temp_index--;
    pos_on_line++;
  }

  // go to the end of the line
  temp_index = m_index;
  int beginning_of_next_line = -1;
  while (temp_index+1 < (int)m_buffer->m_rope.size()) {
    temp_index++;
    if (m_buffer->m_rope.at(temp_index) == '\n') {
      beginning_of_next_line = temp_index;
      break;
    }
  }

  // if there is a new line, place as far as possible
  if (beginning_of_next_line != -1) {
    temp_index = beginning_of_next_line;
    // while the cursor can and should be moved forward
    while (m_buffer->m_rope.at(temp_index) != '\n' && temp_index + 1 < (int)m_buffer->m_rope.size() && (temp_index - beginning_of_next_line) < pos_on_line)
      temp_index++;

    m_index = temp_index;
  }
}
