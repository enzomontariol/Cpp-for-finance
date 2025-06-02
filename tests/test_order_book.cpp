#include "../src/order_book.hpp"
#include <iostream>
#include <cassert>

// Simple test harness function
#define TEST(name) void test_##name()

// Assertion with message
#define ASSERT_TRUE(condition, message) \
    if (!(condition)) { \
        std::cerr << "Assertion failed: " << message << std::endl; \
        exit(1); \
    }

TEST(order_book_basic) {
    OrderBook book("AAPL");
    
    // Test basic properties
    ASSERT_TRUE(book.getInstrument() == "AAPL", "Instrument should be AAPL");
    
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
    
    // Check buy side
    auto& buy_side = book.getBuySide();
    ASSERT_TRUE(!buy_side.empty(), "Buy side should not be empty");
    ASSERT_TRUE(buy_side.count(150.0) == 1, "Buy side should have price level 150.0");
    ASSERT_TRUE(buy_side.at(150.0).size() == 1, "Buy side should have one order at 150.0");
    
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
    
    // Check sell side
    auto& sell_side = book.getSellSide();
    ASSERT_TRUE(!sell_side.empty(), "Sell side should not be empty");
    ASSERT_TRUE(sell_side.count(151.0) == 1, "Sell side should have price level 151.0");
    ASSERT_TRUE(sell_side.at(151.0).size() == 1, "Sell side should have one order at 151.0");
    
    // Test cancel order
    bool cancelled = book.cancelOrder(1);
    ASSERT_TRUE(cancelled, "Order 1 should be cancelled successfully");
    ASSERT_TRUE(buy_side.empty(), "Buy side should be empty after cancellation");
    
    // Test modify order
    Order modified_sell = sell_order;
    modified_sell.quantity = 75;
    modified_sell.price = 152.0;
    bool modified = book.modifyOrder(modified_sell);
    ASSERT_TRUE(modified, "Order 2 should be modified successfully");
    ASSERT_TRUE(sell_side.count(151.0) == 0, "Sell side should not have price level 151.0 anymore");
    ASSERT_TRUE(sell_side.count(152.0) == 1, "Sell side should have price level 152.0");
    ASSERT_TRUE(sell_side.at(152.0).front().quantity == 75, "Modified order should have quantity 75");
    
    std::cout << "All order_book_basic tests passed!" << std::endl;
}

// Test price level ordering and multiple orders at same price
TEST(order_book_advanced) {
    OrderBook book("TSLA");
    
    // Add multiple buy orders at different prices
    Order buy_order1 = {
        .timestamp = 123456789,
        .order_id = 1,
        .instrument = "TSLA",
        .side = Side::BUY,
        .type = Type::LIMIT,
        .quantity = 100,
        .price = 250.0,
        .action = Action::NEW
    };
    book.addOrder(buy_order1);
    
    Order buy_order2 = {
        .timestamp = 123456790,
        .order_id = 2,
        .instrument = "TSLA",
        .side = Side::BUY,
        .type = Type::LIMIT,
        .quantity = 150,
        .price = 249.0,
        .action = Action::NEW
    };
    book.addOrder(buy_order2);
    
    Order buy_order3 = {
        .timestamp = 123456791,
        .order_id = 3,
        .instrument = "TSLA",
        .side = Side::BUY,
        .type = Type::LIMIT,
        .quantity = 200,
        .price = 251.0,
        .action = Action::NEW
    };
    book.addOrder(buy_order3);
    
    // Add multiple sell orders at different prices
    Order sell_order1 = {
        .timestamp = 123456792,
        .order_id = 4,
        .instrument = "TSLA",
        .side = Side::SELL,
        .type = Type::LIMIT,
        .quantity = 120,
        .price = 252.0,
        .action = Action::NEW
    };
    book.addOrder(sell_order1);
    
    Order sell_order2 = {
        .timestamp = 123456793,
        .order_id = 5,
        .instrument = "TSLA",
        .side = Side::SELL,
        .type = Type::LIMIT,
        .quantity = 100,
        .price = 253.0,
        .action = Action::NEW
    };
    book.addOrder(sell_order2);
    
    // Add another order at the same price level
    Order buy_order4 = {
        .timestamp = 123456794,
        .order_id = 6,
        .instrument = "TSLA",
        .side = Side::BUY,
        .type = Type::LIMIT,
        .quantity = 50,
        .price = 251.0,  // Same price as buy_order3
        .action = Action::NEW
    };
    book.addOrder(buy_order4);
    
    // Check buy side ordering (highest price first)
    auto& buy_side = book.getBuySide();
    auto buy_it = buy_side.begin();
    
    ASSERT_TRUE(buy_it->first == 251.0, "First buy price should be 251.0");
    ASSERT_TRUE(buy_it->second.size() == 2, "Should have 2 orders at 251.0");
    
    ++buy_it;
    ASSERT_TRUE(buy_it->first == 250.0, "Second buy price should be 250.0");
    ASSERT_TRUE(buy_it->second.size() == 1, "Should have 1 order at 250.0");
    
    ++buy_it;
    ASSERT_TRUE(buy_it->first == 249.0, "Third buy price should be 249.0");
    ASSERT_TRUE(buy_it->second.size() == 1, "Should have 1 order at 249.0");
    
    // Check sell side ordering (lowest price first)
    auto& sell_side = book.getSellSide();
    auto sell_it = sell_side.begin();
    
    ASSERT_TRUE(sell_it->first == 252.0, "First sell price should be 252.0");
    ASSERT_TRUE(sell_it->second.size() == 1, "Should have 1 order at 252.0");
    
    ++sell_it;
    ASSERT_TRUE(sell_it->first == 253.0, "Second sell price should be 253.0");
    ASSERT_TRUE(sell_it->second.size() == 1, "Should have 1 order at 253.0");
    
    // Cancel an order in a level with multiple orders
    bool cancelled = book.cancelOrder(6);
    ASSERT_TRUE(cancelled, "Order 6 should be cancelled successfully");
    ASSERT_TRUE(buy_side.at(251.0).size() == 1, "Should have 1 order at 251.0 after cancellation");
    ASSERT_TRUE(buy_side.at(251.0).front().order_id == 3, "Order at 251.0 should have ID 3");
    
    std::cout << "All order_book_advanced tests passed!" << std::endl;
}

// Main function that runs all tests
int main() {
    std::cout << "Running OrderBook tests..." << std::endl;
    test_order_book_basic();
    test_order_book_advanced();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
