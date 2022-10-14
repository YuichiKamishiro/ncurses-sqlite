#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include <curses.h>

#include <vector>
#include <string>

class Application
{
private:
    SQLite::Database db;

    bool windowShouldClose = false;
    bool startMenuShouldClose = false;

    std::vector<std::string> choicesForMenu =
    { "quit" };
public:
    Application(std::string);
    
    void init();
    void startMenu();
    void run();
};
