#pragma once
#include <string>

struct RestaurantTable {
    int id = 0;
    int number;             // Номер столу
    int capacity;           // Кількість місць
    std::string location;   // Розташування (наприклад, "Тераса", "Зал")
};

struct Order {
    int id = 0;
    int table_id = 0;       // Зовнішній ключ
    std::string order_time; // Час замовлення
    double total_amount;    // Сума замовлення
    std::string status;     // Статус ("Нове", "Оплачено")
};