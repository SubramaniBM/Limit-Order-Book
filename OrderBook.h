#pragma once
#include "Order.h"
#include <map>
#include <vector>
#include <functional> //For Bids Sorting
#include<unordered_map>

class OrderBook
{
    private:
    struct OrderLocation{
        Side side;
        int32_t price;
    };
    std::unordered_map<uint64_t, struct OrderLocation> orderMap;
    std::map<int32_t, std::vector<Order>> asks;
    std::map<int32_t, std::vector<Order>, std::greater<int32_t>> bids;
    uint64_t nextId=1;

public:
    void addOrder(Side side,int32_t price,uint64_t quantity);
    void display();
};
