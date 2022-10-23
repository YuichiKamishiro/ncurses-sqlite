#pragma once

#include "Menu.h"

class Application
{
private:
  Menu menu;
public:
  Application(std::string);

  void init();
  void run();
};
