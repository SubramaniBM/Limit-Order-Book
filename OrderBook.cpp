
#include "OrderBook.h"
#include <iostream>
#include <algorithm>

void OrderBook::addOrder(Side side, int32_t price, uint64_t quantity)
{
    if (price <= 0)
    {
        std::cout << "REJECTED: Price must be positive.\n";
        return;
    }
    if (quantity == 0 || quantity > 1000000000)
    {
        std::cout << "REJECTED: Invalid Quantity.\n";
        return;
    }

    Order order{nextId++, side, price, quantity};
    if (order.side == Side::Buy)
    {
        while (order.quantity > 0 && asks.size())
        {
            auto it = asks.begin();
            int32_t bestAskPrice = it->first;
            auto &ordersAtPrice = it->second;
            if (bestAskPrice > order.price)
                break;
            while (order.quantity > 0 && ordersAtPrice.size())
            {   
                auto &sittingOrder = ordersAtPrice[0];
                uint64_t matchqty = std::min(order.quantity, sittingOrder.quantity);
                std::cout << "TRADE: Order " << order.id << " matched with " << sittingOrder.id << " for " << matchqty << " units at price " << bestAskPrice << std::endl;
                order.quantity -= matchqty, sittingOrder.quantity -= matchqty;
                if (!sittingOrder.quantity)
                    ordersAtPrice.erase(ordersAtPrice.begin());
                if (ordersAtPrice.empty())
                    asks.erase(bestAskPrice);
            }
        }
        if (order.quantity > 0)
            bids[order.price].push_back(order);
    }
    else
    {
        while (order.quantity > 0 && bids.size())
        {
            auto it = bids.begin();
            int32_t bestBidPrice = it->first;
            auto &ordersAtPrice = it->second;
            if (order.price > bestBidPrice)
                break;
            while (order.quantity > 0 && ordersAtPrice.size())
            {
                auto &sittingOrder = ordersAtPrice[0];
                uint64_t matchqty = std::min(order.quantity, sittingOrder.quantity);
                std::cout << "TRADE: Order " << order.id << " matched with " << sittingOrder.id << " for " << matchqty << " units at price " << bestBidPrice << std::endl;
                order.quantity -= matchqty, sittingOrder.quantity -= matchqty;
                if (!sittingOrder.quantity)
                    ordersAtPrice.erase(ordersAtPrice.begin());
                if (ordersAtPrice.empty())
                    bids.erase(bestBidPrice);
            }
        }
        if (order.quantity > 0)
            asks[order.price].push_back(order);
    }
}

void OrderBook::display()
{
    std::cout << "-------------------LIMIT ORDER BOOK-------------------\n\n";
    std::cout << "-----ASKS(SELLS)-----\n";
    if (asks.empty())
        std::cout << "[EMPTY]\n";
    else
    {
        for (auto it = asks.rbegin(); it != asks.rend(); ++it)
        {
            std::cout << "Price:" << it->first << "\n";
            for (const auto &o : it->second)
            {
                std::cout << "    >>" << "[Order ID: " << o.id << "] Quantity: " << o.quantity << "\n";
            }
        }
    }
    std::cout << "-------------------SPREAD-------------------\n";
    std::cout << "-----BIDS(BUYS)-----\n";
    if (bids.empty())
        std::cout << "[EMPTY]\n";
    else
    {
        for (const auto &it : bids)
        {
            std::cout << "Price:" << it.first << "\n";
            for (const auto &o : it.second)
            {
                std::cout << "    >>" << "[Order ID: " << o.id << "] Quantity: " << o.quantity << "\n";
            }
        }
    }
}
