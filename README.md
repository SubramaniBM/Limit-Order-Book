# Limit Order Book (LOB)

A high-performance C++ implementation of a Limit Order Book, designed to efficiently match buy and sell orders.

## 🚀 Project Roadmap

### Phase 1: Foundation (✅ Completed)
- Unique IDs for tracking individual orders.
- Input Validation (rejecting non-positive prices and invalid quantities).
- Standard display logic to print the depth of the book (bids, asks, and spread).

### Phase 2: Speed & Performance (📍 You are here)
- Implement $O(1)$ order cancellation using `std::unordered_map`.
- Optimize memory overhead (pass-by-reference).

### Phase 3: Stress Testing
- Latency benchmarking using the `<chrono>` library.
- Run a 1,000,000 order liquidity stress test.

### Phase 4: Polish & Deployment
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
  * `asks` (Sells): Stored in a `std::map<int32_t, std::vector<Order>>`. The default behavior of `std::map` sorts keys (prices) in ascending order, ensuring the lowest ask price is at the top of the book.
  * `bids` (Buys): Stored in a `std::map<int32_t, std::vector<Order>, std::greater<int32_t>>`. The `std::greater` comparator ensures that keys are sorted in descending order, meaning the highest bid price is at the top.
* **`nextId`**: An internal counter used to assign unique IDs sequentially as new orders arrive.

### 3. `OrderBook.cpp`
This is where the business logic is implemented.
* **`addOrder(Side side, int32_t price, uint64_t quantity)`**:
  * **Validation**: It first checks if the price is positive and the quantity is within valid limits ($0 < \text{quantity} \le 1,000,000,000$).
  * **Order Creation**: A new `Order` object is created and assigned a unique ID.
  * **Matching Logic (The core engine)**:
    * If a **Buy** order arrives, it attempts to match with existing **Sells** (`asks`). It continues matching as long as the incoming order has remaining quantity and the best ask price is less than or equal to the buy order's price limit.
    * If a **Sell** order arrives, it attempts to match with existing **Buys** (`bids`). It continues matching as long as there is quantity left and the best bid price is greater than or equal to the sell order's price limit.
  * **Resting Orders**: If the incoming order cannot be completely filled (quantity > 0 after all possible matches), the remainder is added to the relevant side of the book as a "resting order", stored in a `std::vector` to maintain time-priority within that price level.
* **`display()`**: A utility function that prints the current state of the order book. Ascending order for asks, followed by the spread, and descending order for bids. 

### 4. `main.cpp`
The entry point of the application. It creates an `OrderBook` instance and submits a series of test orders to demonstrate liquidity provision (adding orders that rest) and liquidity taking (crossing the spread to match existing orders).

## 🛠️ Next Steps (Phase 2)
Currently, order cancellation is not supported, and vectors require $O(N)$ removal shifting. As outlined in the roadmap, our next objective is to integrate an `std::unordered_map` that maps `Order ID -> pointer/iterator` to locate any order instantly. We will also optimize the data structures and matching engine to eliminate unnecessary copies.
