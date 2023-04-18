#include "../backend/CppMacs.h"
#include "../backend/Rope.h"
#include "../backend/Buffer.h"
#include "../misc/Logger.h"
#include "../misc/Tests.h"
#include "Window.h"

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


  window.run();
}
