#include "Order.h"

const Dish dishOrder[5] = {pizza, soup, steak, salad, sushi};
const std::string dishNames[5] = {"pizza", "soup", "steak", "salad", "sushi"};

Order::Order(int id) {
    orderId = id;
    cookAssigned = false;
    cooked = false;
    delivered = false;
    dishId = rand() % 5;
    dish = dishOrder[dishId];
}

std::string Order::repr() const {
    std::string result;
    result.append(dishNames[dishId]);
    result.append(" #");
    result.append(std::to_string(orderId));
    return result;
}