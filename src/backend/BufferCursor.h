#pragma once
#include <iostream>
#include <vector>

class Buffer;

struct BufferCursor {
public:
  BufferCursor(Buffer *buffer);
  BufferCursor(const BufferCursor& other) = default;
  void delete_character_before();
  void insert_text(std::string text);
  void set_text(std::string text);
  void insert_newline();
  void minibuffer_return();

  void move_left();
  void move_right();
  void move_up();
  void move_down();

  int get_index() const;
  char get_character(int index) const;
  bool in_minibuffer() const;
private:
  Buffer *m_buffer;
  int m_index;
};
