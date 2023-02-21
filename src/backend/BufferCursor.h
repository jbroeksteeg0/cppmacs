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

private:
  Buffer *m_buffer;
  int m_index;
};
