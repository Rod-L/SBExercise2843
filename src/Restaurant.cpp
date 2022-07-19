#include <iostream>
#include <thread>

#include "Restaurant.h"
#include "CoutAccess.h"

uint64_t Restaurant::timestamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Restaurant::assignCook(Order* order) {
    if (order->cookAssigned) return;

    Cook* selected = cooks[0];
    for (int i = 1; i < cooks.size(); ++i) {
        auto cook = cooks[i];
        if (cook->ordersAmount() < selected->ordersAmount()) selected = cook;
    }
    selected->addOrder(order);
    order->cookAssigned = true;
    selected->startCooking();
}

void Restaurant::threadAcceptOrder() {
    while (working) {
        acceptOrder();
        uint64_t endStamp = timestamp() + (rand() % 6 + 5) * 1000;
        while (timestamp() < endStamp) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            if (!working) return;
        }
    }
}

void Restaurant::threadDeliverDishes() {
    while (working) {
        deliverDishes();
        uint64_t endStamp = timestamp() + 30000;
        while (timestamp() < endStamp) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            if (!working) return;
        }
    }
}

Restaurant::Restaurant(int cooksCount) {
    ordersAccepted = 0;
    ordersServed = 0;
    working = false;

    for (int i = 0; i < cooksCount; ++i) {
        auto cook = new Cook(i + 1);
        cooks.push_back(cook);
    }
}

Restaurant::~Restaurant() {
    working = false;

    for (auto cook : cooks) delete cook;
    cooks.clear();

    for (auto order : ordersQueue) delete order;
    ordersQueue.clear();

    if (ordersProcessing.joinable()) ordersProcessing.join();
    if (deliveryProcessing.joinable()) deliveryProcessing.join();
}

void Restaurant::acceptOrder() {
    auto order = new Order(ordersAccepted);
    queueChange.lock();
    ordersQueue.push_back(order);
    queueChange.unlock();
    assignCook(order);
    ++ordersAccepted;

    coutLock();
    std::cout << "<< Accepted order: " << order->repr() << std::endl;
    coutUnlock();
}

void Restaurant::deliverDishes() {
    if (ordersQueue.empty()) return;

    coutLock();
    std::cout << "Courier arrives." << std::endl;
    coutUnlock();

    queueChange.lock();
    auto it = ordersQueue.end();
    do {
        --it;
        Order* order = *it;
        if (order->cooked && !order->delivered) {
            order->delivered = true;

            coutLock();
            std::cout << ">> " << order->repr() << " have been sent to client." << std::endl;
            coutUnlock();

            ordersQueue.erase(it);
            delete order;
            ++ordersServed;
        }
    } while(it != ordersQueue.begin());
    queueChange.unlock();

    coutLock();
    std::cout << "Total orders completed: " << ordersServed << std::endl;
    coutUnlock();
}

void Restaurant::startWorking() {
    working = true;
    ordersProcessing = std::thread(&Restaurant::threadAcceptOrder, this);
    deliveryProcessing = std::thread(&Restaurant::threadDeliverDishes, this);

    while (ordersServed < 10) {
        std::this_thread::sleep_for(std::chrono::seconds(4));
    }
    working = false;
}
