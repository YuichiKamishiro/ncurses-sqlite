#include "../inc/Application.h"
#include <curses.h>

Application::Application(std::string nameOfDatabase) : db(nameOfDatabase) {}

void Application::init()
{
    initscr();

    cbreak();
    noecho();
    keypad(stdscr, 1);
    timeout(-1);
}

void Application::startMenu()
{
    clear();

    int minElement = 0;
    int currentElement = minElement;
    int maxElement = 0;

    timeout(0);

    while(!startMenuShouldClose)
    {  
        int key = getch();
        
        if(key == KEY_UP && currentElement != minElement) --currentElement;
        if(key == KEY_DOWN && currentElement != maxElement) ++currentElement;
        
        for(int i = 0; i < choicesForMenu.size(); ++i)
        {
            mvprintw(i, 1, choicesForMenu[i].c_str());
        }
    }
    timeout(-1);
}

void Application::run()
{
    startMenu();

    while(!windowShouldClose)
    {

    }
}
