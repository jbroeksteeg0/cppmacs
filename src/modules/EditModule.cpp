#include "EditModule.h"

EditModule::EditModule() {}

void EditModule::add_key_press_hook(std::function<void(KeyPress)> callback) {
  m_key_press_hook.push_back(callback);
}

void EditModule::set_key_press_hook(
  std::vector<std::function<void(KeyPress)>> callbacks
) {
  m_key_press_hook = callbacks;
}

std::shared_ptr<EditModule> EditModule::from(std::shared_ptr<Module> from) {
  if (from->get_identifier() == from->get_identifier()) {
    return static_pointer_cast<EditModule>(from);
  }
  return nullptr;
}

EditModuleBuilder::EditModuleBuilder() {}

EditModule EditModuleBuilder::create() { return m_module; }
