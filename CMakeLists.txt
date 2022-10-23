cmake_minimum_required(VERSION 3.0)

project(sqlite-ncurses)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SQLITECPP_RUN_CPPCHECK OFF CACHE BOOL "" FORCE)
set(SQLITECPP_RUN_CPPLINT OFF CACHE BOOL "" FORCE)

project(sqlite-ncurses)

add_subdirectory(SQLiteCpp)

add_executable(run src/Main.cpp src/Menu.cpp src/Application.cpp)

target_link_libraries(run
  SQLiteCpp
  sqlite3
  pthread
  dl
  ncursesw
  )

