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

  window.run();
}
