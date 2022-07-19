#pragma once

#include <vector>
#include <mutex>
#include "Order.h"

class Cook {
    Order* currentOrder;
    bool working;
    std::mutex queueChange;
    std::vector<Order*> ordersQueue;

    void processOrders();

public:
    int id;

    explicit Cook(int inId);
    ~Cook();
    void addOrder(Order* order);
    void startCooking();
    bool busy() const;
    int ordersAmount() const;
};