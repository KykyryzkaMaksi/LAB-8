#pragma once
#include "Models.h"
#include <vector>
#include <optional>

// Абстрактний інтерфейс репозиторію для сутності RestaurantTable
class ITableRepository {
public:
    // Віртуальний деструктор для коректного виклику деструкторів похідних класів
    virtual ~ITableRepository() = default;

    // Додавання нового столу до бази даних (повертає згенерований ID)
    virtual int add(const RestaurantTable& table) = 0;

    // Пошук столу за ID (повертає std::optional на випадок, якщо запис відсутній)
    virtual std::optional<RestaurantTable> findById(int id) const = 0;

    // Отримання списку всіх столів з бази даних
    virtual std::vector<RestaurantTable> findAll() const = 0;

    // Оновлення інформації про існуючий стіл
    virtual void update(const RestaurantTable& table) = 0;

    // Видалення столу з бази даних за його ID
    virtual void remove(int id) = 0;
};