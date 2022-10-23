#include "../inc/Menu.h"

Menu::Menu(std::string nameOfTable) : db(nameOfTable, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {}

void Menu::quit()
{
  startMenuShouldClose = true;
}

std::string Menu::selectionTable()
{
  timeout(-1);

  clear();
  echo();

  SQLite::Statement query(db, "select name from sqlite_master where type == 'table';");
  
  std::string strAllTables;

  while(query.executeStep())
  {
    for(int i = 0; i < query.getColumnCount(); ++i)
    {   
      SQLite::Column column = query.getColumn(i);
      strAllTables += column.getString() + " ";
    }
  }

  if(strAllTables.empty())
  {
    clear();
    mvprintw(0, 1, "No Tables Created");

    getch();
    quit();
    return "";
  }

  mvprintw(0, 1, strAllTables.c_str());
  mvprintw(2, 1, "Enter table name: ");
  char *bufStr = new char[64];
  
  getstr(bufStr);
  
  if(!db.tableExists(bufStr)) 
  {
    clear(); 
    mvprintw(1, 1, "Table doen't exist!");
  
    getch();
    quit();
    return "";
  }
  
  return bufStr;
}

void Menu::printInfoFromTable(std::string nameOfTable)
{
  if(startMenuShouldClose) return;
    
  clear();
  noecho();

  SQLite::Statement query(db, "select * from " + nameOfTable);
  
  int columnCount = query.getColumnCount();

  int offsetY = 0;

  bool wasExecuted = false;

  while(query.executeStep())
  {
    if(!wasExecuted) wasExecuted = true;

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
  
  if(wasExecuted == false)
  {
    mvprintw(1, 1, "No Information");
  }

  refresh();
  getch();

  clear();
}

std::string Menu::parseTypes(std::vector<std::string> names, std::vector<std::string> types)
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

void Menu::insertValuesIntoTable(std::string nameOfTable)
{
  if(startMenuShouldClose) return;

  clear();

  std::string execStr;
  
  std::vector<std::string> names;
  std::vector<std::string> types;

  SQLite::Statement query(db, "select * from " + nameOfTable + ";");

  int columnsCount = query.getColumnCount();

  query.executeStep();
  for(int i = 0; i < columnsCount; ++i)
  {
    names.push_back(query.getColumnName(i));
    types.push_back(query.getColumnDeclaredType(i));
  }
  
  execStr = parseTypes(names, types);

  db.exec("insert into " + nameOfTable + " values(" + execStr.substr(0, execStr.size() - 2) + ");"); 
  
  noecho();
  clear();
}

void Menu::deleteInfoFromTable(std::string nameOfTable)
{
  if(startMenuShouldClose) return;

  db.exec("DELETE FROM " + nameOfTable);

  clear();
}

void Menu::drawMenu(int currentElement)
{
  for(int i = 0; i < choicesForMenu.size(); ++i)
  {
    if(currentElement == i)
    {
      attron(COLOR_PAIR(1));
      mvprintw(i, 1, choicesForMenu[i].c_str());
      attroff(COLOR_PAIR(1));
      continue;
    }
    mvprintw(i, 1, choicesForMenu[i].c_str());
  }
}


void Menu::startMenu()
{
  int minElement = 0;
  int currentElement = minElement;
  int maxElement = choicesForMenu.size();

  init_pair(1, COLOR_BLACK, COLOR_WHITE);

  while(!startMenuShouldClose)
  {
    timeout(0);

    int key = getch();
    
    if(key == KEY_UP && currentElement != minElement) --currentElement;
    if(key == KEY_DOWN && currentElement != maxElement) ++currentElement;

    if(key == 10 && choicesForMenu[currentElement] == "Quit")
    {
      quit();
    }
    if(key == 10 && choicesForMenu[currentElement] == "Print All")
    {
      printInfoFromTable(selectionTable());
    }
    if(key == 10 && choicesForMenu[currentElement] == "Insert Values")
    {
      insertValuesIntoTable(selectionTable());
    }
    if(key == 10 && choicesForMenu[currentElement] == "Delete Info From Table")
    {
      deleteInfoFromTable(selectionTable());
    }

    drawMenu(currentElement);
  }
}

