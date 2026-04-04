#include <iostream>
#include <chrono>
#include <random>
#include "../OrderBook.h"

int main()
{
    OrderBook book;
    const int NUM_ORDERS = 1000000;

    std::cout << "--- Phase 3: Stress Testing ---\n";
    std::cout << "Running " << NUM_ORDERS << " order liquidity stress test...\n";

    // Random number generation for prices and quantities
    std::mt19937 gen(42); // Deterministic seed for reproducible testing
    std::uniform_int_distribution<> side_dist(0, 1);
    std::uniform_int_distribution<int32_t> price_dist(90, 110); // Tight spread for max matching
    std::uniform_int_distribution<uint64_t> qty_dist(1, 100);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_ORDERS; ++i)
    {
        Side side = (side_dist(gen) == 0) ? Side::Buy : Side::Sell;
        int32_t price = price_dist(gen);
        uint64_t qty = qty_dist(gen);
        book.addOrder(side, price, qty);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    auto duration_micros = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "Stress test completed in " << duration_ms.count() << " ms.\n";
    std::cout << "Average time per order: " << (double)duration_micros.count() / NUM_ORDERS << " microseconds.\n";

    return 0;
}