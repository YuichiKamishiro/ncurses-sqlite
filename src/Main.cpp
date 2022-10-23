#include "../inc/Application.h"

#include <iostream>

int main() {
  try
  {
    Application app("yourDB.db");
    app.run();
  }
  catch(std::exception &ex)
  {
    std::cout << ex.what();
  }
}
