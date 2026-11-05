#pragma once
#include "Models.h"
#include <sqlite_orm/sqlite_orm.h>

using namespace sqlite_orm;

inline auto initStorage(const std::string& path) {
    return make_storage(path,
        // Опис таблиці "RestaurantTables" (Основна сутність)
        make_table("RestaurantTables",
            make_column("id", &RestaurantTable::id, primary_key().autoincrement()), // Первинний ключ з автоінкрементом
            make_column("number", &RestaurantTable::number),                       // Номер столу
            make_column("capacity", &RestaurantTable::capacity),                   // Кількість місць
            make_column("location", &RestaurantTable::location)                    // Локація столу
        ),
        // Опис таблиці "Orders" (Залежна сутність)
        make_table("Orders",
            make_column("id", &Order::id, primary_key().autoincrement()),          // Первинний ключ з автоінкрементом
            make_column("table_id", &Order::table_id),                             // Поле зовнішнього ключа
            make_column("order_time", &Order::order_time),                         // Час створення замовлення
            make_column("total_amount", &Order::total_amount),                     // Сума замовлення
            make_column("status", &Order::status),                                 // Статус замовлення
            foreign_key(&Order::table_id).references(&RestaurantTable::id)
        )
    );
}

// Визначення псевдоніма типу Storage для зручного використання об'єкта сховища в інших файлах проєкту
using Storage = decltype(initStorage(""));