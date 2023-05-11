#include "BufferCursor.h"
#include "Buffer.h"
#include "src/misc/Util.h"

BufferCursor::BufferCursor(Buffer *buffer)
    : m_buffer(buffer), m_index(0) {}

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
  int cursor_pos = m_index;

  // -2 default value, not set
  int prev_newline = -2, second_prev_newline = -2;

  int temp_index = m_index - 1;

  while (temp_index >= -1 && second_prev_newline == -2) {
    if (temp_index == -1 && prev_newline == -2) {
      break;    // no line before, do nothing
    } else if (temp_index == -1) {
      second_prev_newline = -1;
    } else {
      char current_char = m_buffer->m_rope.at(temp_index);

      if (current_char == '\n' && prev_newline == -2) {
        prev_newline = temp_index;
      } else if (current_char == '\n' && second_prev_newline == -2) {
        second_prev_newline = temp_index;
      }
    }
    temp_index--;
  }

  // if there is a line above, move onto it
  if (second_prev_newline != -2) {
    int new_pos = second_prev_newline + 1;
    int dist = cursor_pos - prev_newline - 1;

    while (dist > 0 && ((new_pos < (int)m_buffer->m_rope.size() && m_buffer->m_rope.at(new_pos) != '\n') || new_pos == (int)m_buffer->m_rope.size())) {
      new_pos++;
      dist--;
    }
    m_index = new_pos;
  }
}

void BufferCursor::move_down() {
  // calculate char's position on line
  int pos_on_line = 1;
  int temp_pos = m_index - 1;
  while (temp_pos >= 0
         && m_buffer->m_rope.at(temp_pos) != '\n') {
    temp_pos--;
    pos_on_line++;
  }

  // the next newline
  int start_of_next = m_index;
  while (start_of_next + 1 < (int)m_buffer->m_rope.size()
         && m_buffer->m_rope.at(start_of_next) != '\n')
    start_of_next++;

  // if there is no newline, return
  if (start_of_next == (int)m_buffer->m_rope.size() || m_buffer->m_rope.at(start_of_next) != '\n') {
    return;
  }

  int new_pos =
    start_of_next + 1;    // handle newline edge with +1
  pos_on_line--;

  while (
    pos_on_line > 0
    && new_pos < (int)m_buffer->m_rope.size()
    && (new_pos+1==(int)m_buffer->m_rope.size() || m_buffer->m_rope.at(new_pos) != '\n')
  ) {
    pos_on_line--;
    new_pos++;
  }

  m_index = new_pos;
}

int BufferCursor::get_index() const { return m_index; }

char BufferCursor::get_character(int index) const {
  ASSERT(index >= 0 && index <= (int)m_buffer->m_rope.size(), "Index not in bounds");

  return m_buffer->m_rope.at(index);
}
