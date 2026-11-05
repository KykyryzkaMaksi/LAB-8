#include "TableRepository.h"

// Реалізація додавання нового столу до БД. Засобами sqlite_orm виконується операція INSERT, що повертає згенерований ID
int TableRepository::add(const RestaurantTable& table) {
    return storage.insert(table);
}

// Пошук столу за ID. Засобами sqlite_orm виконується get_optional, що безпечно повертає std::optional (об'єкт або std::nullopt)
std::optional<RestaurantTable> TableRepository::findById(int id) const {
    return storage.get_optional<RestaurantTable>(id);
}

// Отримання всіх записів столів. Викликом get_all матеріалізується повна колекція записів у вигляді std::vector
std::vector<RestaurantTable> TableRepository::findAll() const {
    return storage.get_all<RestaurantTable>();
}

// Оновлення полів існуючого столу. Виконується операція UPDATE в базі даних за первинним ключем об'єкта
void TableRepository::update(const RestaurantTable& table) {
    storage.update(table);
}

// Видалення столу з БД за первинним ключем (ID). Виконується відповідна операція DELETE
void TableRepository::remove(int id) {
    storage.remove<RestaurantTable>(id);
}