#include "Buffer.h"

Buffer::Buffer()
    : m_file({}), m_rope(Rope<char>()),
      m_cursor(BufferCursor(this)) {}

Buffer::Buffer(std::string file_path)
    : m_file(file_path), m_rope(Rope<char>()),
      m_cursor(BufferCursor(this)) {}

std::unique_ptr<std::vector<FormattedText>> Buffer::get_formatted_text() const {
  std::unique_ptr<std::vector<FormattedText>> formatted_text =
    std::make_unique<std::vector<FormattedText>>();

  std::string current_text = "";
  for (const char ch : m_rope) {
    current_text += std::string(1,ch);
  }
  
  formatted_text->push_back(FormattedText{current_text});
  return formatted_text;
}

void Buffer::use_cursor(std::function<void(BufferCursor &)> func) {
  func(m_cursor);
}
