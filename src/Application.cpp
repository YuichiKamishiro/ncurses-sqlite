#include "../inc/Application.h"
#include <curses.h>

Application::Application(std::string nameOfDatabase) : db(nameOfDatabase) {}

void Application::quit()
{
    startMenuShouldClose = 1;
    windowShouldClose = 1;
}

void Application::printInfoFromTable(std::string nameOfTable)
{
    clear();

    SQLite::Statement query(db, "select * from " + nameOfTable);

    int columnCount = query.getColumnCount();

    int offsetY = 0;

    while(query.executeStep())
    {
        for(int i = 0; i < columnCount; ++i)
        {
            mvprintw(1 + offsetY, 1, query.getColumn(i));
            ++offsetY;
        }
        ++offsetY;
    }
    
    refresh();

    timeout(-1);
    getch();

    quit();
}

void Application::insertValuesIntoTable()
{
    
}

void Application::init()
{
    initscr();

    cbreak();
    noecho();
    keypad(stdscr, 1);
    timeout(0);
    curs_set(0);

    start_color();
}

std::string Application::selectionTable()
{
    clear();
    
    timeout(-1);

    mvprintw(1, 1, "Enter table name: ");

    char *bufStr = new char[64];
    
    getstr(bufStr);

    timeout(0);

    return bufStr;
}


void Application::startMenu()
{
    int minElement = 0;
    int currentElement = minElement;
    int maxElement = choicesForMenu.size();

    init_pair(SELECT_COLOR_PAIR, COLOR_BLACK, COLOR_WHITE);

    while(!startMenuShouldClose)
    {
        int key = getch();
        
        if(key == KEY_UP && currentElement != minElement) --currentElement;
        if(key == KEY_DOWN && currentElement != maxElement) ++currentElement;
        if(key == 10 && choicesForMenu[currentElement] == "Quit") quit();
        if(key == 10 && choicesForMenu[currentElement] == "Print All") printInfoFromTable(selectionTable());

        for(int i = 0; i < choicesForMenu.size(); ++i)
        {
            if(currentElement == i)
            {
                attron(COLOR_PAIR(SELECT_COLOR_PAIR));
                mvprintw(i, 1, choicesForMenu[i].c_str());
                attroff(COLOR_PAIR(SELECT_COLOR_PAIR));
            }
            else mvprintw(i, 1, choicesForMenu[i].c_str());
        }
    }
}

void Application::run()
{
    init();
    startMenu();

    while(!windowShouldClose)
    {

    }

    endwin();
}
