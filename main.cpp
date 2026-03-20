#include <iostream>
#include "OrderBook.h"

int main()
{
    OrderBook book;
    std::cout << "Adding initial orders...\n";
    book.addOrder(Side::Buy, 100, 10);
    book.addOrder(Side::Sell, 110, 5);
    book.display();
    std::cout << "Adding incoming orders...\n";
    book.addOrder(Side::Sell, 100, 5);
    book.addOrder(Side::Buy, 100, 5);
    book.display();
    return 0;
}