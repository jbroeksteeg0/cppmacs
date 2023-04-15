#include "InputManager.h"

InputManager::InputManager(Window *parent) : m_parent(parent) {}

void InputManager::press_key(int key_code, int mods) {
  std::string translated;

  if (key_code == 340)    // LSHIFT
    return;

  if (key_code >= 'A' && key_code <= 'Z') {
    if (mods & 1) {    // LSHIFT
      translated = (char)key_code;
    } else {
      translated = (char)key_code + ('a' - 'A');
    }
  }

  if (!m_current_combo.empty())
    m_current_combo += " ";    // to separate keys

  m_current_combo += translated;

  int num_possibilities = 0;

  for (auto p : m_candidates) {
    if (p.first.size() < m_current_combo.size())    // too short
      continue;

    if (p.first == m_current_combo) {    // run it
      Buffer* buffer = m_parent->get_active_buffer().get();
      p.second(m_parent, BufferCursor(buffer));
      m_current_combo = "";
      return;
    } else if (p.first.substr(0, m_current_combo.size()) == m_current_combo) {    // if a substring
      num_possibilities += 1;
    }
  }

  if (num_possibilities == 0) {
    m_current_combo = "";
  }
}

void InputManager::add_key_combo(
  std::string combo,
  std::function<void(Window *window, BufferCursor cursor)> function
) {
  for (auto iter = m_candidates.begin(); iter != m_candidates.end(); iter++) {
    if (iter->first == combo) {
      m_candidates.erase(iter);
      break;
    }
  }
  m_candidates.push_back({combo, function});
}
