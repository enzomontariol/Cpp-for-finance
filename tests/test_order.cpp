#include "../src/order.hpp"
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

// Test Order struct initialization and field access
TEST(order_creation) {
    Order order = {
        .timestamp = 1622631600000, // June 2, 2023 00:00:00
        .order_id = 12345,
        .instrument = "AAPL",
        .side = Side::BUY,
        .type = Type::LIMIT,
        .quantity = 100,
        .price = 175.5f,
        .action = Action::NEW
    };
    
    // Test field values
    ASSERT_TRUE(order.timestamp == 1622631600000, "Timestamp value incorrect");
    ASSERT_TRUE(order.order_id == 12345, "Order ID value incorrect");
    ASSERT_TRUE(order.instrument == "AAPL", "Instrument value incorrect");
    ASSERT_TRUE(order.side == Side::BUY, "Side value incorrect");
    ASSERT_TRUE(order.type == Type::LIMIT, "Type value incorrect");
    ASSERT_TRUE(order.quantity == 100, "Quantity value incorrect");
    ASSERT_TRUE(order.price == 175.5f, "Price value incorrect");
    ASSERT_TRUE(order.action == Action::NEW, "Action value incorrect");

    std::cout << "All order_creation tests passed!" << std::endl;
}

// Test Order enums
TEST(order_enums) {
    // Test Side enum values
    Side buy_side = Side::BUY;
    Side sell_side = Side::SELL;
    ASSERT_TRUE(buy_side != sell_side, "BUY and SELL should be different");
    
    // Test Type enum values
    Type market_type = Type::MARKET;
    Type limit_type = Type::LIMIT;
    ASSERT_TRUE(market_type != limit_type, "MARKET and LIMIT should be different");
    
    // Test Action enum values
    Action new_action = Action::NEW;
    Action modify_action = Action::MODIFY;
    Action cancel_action = Action::CANCEL;
    ASSERT_TRUE(new_action != modify_action, "NEW and MODIFY should be different");
    ASSERT_TRUE(new_action != cancel_action, "NEW and CANCEL should be different");
    ASSERT_TRUE(modify_action != cancel_action, "MODIFY and CANCEL should be different");

    std::cout << "All order_enums tests passed!" << std::endl;
}

// Test Order modification
TEST(order_modification) {
    Order original_order = {
        .timestamp = 1622631600000,
        .order_id = 12345,
        .instrument = "AAPL",
        .side = Side::BUY,
        .type = Type::LIMIT,
        .quantity = 100,
        .price = 175.5f,
        .action = Action::NEW
    };
    
    // Modify the order
    Order modified_order = original_order;
    modified_order.timestamp = 1622631700000; // 100 seconds later
    modified_order.quantity = 150;
    modified_order.price = 176.0f;
    modified_order.action = Action::MODIFY;
    
    // Verify fields are modified correctly
    ASSERT_TRUE(modified_order.timestamp == 1622631700000, "Modified timestamp incorrect");
    ASSERT_TRUE(modified_order.quantity == 150, "Modified quantity incorrect");
    ASSERT_TRUE(modified_order.price == 176.0f, "Modified price incorrect");
    ASSERT_TRUE(modified_order.action == Action::MODIFY, "Modified action incorrect");
    
    // Verify unchanged fields remain the same
    ASSERT_TRUE(modified_order.order_id == original_order.order_id, "Order ID should not change");
    ASSERT_TRUE(modified_order.instrument == original_order.instrument, "Instrument should not change");
    ASSERT_TRUE(modified_order.side == original_order.side, "Side should not change");
    ASSERT_TRUE(modified_order.type == original_order.type, "Type should not change");

    std::cout << "All order_modification tests passed!" << std::endl;
}

int main() {
    test_order_creation();
    test_order_enums();
    test_order_modification();
    
    std::cout << "All order tests passed successfully!" << std::endl;
    return 0;
}