#pragma once

#include <iostream>

void LOG(std::string text);
void ERROR(std::string text);
void DEBUG(std::string text);

enum class LogLevel {
  ERROR,
  LOG,
  DEBUG
};

void SET_LOG_LEVEL(LogLevel level);
