#pragma once

#include "../backend/Module.h"
#include <functional>
#include <iostream>
#include <memory>

class EditModuleBuilder;

class EditModule : public Module {
public:
  std::string get_identifier() const override { return "Edit"; }
  static std::shared_ptr<EditModule> from(std::shared_ptr<Module> from);

private:
  EditModule();

  struct KeyPress {
    char key;
  };
  std::vector<std::function<void(KeyPress)>> m_key_press_hook;

public:
  void add_key_press_hook(std::function<void(KeyPress)> callback);
  void set_key_press_hook(std::vector<std::function<void(KeyPress)>> callbacks);

  friend class EditModuleBuilder;
  friend class Window;
};

class EditModuleBuilder {
public:
  EditModuleBuilder();
  EditModule create();

private:
  EditModule m_module;
};
