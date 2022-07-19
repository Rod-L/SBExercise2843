#include <iostream>
#include "Restaurant.h"

int main() {
    std::cout << "How many chefs do work in restaurant?" << std::endl;
    int cooksCount;
    std::cin >> cooksCount;

    auto restaurant = new Restaurant(cooksCount);
    restaurant->startWorking();
    delete restaurant;
}
