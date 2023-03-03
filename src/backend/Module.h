#pragma once
#include <iostream>
#include <memory>

class Module {
public:
  virtual std::string get_identifier() const = 0;
};
