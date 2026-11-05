#include "Storage.h"
#include "TableRepository.h"
#include "RawSqlComparison.h"
#include <iostream>
#include <windows.h>

void generateTestData(Storage& storage, TableRepository& repo) {
    // Додаємо 5 столів
    int t1 = repo.add({ 0, 1, 2, "Біля вікна" });
    int t2 = repo.add({ 0, 2, 4, "Зал" });
    int t3 = repo.add({ 0, 3, 6, "Зал" });
    int t4 = repo.add({ 0, 4, 2, "Тераса" });
    int t5 = repo.add({ 0, 5, 8, "VIP" });

    // Додаємо 15 замовлень по 3 на стіл
    for (int i = 1; i <= 5; ++i) {
        storage.insert(Order{ 0, i, "18:00", 150.50 * i, "Оплачено" });
        storage.insert(Order{ 0, i, "19:30", 300.00, "Оплачено" });
        storage.insert(Order{ 0, i, "20:45", 55.00, "Нове" });
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    try {
        auto storage = initStorage("restaurant.db");

        // Увімкнення перевірки зовнішніх ключів
        storage.on_open = [](sqlite3* db) {
            sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
            };

        // Синхронізація схеми
        storage.sync_schema();
        storage.remove_all<Order>();
        storage.remove_all<RestaurantTable>();

        TableRepository tableRepo(storage);
        generateTestData(storage, tableRepo);

        // Завдання 2: Порівняння
        runRawSqlComparison("restaurant.db");

        std::cout << "\n--- Завдання 5: Запити ---\n";

        // 1. Фільтрація
        auto terraceTables = storage.get_all<RestaurantTable>(where(c(&RestaurantTable::location) == "Тераса"));
        std::cout << "Столи на терасі: " << terraceTables.size() << "\n";

        // 2. Сортування та обмеження
        auto topOrders = storage.get_all<Order>(order_by(&Order::total_amount).desc(), limit(3));
        std::cout << "Топ 3 найдорожчих замовлення:\n";
        for (const auto& o : topOrders) {
            std::cout << " - Сума: " << o.total_amount << "\n";
        }

        // 3. Агрегація (SUM)
        auto totalRevenue = storage.select(sum(&Order::total_amount));
        if (!totalRevenue.empty() && totalRevenue.front()) {
            std::cout << "Загальна сума всіх замовлень: " << *totalRevenue.front() << "\n";
        }
        else {
            std::cout << "Загальна сума всіх замовлень: 0\n";
        }

        // 4. JOIN (Об'єднання таблиць)
        auto joined = storage.select(
            columns(&RestaurantTable::number, &Order::total_amount),
            inner_join<Order>(on(c(&RestaurantTable::id) == &Order::table_id)),
            where(c(&Order::status) == "Нове")
        );
        std::cout << "Нові замовлення (Стіл -> Сума):\n";
        for (const auto& row : joined) {
            std::cout << " - Стіл #" << std::get<0>(row) << " на суму " << std::get<1>(row) << "\n";
        }

        std::cout << "\n--- Завдання 6: Транзакції ---\n";

        // Успішна транзакція: Реєстрація нового столу і першого замовлення
        storage.transaction([&]() {
            int newTableId = tableRepo.add({ 0, 10, 4, "Зал" });
            storage.insert(Order{ 0, newTableId, "21:00", 500.00, "Нове" });
            return true;
            });
        std::cout << "Успішна транзакція виконана.\n";

        // Відкат транзакції (ROLLBACK)
        try {
            storage.transaction([&]() {
                int newTableId = tableRepo.add({ 0, 11, 2, "Тераса" });
                // Спроба додати замовлення з неіснуючим table_id (999), щоб викликати помилку Foreign Key
                storage.insert(Order{ 0, 999, "22:00", 100.00, "Нове" });
                return true;
                });
        }
        catch (const std::exception& e) {
            std::cout << "Транзакція скасована через помилку: " << e.what() << "\n";
        }

        // Перевірка атомарності: стіл 11 не повинен існувати
        auto tables = storage.get_all<RestaurantTable>(where(c(&RestaurantTable::number) == 11));
        if (tables.empty()) {
            std::cout << "Перевірка атомарності: Стіл №11 не був збережений (ROLLBACK спрацював успішно).\n";
        }

    }
    catch (const std::exception& e) {
        std::cout << "\nКРИТИЧНА ПОМИЛКА БАЗИ ДАНИХ: " << e.what() << "\n";
    }

    return 0;
}