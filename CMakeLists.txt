cmake_minimum_required(VERSION 3.20)
project(Lab8_Restaurant CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)
find_package(SqliteOrm CONFIG REQUIRED)

add_executable(Lab8_Restaurant 
    main.cpp 
    TableRepository.cpp 
    RawSqlComparison.cpp
)

target_link_libraries(Lab8_Restaurant PRIVATE SQLite::SQLite3 sqlite_orm::sqlite_orm)