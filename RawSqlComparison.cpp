#include "RawSqlComparison.h"
#include <sqlite3.h>
#include <iostream>

void runRawSqlComparison(const std::string& db_name) {
    std::cout << "\n--- Завдання 2: Порівняння C API та ORM ---\n";
    sqlite3* db;
    if (sqlite3_open(db_name.c_str(), &db) != SQLITE_OK) return;

    // 1. RAW API: Додавання
    const char* insert_sql = "INSERT INTO RestaurantTables (number, capacity, location) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, 99);
    sqlite3_bind_int(stmt, 2, 4);
    sqlite3_bind_text(stmt, 3, "VIP", -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    int new_id = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);
    std::cout << "RAW API: Додано стіл з ID " << new_id << "\n";

    // 2. RAW API: Отримання
    const char* select_sql = "SELECT number, location FROM RestaurantTables WHERE id = ?;";
    sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, new_id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "RAW API: Знайдено стіл #" << sqlite3_column_int(stmt, 0)
            << ", Локація: " << sqlite3_column_text(stmt, 1) << "\n";
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}