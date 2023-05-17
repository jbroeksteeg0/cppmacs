#include "../backend/Buffer.h"
#include "../backend/CppMacs.h"
#include "../backend/Rope.h"
#include "../misc/Logger.h"
#include "../misc/Tests.h"
#include "Window.h"
#include "src/backend/BufferCursor.h"

#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <thread>
#include <vector>

struct Frame;
void delete_word(Window *window, BufferCursor &cursor, Frame *frame) {
  // if at the start of the file, do nothing
  if (cursor.get_index() == 0)
    return;

  auto is_num = [](char ch) -> bool { return ch >= '0' && ch <= '9'; };

  char char_before = cursor.get_character(cursor.get_index() - 1);

  if (char_before == ' ') {
    // delete spaces
    while (cursor.get_index() > 0
           && cursor.get_character(cursor.get_index() - 1) == ' ') {
      cursor.delete_character_before();
    }
  } else if (isalpha(char_before)) {
    // delete letters
    while (cursor.get_index() > 0
           && isalpha(cursor.get_character(cursor.get_index() - 1))) {
      cursor.delete_character_before();
    }
  } else if (is_num(char_before)) {
    // delete numbers
    while (cursor.get_index() > 0
           && is_num(cursor.get_character(cursor.get_index() - 1))) {
      cursor.delete_character_before();
    }
  } else {
    // delete a single of something else
    cursor.delete_character_before();
  }
}

int main() {
  SET_LOG_LEVEL(LogLevel::DEBUG);

  Window window = Window();

  for (char ch = 'a'; ch <= 'z'; ch++) {
    window.add_key_combo(
      std::string(1, ch),
      [ch](Window *window, BufferCursor &cursor, Frame *frame) {
        cursor.insert_text(std::string(1, ch));
      }
    );
  }

  for (char ch = 'A'; ch <= 'Z'; ch++) {
    window.add_key_combo(
      std::string(1, ch),
      [ch](Window *window, BufferCursor &cursor, Frame *frame) {
        cursor.insert_text(std::string(1, ch));
      }
    );
  }

  for (char ch = '0'; ch <= '9'; ch++) {
    window.add_key_combo(
      std::string(1, ch),
      [ch](Window *window, BufferCursor &cursor, Frame *frame) {
        cursor.insert_text(std::string(1, ch));
      }
    );
  }

  window.add_key_combo(
    "Space",
    [](Window *windnow, BufferCursor &cursor, Frame *frame) {
      cursor.insert_text(" ");
    }
  );

  window.add_key_combo(
    "Backspace",
    [](Window *window, BufferCursor &cursor, Frame *frame) {
      cursor.delete_character_before();
    }
  );

  // delete word
  window.add_key_combos({"Ctrl Backspace", "Ctrl w"}, delete_word);

  window.add_key_combo(
    "Tab",
    [](Window *window, BufferCursor &cursor, Frame *frame) {
      cursor.insert_text("  ");
    }
  );
  window.add_key_combo(
    "Return",
    [](Window *window, BufferCursor &cursor, Frame *frame) {
      if (cursor.in_minibuffer()) {
	cursor.minibuffer_return();
      } else {
	cursor.insert_newline();
      }
    }
  );
  window.add_key_combos(
    {"Left", "Ctrl h"},
    [](Window *window, BufferCursor &cursor, Frame *frame) {
      cursor.move_left();
    }
  );

  window.add_key_combos(
    {"Right", "Ctrl l"},
    [](Window *window, BufferCursor &cursor, Frame *frame) {
      cursor.move_right();
    }
  );

  window.add_key_combos(
    {"Down", "Ctrl j"},
    [](Window *window, BufferCursor &cursor, Frame *frame) {
      cursor.move_down();
    }
  );

  window.add_key_combos(
    {"Up", "Ctrl k"},
    [](Window *window, BufferCursor &cursor, Frame *frame) { cursor.move_up(); }
  );

  window.add_key_combo(
    "Ctrl Space q",
    [](Window *window, BufferCursor &cursor, Frame *frame) { window->close(); }
  );

  // TODO: this dies lmao
  window.add_key_combo(
    "Ctrl Space w v",
    [](Window *window, BufferCursor &cursor, Frame *frame) {
      window->vertical_split();
    }
  );
  window.add_key_combo(
    "Ctrl Space w f",
    [](Window *window, BufferCursor &cursor, Frame *frame) {
      window->horizontal_split();
    }
  );
  window.add_key_combo(
    "Ctrl Space w j",
    [](Window *window, BufferCursor &cursor, Frame *frame) {
      window->change_selected_frame_down();
    }
  );
  window.add_key_combo(
    "Ctrl Space w k",
    [](Window *window, BufferCursor &cursor, Frame *frame) {
      window->change_selected_frame_up();
    }
  );

  window.add_key_combo(
    "Ctrl Space w h",
    [](Window *window, BufferCursor &cursor, Frame *frame) {
      window->change_selected_frame_left();
    }
  );
  window.add_key_combo(
    "Ctrl Space w l",
    [](Window *window, BufferCursor &cursor, Frame *frame) {
      window->change_selected_frame_right();
    }
  );

  std::vector<char> symbols = {'!', '@', '#',  '$', '%',  '^', '&', '*',
                               '(', ')', '-',  '=', '_',  '+', '{', '}',
                               '[', ']', ':',  ';', '\'', '"', ',', '<',
                               '.', '>', '\\', '|', '/',  '?'};
  for (char ch : symbols) {
    window.add_key_combo(
      std::string(1, ch),
      [ch](Window *window, BufferCursor &cursor, Frame *frame) {
        cursor.insert_text(std::string(1, ch));
      }
    );
  }

  window.add_key_combo(
    "Ctrl Space :",
    [](Window *window, BufferCursor &cursor, Frame *frame) {
      window->open_minibuffer();
    }
  );

  window.add_key_combo("Ctrl Space w q", [](Window *window, BufferCursor &cursor, Frame *frame){
    window->close_current_frame();
  });

  window.run();
}
