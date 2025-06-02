# OrderBook

## Overview
The `OrderBook` class implements a limit order book system for financial markets. It maintains separate buy and sell sides, each properly sorted by price level, and provides methods to add, modify, and cancel orders. The implementation is designed for efficient lookups and order management.

## Class: OrderBook

### Constructor
- `OrderBook(const std::string& instrument)`: Constructs an order book for the specified financial instrument

### Public Methods
- `void addOrder(const Order& order)`: Adds a new order to the book
- `bool cancelOrder(int order_id)`: Cancels an existing order identified by its ID
- `bool modifyOrder(const Order& order)`: Modifies (fully replaces) an existing order
- `const std::string& getInstrument() const`: Returns the instrument name this order book is for
- `const std::map<double, std::list<Order>, std::greater<double>>& getBuySide() const`: Returns the buy side of the book (sorted high to low)
- `const std::map<double, std::list<Order>>& getSellSide() const`: Returns the sell side of the book (sorted low to high)

### Private Members
- `std::string instrument`: The name of the financial instrument this book is for
- `std::map<double, std::list<Order>, std::greater<double>> buy_orders`: Buy side orders sorted by price (high to low)
- `std::map<double, std::list<Order>> sell_orders`: Sell side orders sorted by price (low to high)
- `std::unordered_map<int, std::pair<Side, std::list<Order>::iterator>> order_lookup`: Hash map for quick order lookup by ID

### Private Methods
- `std::map<double, std::list<Order>, std::greater<double>>& getBuyOrderMap()`: Returns reference to the buy order map
- `std::map<double, std::list<Order>>& getSellOrderMap()`: Returns reference to the sell order map
- `std::map<double, std::list<Order>, std::greater<double>>& getOrderMap(Side side)`: Helper to get the appropriate map by side (with limitations)

## Key Features
1. **Price Level Organization**: Orders are organized by price levels, with multiple orders at the same price level stored in a list
2. **Efficient Lookup**: The `order_lookup` hash map enables O(1) lookup by order ID for fast modification and cancellation
3. **Price-Time Priority**: Orders at the same price level are maintained in the order they were added (time priority)
4. **Different Sorting for Buy/Sell**: 
   - Buy side is sorted from highest to lowest price (best bids first)
   - Sell side is sorted from lowest to highest price (best asks first)

## Implementation Details
The `OrderBook` uses different sorting criteria for its buy and sell sides:
- `buy_orders` uses the `std::greater<double>` comparator to sort prices from high to low
- `sell_orders` uses the default `std::less<double>` comparator to sort prices from low to high

This design allows for easy access to the best prices on either side (highest bid, lowest ask).

## Usage Example
```cpp
// Create an order book for AAPL
OrderBook book("AAPL");

// Add a buy order
Order buy_order = {
    .timestamp = 123456789,
    .order_id = 1,
    .instrument = "AAPL",
    .side = Side::BUY,
    .type = Type::LIMIT,
    .quantity = 100,
    .price = 150.0,
    .action = Action::NEW
};
book.addOrder(buy_order);

// Add a sell order
Order sell_order = {
    .timestamp = 123456790,
    .order_id = 2,
    .instrument = "AAPL",
    .side = Side::SELL,
    .type = Type::LIMIT,
    .quantity = 50,
    .price = 151.0,
    .action = Action::NEW
};
book.addOrder(sell_order);

// Get the current state of the book
auto& buy_side = book.getBuySide();
auto& sell_side = book.getSellSide();

// Cancel an order
bool success = book.cancelOrder(1);

// Modify an order
Order modified = sell_order;
modified.quantity = 75;
modified.price = 152.0;
book.modifyOrder(modified);
```
