#include <SQLiteCpp/SQLiteCpp.h>

#include <fmt/core.h>

#include <curses.h>
#include <vector>
#include <string>
#include <iostream>

#define SELECT_COLOR_PAIR 1

class Application
{
private:
    SQLite::Database db;
    
    bool startMenuShouldClose = false;

    std::vector<std::string> choicesForMenu =
    { 
      "Quit",
      "Print All",
      "Insert Values",
      "Delete Info From Table",
    };
public:
    Application(std::string);
    
    void init();
    void quit();
    std::string selectionTable();
    void printInfoFromTable(std::string);
    std::string parse(std::vector<std::string>, std::vector<std::string>);
    void insertValuesIntoTable(std::string);
    void deleteInfoFromTable(std::string);
    void drawMenu(int);
    void startMenu();
    void run();
};
