#pragma once

#include <vector>
#include <mutex>
#include <thread>

#include "Order.h"
#include "Cook.h"

class Restaurant{
    int ordersAccepted;
    int ordersServed;
    bool working;
    std::mutex queueChange;
    std::vector<Order*> ordersQueue;
    std::vector<Cook*> cooks;

    std::thread ordersProcessing;
    std::thread deliveryProcessing;

    static uint64_t timestamp();
    void assignCook(Order* order);
    void threadAcceptOrder();
    void threadDeliverDishes();

public:
    explicit Restaurant(int cooksCount);
    ~Restaurant();
    void acceptOrder();
    void deliverDishes();
    void startWorking();
};