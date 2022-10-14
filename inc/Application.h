#include <SQLiteCpp/SQLiteCpp.h>

#include <curses.h>
#include <vector>
#include <string>
#include <iostream>

#define SELECT_COLOR_PAIR 1

class Application
{
private:
    SQLite::Database db;
    
    bool windowShouldClose = false;
    bool startMenuShouldClose = false;

    std::vector<std::string> choicesForMenu =
    { 
      "Quit",
      "Print All",
      "Insert Values"
    };
public:
    Application(std::string);
    
    void init();
    void quit();
    std::string selectionTable();
    void printInfoFromTable(std::string);
    void insertValuesIntoTable();
    void startMenu();
    void run();
};
