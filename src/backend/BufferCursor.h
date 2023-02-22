#pragma once
#include <iostream>
#include <vector>

class Buffer;

struct BufferCursor {
public:
  BufferCursor(Buffer *buffer);

  void delete_character_before();
  void insert_text(std::string text);
  void insert_newline();

  void move_left();
  void move_right();
  void move_up();
  void move_down();

private:
  Buffer *m_buffer;
  int m_index;
};
