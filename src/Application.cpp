#include "../inc/Application.h"

Application::Application(std::string nameOfTable) : menu(nameOfTable) {}

void Application::init()
{
  initscr();

  cbreak();
  noecho();
  keypad(stdscr, 1);
  curs_set(0);

  start_color();
}

void Application::run()
{
  init();
  menu.startMenu();
  endwin();
}
