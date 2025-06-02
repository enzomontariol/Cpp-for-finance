# Order

## Overview
The `Order` header file defines the fundamental order structure and enumerations used throughout the trading system. It provides the basic data types for representing financial orders in the system.

## Enumerations

### Side
Represents the side of an order:
- `BUY`: Buy order (bid)
- `SELL`: Sell order (ask)

### Type
Represents the type of order:
- `MARKET`: Market order (executed at best available price)
- `LIMIT`: Limit order (executed at specified price or better)

### Action
Represents the action to be performed for an order:
- `NEW`: Create a new order
- `MODIFY`: Modify an existing order
- `CANCEL`: Cancel an existing order

## Structures

### Order
A structure that represents a financial trading order.

#### Members
- `timestamp`: 64-bit unsigned integer representing the time when the order was created or modified
- `order_id`: Integer identifier for the order
- `instrument`: String representing the financial instrument (e.g., stock symbol)
- `side`: Side of the order (BUY or SELL)
- `type`: Type of the order (MARKET or LIMIT)
- `quantity`: Integer representing the quantity of the financial instrument
- `price`: Floating-point value representing the price of the order
- `action`: Action to be performed (NEW, MODIFY, or CANCEL)

## Usage Examples
```cpp
// Create a new buy order
Order buy_order = {
    .timestamp = 1622631600000, // June 2, 2023 00:00:00
    .order_id = 12345,
    .instrument = "AAPL",
    .side = Side::BUY,
    .type = Type::LIMIT,
    .quantity = 100,
    .price = 175.5f,
    .action = Action::NEW
};

// Create a sell order
Order sell_order = {
    .timestamp = 1622631600000,
    .order_id = 12346,
    .instrument = "AAPL",
    .side = Side::SELL,
    .type = Type::LIMIT,
    .quantity = 50,
    .price = 176.0f,
    .action = Action::NEW
};

// Modify an existing order
Order modified_order = original_order;
modified_order.quantity = 150;
modified_order.price = 176.0f;
modified_order.action = Action::MODIFY;
```
