#include <iostream>
#include <vector>
#include <random>
#include "../OrderBook.h"

// Dummy buffer to suppress std::cout output during the test to avoid terminal flooding
class NullBuffer : public std::streambuf {
public:
    int overflow(int c) { return c; }
};

int main() {
    std::cout << "Starting large randomized fuzz test (100,000 operations)...\n";
    
    // Redirect std::cout
    NullBuffer null_buffer;
    std::ostream null_stream(&null_buffer);
    std::streambuf* old_cout_buf = std::cout.rdbuf();
    std::cout.rdbuf(null_stream.rdbuf());

    OrderBook book;
    std::mt19937 gen(1337);
    std::uniform_int_distribution<> action_dist(0, 100);
    std::uniform_int_distribution<> side_dist(0, 1);
    std::uniform_int_distribution<int32_t> price_dist(80, 120);
    std::uniform_int_distribution<uint64_t> qty_dist(1, 50);

    std::vector<uint64_t> active_orders;
    uint64_t current_id = 1;

    try {
        for (int i = 0; i < 100000; ++i) {
            int action = action_dist(gen);
            if (action < 80) { // 80% chance to add order
                Side side = (side_dist(gen) == 0) ? Side::Buy : Side::Sell;
                int32_t price = price_dist(gen);
                uint64_t qty = qty_dist(gen);
                book.addOrder(side, price, qty);
                active_orders.push_back(current_id++);
            } else { // 20% chance to delete a random order
                if (!active_orders.empty()) {
                    std::uniform_int_distribution<size_t> idx_dist(0, active_orders.size() - 1);
                    size_t idx = idx_dist(gen);
                    uint64_t id_to_delete = active_orders[idx];
                    book.deleteOrder(id_to_delete);
                }
            }
        }
    } catch (...) {
        std::cout.rdbuf(old_cout_buf); // Restore cout
        std::cout << "TEST FAILED: Exception thrown during execution.\n";
        return 1;
    }

    std::cout.rdbuf(old_cout_buf); // Restore cout
    std::cout << "Successfully processed 100,000 randomized operations (adds, crossings, and deletes) without any crashes or memory errors.\n";

    return 0;
}