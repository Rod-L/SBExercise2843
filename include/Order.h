#pragma once

#include <string>

enum Dish {
    pizza,
    soup,
    steak,
    salad,
    sushi
};

class Order {
    int orderId;
    int dishId;
    Dish dish;

public:
    bool cookAssigned;
    bool cooked;
    bool delivered;

    explicit Order(int id);
    std::string repr() const;
};