# Limit Order Book (LOB)

A high-performance C++ implementation of a Limit Order Book, designed to efficiently match buy and sell orders.

## 🚀 Project Roadmap

### Phase 1: Foundation (✅ Completed)
- Unique IDs for tracking individual orders.
- Input Validation (rejecting non-positive prices and invalid quantities).
- Standard display logic to print the depth of the book (bids, asks, and spread).

### Phase 2: Speed & Performance (✅ Completed)
- Implement $O(1)$ order cancellation using `std::unordered_map` and `std::list` iterators.
- Optimize memory overhead (pass-by-reference and `std::move`).

### Phase 3: Stress Testing (✅ Completed)
- Latency benchmarking using the `<chrono>` library.
- Run a 1,000,000 order liquidity stress test.
- Integrate a 100,000 operations randomized fuzz test.

### Phase 4: Polish & Deployment (✅ Completed)
- Clean up the code structure.
- Push the final version to GitHub.

---

## 📖 Developer Manual & Code Breakdown

This section explains the core components of the codebase as they stand in Phase 1 and what they do.

### 1. `Order.h`
This file contains the fundamental data structures that make up an order.
* **`Side` Enum**: An enumeration (`enum class`) representing whether an order is a `Buy` or a `Sell`.
* **`Order` Struct**: Represents a single order in the system. It contains:
  * `id`: A unique 64-bit integer identifier for the order.
  * `side`: The side of the order (Buy/Sell).
  * `price`: A 32-bit integer representing the price limit.
  * `quantity`: A 64-bit integer representing the number of units to trade.

### 2. `OrderBook.h`
Defines the `OrderBook` class which is the central engine of the project.
* **Storage mechanism**: Orders are grouped by price. 
  * `asks` (Sells): Stored in a `std::map<int32_t, std::list<Order>>`. The default behavior of `std::map` sorts keys (prices) in ascending order, ensuring the lowest ask price is at the top of the book.
  * `bids` (Buys): Stored in a `std::map<int32_t, std::list<Order>, std::greater<int32_t>>`. The `std::greater` comparator ensures that keys are sorted in descending order, meaning the highest bid price is at the top.
* **`orderMap` & `OrderLocation`**: An `std::unordered_map` that acts as a hash map directly mapping a unique `Order ID` (uint64_t) to an `OrderLocation` struct containing the order's `side`, `price`, and crucially, a `std::list<Order>::iterator`. This allows for true $O(1)$ time complexity when looking up an order's location and deleting it directly from the inner linked list.
* **`nextId`**: An internal counter used to assign unique IDs sequentially as new orders arrive.

### 3. `OrderBook.cpp`
This is where the business logic is implemented.
* **`addOrder(Side side, int32_t price, uint64_t quantity)`**:
  * **Validation**: It first checks if the price is positive and the quantity is within valid limits ($0 < \text{quantity} \le 1,000,000,000$).
  * **Order Creation**: A new `Order` object is created and assigned a unique ID.
  * **Matching Logic (The core engine)**:
    * If a **Buy** order arrives, it attempts to match with existing **Sells** (`asks`). It continues matching as long as the incoming order has remaining quantity and the best ask price is less than or equal to the buy order's price limit.
    * If a **Sell** order arrives, it attempts to match with existing **Buys** (`bids`). It continues matching as long as there is quantity left and the best bid price is greater than or equal to the sell order's price limit.
    * Whenever a sitting order is completely filled (`sittingOrder.quantity == 0`), it is erased from its queue and also erased from `orderMap` to free up space.
  * **Resting Orders**: If the incoming order cannot be completely filled (quantity > 0 after all possible matches), the remainder is added to the relevant side of the book as a "resting order", stored in a `std::list` to maintain time-priority within that price level. `std::move(order)` is now used to place the resting order in the data structure, preventing expensive object copies. The resting order's coordinates (`side`, `price`, and `iterator`) are simultaneously recorded in the `orderMap`.
* **`deleteOrder(uint64_t orderId)`**:
  * Leverages the `orderMap` to achieve $O(1)$ fast lookup. Instead of searching linearly across the entire order book or even the specific price queue, it instantly fetches the iterator pointer saved in `OrderLocation`.
  * The order is immediately erased passing the cached iterator directly to `std::list::erase`. If the queue is depleted by the removal, it cleans up the empty price level in the tree map. This effectively eliminates all $O(N)$ overheads.
* **`display()`**: A utility function that prints the current state of the order book. Ascending order for asks, followed by the spread, and descending order for bids. 

### 4. `main.cpp`
The main entry point of the project acts as a clean, functional step-by-step tutorial. It creates an `OrderBook` instance and submits a scenario of test orders to demonstrate liquidity provision (adding orders that rest), liquidity taking (crossing the spread to match existing orders), and a demonstration of the $O(1)$ fast cancellation.

### 5. `benchmarks` & `tests`
The project's scale testing has been logically split:
* **`benchmarks/latency_test.cpp`**: Contains a 1,000,000 order sequence generating a tight spread to aggressively benchmark the latency of the matching calculation and insertions across the `std::map`. Prints out the average fractional microseconds computing time for each order.
* **`tests/fuzz_test.cpp`**: Generates a massive 100,000 operation fuzz-test that randomly applies Adds, Crosing Matches, and Deletions on random ID’s. This runs without output to strictly test memory stability, crashes, and exceptions over an extended simulation. 

## 🎉 Project Completion (All Phases Done)
The Limit Order Book project has successfully concluded its final phase! 

Through extensive iterative development, the engine has moved from a basic foundational logic system into a professionally structured, high-frequency $O(1)$ optimized framework. Benchmarks prove out a highly robust system resolving individual orders dynamically within a fraction of a microsecond while maintaining complete exception-free memory stability under simulated massive operation fuzz tests. 

The codebase has been refactored, the debug prints have been insulated into test beds, and the engine stands ready for deployment. The goal of this roadmap is fully complete!
