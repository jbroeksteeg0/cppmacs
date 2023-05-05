#include "InputManager.h"

InputManager::InputManager(Window *parent) : m_parent(parent) {}

void InputManager::press_key(int key_code, int mods) {
  std::string translated;

  if (key_code == 340)    // LSHIFT
    return;

  m_parent->get_canvas()->text_box_key_pressed();

  if (key_code >= 'A' && key_code <= 'Z') {
    key_code -= 'A' - 'a';
  }

  std::vector<std::pair<char,char>> shift_values = {};
  for (char ch = 'a'; ch <= 'z'; ch++) {
    shift_values.push_back({ch, ch + 'A'-'a'});
  }

  shift_values.push_back({'0',')'});
  shift_values.push_back({'1','!'});
  shift_values.push_back({'2','@'});
  shift_values.push_back({'3','#'});
  shift_values.push_back({'4','$'});
  shift_values.push_back({'5','%'});
  shift_values.push_back({'6','^'});
  shift_values.push_back({'7','&'});
  shift_values.push_back({'8','*'});
  shift_values.push_back({'9','('});
  shift_values.push_back({'=','+'});
  shift_values.push_back({'-','_'});
  shift_values.push_back({'[','{'});
  shift_values.push_back({']','}'});
  shift_values.push_back({';',':'});
  shift_values.push_back({'\'','"'});
  shift_values.push_back({',','<'});
  shift_values.push_back({'.','>'});
  shift_values.push_back({'/','?'});
  shift_values.push_back({'\\','|'});

  for (auto pair: shift_values) {
    if (key_code == pair.first && (mods & 1) == 0) {
      translated = pair.first;
      break;
    } else if (key_code == pair.first && (mods & 1) == 1) {
      translated = pair.second;
      break;
    }
  }

  // Other codes
  if (key_code == 32) {
    translated = "Space";
  } else if (key_code == 259) {
    translated = "Backspace";
  } else if (key_code == 257) {
    translated = "Return";
  } else if (key_code == 263) {
    translated = "Left";
  } else if (key_code == 265) {
    translated = "Up";
  } else if (key_code == 264) {
    translated = "Down";
  } else if (key_code == 262) {
    translated = "Right";
  } else if (key_code == 258) {
    translated = "Tab";
  }

  if (!m_current_combo.empty())
    m_current_combo += " ";    // to separate keys

  // If control pressed
  if (m_current_combo.empty() && (mods & 2)==2) {
    m_current_combo = "Ctrl ";
  }
  m_current_combo += translated;

  int num_possibilities = 0;

  for (auto p : m_candidates) {
    if (p.first.size() < m_current_combo.size())    // too short
      continue;

    if (p.first == m_current_combo) {    // run it
      m_parent->get_active_buffer()->use_cursor(p.second);
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
  std::function<void(Window *window, BufferCursor& cursor)> function
) {
  for (auto iter = m_candidates.begin(); iter != m_candidates.end(); iter++) {
    if (iter->first == combo) {
      m_candidates.erase(iter);
      break;
    }
  }
  m_candidates.push_back({combo, function});
}
