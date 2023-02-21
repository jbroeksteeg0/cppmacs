#pragma once
#include "BufferCursor.h"
#include "Rope.h"
#include <iostream>
#include <memory>
#include <optional>
#include <vector>
#include <functional>

struct FormattedText {
  std::string text;
};

class Buffer {
public:
  explicit Buffer();
  Buffer(std::string file_path);
  // Generate the raw text, run hooks, output
  std::unique_ptr<std::vector<FormattedText>> get_formatted_text() const;

  void use_cursor(std::function<void(BufferCursor&)> func);
private:
  std::optional<std::string> m_file;
  Rope<char> m_rope;
  BufferCursor m_cursor;

  friend struct BufferCursor;
};
