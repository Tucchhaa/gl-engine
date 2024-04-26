#include "iinput.hpp"

IInput* IInput::instance = nullptr;

IInput* IInput::getInstance() {
    return instance;
}