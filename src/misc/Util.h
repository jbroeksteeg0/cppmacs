#pragma once

#include "Logger.h"
#include <iostream>

//NOLINTNEXTLINE because the macro cannot be recreated with a function
#define ASSERT(condition, message)                                             \
  if (!(condition)) {                                                          \
    std::cout << "\e[31m\e[1m[ASSERTION FAILED]\e[0m\e[1m"                     \
              << " on line " << __LINE__ << " of file " << __FILE__            \
              << " with message '" << message << "'\e[0m" << std::endl;        \
    exit(1);                                                                   \
  };

#define ASSERT_NOT_REACHED() \
  ASSERT(false, "ASSERT_NOT_REACHED"); \
  exit(1);
