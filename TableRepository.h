#pragma once
#include "ITableRepository.h"
#include "Storage.h"

// Похідний клас, який реалізує інтерфейс ITableRepository для роботи з SQLite через sqlite_orm
class TableRepository : public ITableRepository {
    Storage& storage;

public:
    // Конструктор, що приймає посилання на ORM-сховище
    explicit TableRepository(Storage& storage) : storage(storage) {}

    // Оголошення перевизначеного методу додавання нового столу
    int add(const RestaurantTable& table) override;

    // Оголошення перевизначеного методу пошуку столу за його ID
    std::optional<RestaurantTable> findById(int id) const override;

    // Оголошення перевизначеного методу отримання списку всіх столів
    std::vector<RestaurantTable> findAll() const override;

    // Оголошення перевизначеного методу оновлення даних існуючого столу
    void update(const RestaurantTable& table) override;

    // Оголошення перевизначеного методу видалення столу за його ID
    void remove(int id) override;
};