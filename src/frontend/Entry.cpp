#include "../backend/CppMacs.h"
#include "../backend/Rope.h"
#include "../backend/Buffer.h"
#include "../misc/Logger.h"
#include "../misc/Tests.h"

#include <chrono>
#include <iostream>
#include <stdio.h>
#include <thread>
#include <vector>

int main() {
  SET_LOG_LEVEL(LogLevel::DEBUG);

  Buffer buffer = Buffer();
  
  buffer.use_cursor([](BufferCursor& cursor){
    cursor.insert_text("a");
    cursor.insert_text("b");
    cursor.insert_text("c");
    cursor.insert_newline();
    cursor.insert_text("d");
    cursor.insert_text("e");
    cursor.delete_character_before();
  });

  auto text = buffer.get_formatted_text();
  std::cout << (*text)[0].text << std::endl;

}
