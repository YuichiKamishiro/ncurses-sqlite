#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include <string>
#include <vector>
#include <curses.h>

class Menu
{
private:
  SQLite::Database db;
  
  bool startMenuShouldClose = false;

  std::vector<std::string> choicesForMenu = {"Quit", "Print All", "Insert Values", "Delete Info From Table"};
public:
  
  Menu(std::string);

  std::string selectionTable();
  std::string parseTypes(std::vector<std::string>, std::vector<std::string>);

  void drawMenu(int);
  void printInfoFromTable(std::string);
  void insertValuesIntoTable(std::string);
  void deleteInfoFromTable(std::string);
  void quit();
  void startMenu();
};

