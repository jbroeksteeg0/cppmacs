#include "../backend/CppMacs.h"
#include "../backend/Rope.h"
#include "../backend/Buffer.h"
#include "../misc/Logger.h"
#include "../misc/Tests.h"
#include "Window.h"
#include "src/backend/BufferCursor.h"

#include <iostream>
#include <stdio.h>
#include <thread>
#include <vector>

int main() {
  SET_LOG_LEVEL(LogLevel::DEBUG);

  Window window = Window();

  for (char ch = 'a'; ch <= 'z'; ch++) {
    window.add_key_combo(std::string(1,ch), [ch](Window *window, BufferCursor& cursor){
      cursor.insert_text(std::string(1,ch));
    });   
  }

  for (char ch = 'A'; ch <= 'Z'; ch++) {
    window.add_key_combo(std::string(1,ch), [ch](Window *window, BufferCursor& cursor){
      cursor.insert_text(std::string(1,ch));
    });   
  }

  window.add_key_combo("Space", [](Window *windnow, BufferCursor& cursor){
    cursor.insert_text(" ");
  });

  window.add_key_combo("Backspace", [](Window *window, BufferCursor& cursor){
    cursor.delete_character_before();
  });
  window.add_key_combo("Return", [](Window *window, BufferCursor& cursor){
    cursor.insert_newline();
  });


  window.run();
}
