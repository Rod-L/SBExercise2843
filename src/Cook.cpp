#include <iostream>
#include <thread>

#include "Cook.h"
#include "CoutAccess.h"

Cook::Cook(int inId) {
    currentOrder = nullptr;
    working = false;
    id = inId;
}

Cook::~Cook() {
    queueChange.lock();
    ordersQueue.clear();
    queueChange.unlock();
    working = false;
}

void Cook::addOrder(Order* order) {
    queueChange.lock();
    ordersQueue.push_back(order);
    queueChange.unlock();
}

bool Cook::busy() const {
    return working;
}

void Cook::processOrders() {
    while (true) {
        if (ordersQueue.empty()) continue;
        currentOrder = ordersQueue[0];

        coutLock();
        std::cout << "Chef #" << id << " started to cook " << currentOrder->repr() << std::endl;
        coutUnlock();

        queueChange.lock();
        ordersQueue.erase(ordersQueue.begin());
        queueChange.unlock();

        int cookingTime = rand() % 11 + 5;
        while (cookingTime > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            --cookingTime;
            if (!working) return;
        }
        currentOrder->cooked = true;

        coutLock();
        std::cout << currentOrder->repr() << " have been cooked by chef #" << id << std::endl;
        coutUnlock();
    }
}

void Cook::startCooking() {
    if (!working) std::thread(&Cook::processOrders, this).detach();
    working = true;
}

int Cook::ordersAmount() const {
    return static_cast<int>(ordersQueue.size() + (currentOrder == nullptr ? 0 : 1));
}