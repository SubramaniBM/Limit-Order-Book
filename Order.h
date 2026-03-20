#pragma once       // To prevent compiler from reading definitions twice
#include <cstdint> //For uint64_t and int32_t, fixed width integer types

enum class Side
{
    Buy,
    Sell
};

struct Order
{
    uint64_t id;
    Side side;
    int32_t price;
    uint64_t quantity;
};
