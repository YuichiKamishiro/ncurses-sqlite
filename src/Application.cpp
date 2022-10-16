#include "../inc/Application.h"
#include <curses.h>

Application::Application(std::string nameOfDatabase) : db(nameOfDatabase, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {}

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

void Application::quit()
{
    startMenuShouldClose = 1;
}

std::string Application::selectionTable()
{
    clear();
    echo();
    timeout(-1);

    mvprintw(1, 1, "Enter table name: ");
    char *bufStr = new char[64];
    
    getstr(bufStr);
    
    if(db.tableExists(bufStr)) 
    {
        return bufStr;
    }
    
    clear(); 
    mvprintw(1, 1, "Table doen't exist!");
    
    getch();
    quit();
    return "";
}

void Application::printInfoFromTable(std::string nameOfTable)
{
    if(startMenuShouldClose) return;
    
    clear();
    noecho();

    SQLite::Statement query(db, "select * from " + nameOfTable);

    int columnCount = query.getColumnCount();
    
    if(columnCount <= 0) quit();

    int offsetY = 0;

    while(query.executeStep())
    {
        for(int i = 0; i < columnCount; ++i)
        {
            SQLite::Column column(query.getColumn(i));
            std::string bufStr;
            bufStr += column.getName();
            bufStr += ": " + column.getString();

            mvprintw(1 + offsetY, 1, bufStr.c_str());
            ++offsetY;
        }
        ++offsetY;
    }
    
    refresh();
    getch();

    timeout(0);
    clear();
}

std::string Application::parse(std::vector<std::string> names, std::vector<std::string> types)
{
    std::string dataOutput;

    for(int i = 0; i < types.size(); ++i)
    {
        char *enterStr = new char[32];
        if(types[i] == "INTEGER")
        {
            std::string  bufStr = "Enter " + names[i] + "(INTEGER) - ";
            mvprintw(1 + i, 1, bufStr.c_str()); getstr(enterStr); dataOutput += enterStr; dataOutput += ", ";
        }
        else if(types[i] == "TEXT")
        {
            std::string  bufStr = "Enter " + names[i] + "(TEXT) - ";
            mvprintw(1 + i, 1, bufStr.c_str()); getstr(enterStr); dataOutput += "'"; dataOutput += enterStr; dataOutput += "', ";
        }
    }
    return dataOutput;
}

void Application::insertValuesIntoTable(std::string nameOfTable)
{
    if(startMenuShouldClose) return;

    clear();

    std::string execStr;

    SQLite::Statement typeQuery(db, "SELECT type FROM PRAGMA_TABLE_INFO('" + nameOfTable + "');");
    SQLite::Statement nameQuery(db, "SELECT name FROM PRAGMA_TABLE_INFO('" + nameOfTable + "');");

    std::vector<std::string> types;
    std::vector<std::string> names;

    while(typeQuery.executeStep())
    {
        for(int i = 0; i < typeQuery.getColumnCount(); ++i)
        {
            types.push_back(typeQuery.getColumn(i));
        }
    }
    
    while(nameQuery.executeStep())
    {
        for(int i = 0; i < nameQuery.getColumnCount(); ++i)
        {
            names.push_back(nameQuery.getColumn(i));
        }
    }

    execStr = parse(names, types);
    
    int result = db.exec("INSERT INTO t VALUES(" + execStr.substr(0, execStr.size() - 2) + ");"); 
    
    timeout(0);
    noecho();
    clear();
}

void Application::deleteInfoFromTable(std::string nameOfTable)
{
    if(startMenuShouldClose) return;

    db.exec("DELETE FROM " + nameOfTable);

    clear();
}

void Application::drawMenu(int currentElement)
{
    for(int i = 0; i < choicesForMenu.size(); ++i)
    {
        if(currentElement == i)
        {
            attron(COLOR_PAIR(SELECT_COLOR_PAIR));
            mvprintw(i, 1, choicesForMenu[i].c_str());
            attroff(COLOR_PAIR(SELECT_COLOR_PAIR));
            continue;
        }
        mvprintw(i, 1, choicesForMenu[i].c_str());
    }
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
        if(key == 10 && choicesForMenu[currentElement] == "Insert Values") insertValuesIntoTable(selectionTable());
        if(key == 10 && choicesForMenu[currentElement] == "Delete Info From Table") deleteInfoFromTable(selectionTable());

        drawMenu(currentElement);
    }
}

void Application::run()
{
    init();
    startMenu();
    endwin();
}
