#include "Logger.h"
#include <gsl/gsl>

class Logger {
private:
  int m_level;

public:
  static Logger &get() {
    static gsl::owner<Logger*> logger = new Logger();
    return *logger;
  }

  void set_level(LogLevel level) { m_level = (int)level; }

  void log(std::string text) {
    if (m_level >= (int)LogLevel::LOG) {
      std::cout << "\e[33m\e[1m[LOG]:\e[0m\e[1m " + text + "\e[0m" << std::endl;
    }
  }

  void error(std::string text) {
    if (m_level >= (int)LogLevel::ERROR) {
      std::cout << "\e[31m\e[1m[ERROR]:\e[0m\e[1m " + text + "\e[0m"
                << std::endl;
    }
  }

  void debug(std::string text) {
    if (m_level >= (int)LogLevel::DEBUG) {
      std::cout << "\e[33m[DEBUG]:\e[0m " << text << std::endl;
    }
  }
};

void LOG(std::string text) { Logger::get().log(text); }

void ERROR(std::string text) { Logger::get().error(text); }

void DEBUG(std::string text) { Logger::get().debug(text); }

void SET_LOG_LEVEL(LogLevel level) { Logger::get().set_level(level); }
