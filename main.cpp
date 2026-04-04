#include <iostream>
#include "OrderBook.h"

int main()
{
    OrderBook book;

    std::cout << "\n[1] Adding initial liquidity to the order book...\n";
    book.addOrder(Side::Buy, 100, 10);
    book.addOrder(Side::Buy, 99, 5);
    book.addOrder(Side::Sell, 105, 10);
    book.addOrder(Side::Sell, 106, 5);
    book.display();

    std::cout << "\n[2] Executing a matching crossing trade...\n";
    book.addOrder(Side::Buy, 105, 5);
    book.display();

    std::cout << "\n[3] Testing O(1) Cancellation Logic...\n";
    std::cout << "Attempting to cancel Order ID 2 (Buy at 99 for 5 units)...\n";
    book.deleteOrder(2);

    std::cout << "\n[4] Final Order Book State:\n";
    book.display();

    return 0;
}