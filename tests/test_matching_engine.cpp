#include "../src/matching_engine.hpp"
#include <iostream>
#include <cassert>
#include <vector>

// Simple test harness function
#define TEST(name) void test_##name()

// Assertion with message
#define ASSERT_TRUE(condition, message) \
    if (!(condition)) { \
        std::cerr << "Assertion failed: " << message << std::endl; \
        exit(1); \
    }

// Test basic order matching functionality
TEST(matching_engine_basic) {
    MatchingEngine engine;
    
    // Create a buy order
    Order buy_order = {
        .timestamp = 1617278400000000000,
        .order_id = 1,
        .instrument = "AAPL",
        .side = Side::BUY,
        .type = Type::LIMIT,
        .quantity = 100,
        .price = 150.25,
        .action = Action::NEW
    };
    
    // Process the buy order
    std::vector<OrderResult> results = engine.processOrder(buy_order);
    
    // Verify we got one result back
    ASSERT_TRUE(results.size() == 1, "Should get one result");
    
    // Verify the result
    ASSERT_TRUE(results[0].order_id == 1, "Result should be for order 1");
    ASSERT_TRUE(results[0].status == OrderStatus::PENDING, "Order should be pending");
    ASSERT_TRUE(results[0].executed_quantity == 0, "No execution yet");
    
    // Create a matching sell order
    Order sell_order = {
        .timestamp = 1617278400000000100,
        .order_id = 2,
        .instrument = "AAPL",
        .side = Side::SELL,
        .type = Type::LIMIT,
        .quantity = 50,
        .price = 150.25,
        .action = Action::NEW
    };
    
    // Process the sell order
    results = engine.processOrder(sell_order);
    
    // Verify we got two results back (sell order result and updated buy order result)
    ASSERT_TRUE(results.size() == 2, "Should get two results");
    
    // Find the buy and sell order results
    OrderResult* buy_result = nullptr;
    OrderResult* sell_result = nullptr;
    for (auto& result : results) {
        if (result.order_id == 1) buy_result = &result;
        if (result.order_id == 2) sell_result = &result;
    }
    
    ASSERT_TRUE(buy_result != nullptr, "Should have a result for buy order");
    ASSERT_TRUE(sell_result != nullptr, "Should have a result for sell order");
    
    // Verify the sell order was executed
    ASSERT_TRUE(sell_result->status == OrderStatus::EXECUTED, "Sell order should be executed");
    ASSERT_TRUE(sell_result->executed_quantity == 50, "Sell order should be fully executed");
    ASSERT_TRUE(sell_result->execution_price == 150.25, "Execution price should be 150.25");
    ASSERT_TRUE(sell_result->counterparty_id == 1, "Counterparty should be order 1");
    
    // Verify the buy order was partially executed
    ASSERT_TRUE(buy_result->status == OrderStatus::PARTIALLY_EXECUTED, "Buy order should be partially executed");
    ASSERT_TRUE(buy_result->executed_quantity == 50, "Buy order should be executed for 50 units");
    ASSERT_TRUE(buy_result->execution_price == 150.25, "Execution price should be 150.25");
    ASSERT_TRUE(buy_result->counterparty_id == 2, "Counterparty should be order 2");
    
    // Create another matching sell order to complete the buy order
    Order sell_order2 = {
        .timestamp = 1617278400000000200,
        .order_id = 3,
        .instrument = "AAPL",
        .side = Side::SELL,
        .type = Type::LIMIT,
        .quantity = 50,
        .price = 150.25,
        .action = Action::NEW
    };
    
    // Process the second sell order
    results = engine.processOrder(sell_order2);
    
    // Verify we got two results back again
    ASSERT_TRUE(results.size() == 2, "Should get two results");
    
    // Find the buy and sell order results
    buy_result = nullptr;
    sell_result = nullptr;
    for (auto& result : results) {
        if (result.order_id == 1) buy_result = &result;
        if (result.order_id == 3) sell_result = &result;
    }
    
    ASSERT_TRUE(buy_result != nullptr, "Should have a result for buy order");
    ASSERT_TRUE(sell_result != nullptr, "Should have a result for sell order");
    
    // Verify the second sell order was executed
    ASSERT_TRUE(sell_result->status == OrderStatus::EXECUTED, "Second sell order should be executed");
    ASSERT_TRUE(sell_result->executed_quantity == 50, "Second sell order should be fully executed");
    
    // Verify the buy order is now fully executed
    ASSERT_TRUE(buy_result->status == OrderStatus::PARTIALLY_EXECUTED, "Buy order should be partially executed");
    ASSERT_TRUE(buy_result->executed_quantity == 50, "Buy order should be executed for 50 units in total");
    
    std::cout << "All matching_engine_basic tests passed!" << std::endl;
}

// Test price-time priority order matching
TEST(matching_engine_priority) {
    MatchingEngine engine;
    
    // Add several sell orders at different prices
    Order sell_order1 = {
        .timestamp = 1617278400000000000,
        .order_id = 1,
        .instrument = "AAPL",
        .side = Side::SELL,
        .type = Type::LIMIT,
        .quantity = 50,
        .price = 150.30,
        .action = Action::NEW
    };
    
    Order sell_order2 = {
        .timestamp = 1617278400000000100,
        .order_id = 2,
        .instrument = "AAPL",
        .side = Side::SELL,
        .type = Type::LIMIT,
        .quantity = 50,
        .price = 150.25, // Better price
        .action = Action::NEW
    };
    
    Order sell_order3 = {
        .timestamp = 1617278400000000200,
        .order_id = 3,
        .instrument = "AAPL",
        .side = Side::SELL,
        .type = Type::LIMIT,
        .quantity = 50,
        .price = 150.25, // Same price as order 2, but later timestamp
        .action = Action::NEW
    };
    
    // Process the sell orders
    engine.processOrder(sell_order1);
    engine.processOrder(sell_order2);
    engine.processOrder(sell_order3);
    
    // Create a buy order that will match
    Order buy_order = {
        .timestamp = 1617278400000000300,
        .order_id = 4,
        .instrument = "AAPL",
        .side = Side::BUY,
        .type = Type::LIMIT,
        .quantity = 100,
        .price = 150.30, // Can match with any of the sell orders
        .action = Action::NEW
    };
    
    // Process the buy order
    std::vector<OrderResult> results = engine.processOrder(buy_order);
    
    // Should have 3 results (buy order + 2 matching sell orders)
    ASSERT_TRUE(results.size() == 3, "Should have 3 results");
    
    // Find the order results
    OrderResult* buy_result = nullptr;
    OrderResult* sell_result2 = nullptr;
    OrderResult* sell_result3 = nullptr;
    
    for (auto& result : results) {
        if (result.order_id == 4) buy_result = &result;
        if (result.order_id == 2) sell_result2 = &result;
        if (result.order_id == 3) sell_result3 = &result;
    }
    
    ASSERT_TRUE(buy_result != nullptr, "Should have a result for buy order");
    ASSERT_TRUE(sell_result2 != nullptr, "Should have a result for sell order 2");
    ASSERT_TRUE(sell_result3 != nullptr, "Should have a result for sell order 3");
    
    // Verify price-time priority: order 2 should be matched first (better price)
    ASSERT_TRUE(sell_result2->status == OrderStatus::EXECUTED, "Sell order 2 should be executed");
    
    // Then order 3 (same price but later timestamp)
    ASSERT_TRUE(sell_result3->status == OrderStatus::EXECUTED, "Sell order 3 should be executed");
    
    // Buy order should be executed for 100 units
    ASSERT_TRUE(buy_result->status == OrderStatus::EXECUTED, "Buy order should be executed");
    ASSERT_TRUE(buy_result->executed_quantity == 100, "Buy order should be executed for 100 units");
    
    std::cout << "All matching_engine_priority tests passed!" << std::endl;
}

// Test market orders
TEST(matching_engine_market_orders) {
    MatchingEngine engine;
    
    // Add a limit sell order
    Order sell_order = {
        .timestamp = 1617278400000000000,
        .order_id = 1,
        .instrument = "AAPL",
        .side = Side::SELL,
        .type = Type::LIMIT,
        .quantity = 100,
        .price = 150.25,
        .action = Action::NEW
    };
    
    engine.processOrder(sell_order);
    
    // Create a market buy order
    Order buy_order = {
        .timestamp = 1617278400000000100,
        .order_id = 2,
        .instrument = "AAPL",
        .side = Side::BUY,
        .type = Type::MARKET,
        .quantity = 50,
        .price = 0.0, // Price is ignored for market orders
        .action = Action::NEW
    };
    
    // Process the market buy order
    std::vector<OrderResult> results = engine.processOrder(buy_order);
    
    // Should have 2 results (buy order + partial sell order)
    ASSERT_TRUE(results.size() == 2, "Should have 2 results");
    
    // Find the order results
    OrderResult* buy_result = nullptr;
    OrderResult* sell_result = nullptr;
    
    for (auto& result : results) {
        if (result.order_id == 2) buy_result = &result;
        if (result.order_id == 1) sell_result = &result;
    }
    
    ASSERT_TRUE(buy_result != nullptr, "Should have a result for buy order");
    ASSERT_TRUE(sell_result != nullptr, "Should have a result for sell order");
    
    // Market buy order should be executed at the limit sell price
    ASSERT_TRUE(buy_result->status == OrderStatus::EXECUTED, "Market buy order should be executed");
    ASSERT_TRUE(buy_result->executed_quantity == 50, "Market buy order should be executed for 50 units");
    ASSERT_TRUE(buy_result->execution_price == 150.25, "Execution price should be the limit price");
    
    // Sell order should be partially executed
    ASSERT_TRUE(sell_result->status == OrderStatus::PARTIALLY_EXECUTED, "Sell order should be partially executed");
    ASSERT_TRUE(sell_result->executed_quantity == 50, "Sell order should be executed for 50 units");
    
    std::cout << "All matching_engine_market_orders tests passed!" << std::endl;
}

// Test cancel and modify orders
TEST(matching_engine_cancel_modify) {
    MatchingEngine engine;
    
    // Add a buy order
    Order buy_order = {
        .timestamp = 1617278400000000000,
        .order_id = 1,
        .instrument = "AAPL",
        .side = Side::BUY,
        .type = Type::LIMIT,
        .quantity = 100,
        .price = 150.25,
        .action = Action::NEW
    };
    
    engine.processOrder(buy_order);
    
    // Cancel the order
    Order cancel_order = {
        .timestamp = 1617278400000000100,
        .order_id = 1,
        .instrument = "AAPL",
        .side = Side::BUY,
        .type = Type::LIMIT,
        .quantity = 100,
        .price = 0.0, // Price is ignored for cancel
        .action = Action::CANCEL
    };
    
    std::vector<OrderResult> results = engine.processOrder(cancel_order);
    
    // Should have 1 result
    ASSERT_TRUE(results.size() == 1, "Should have 1 result");
    ASSERT_TRUE(results[0].status == OrderStatus::CANCELED, "Order should be canceled");
    
    // Add another buy order
    buy_order.order_id = 2;
    engine.processOrder(buy_order);
    
    // Modify the order (increase price)
    Order modify_order = {
        .timestamp = 1617278400000000200,
        .order_id = 2,
        .instrument = "AAPL",
        .side = Side::BUY,
        .type = Type::LIMIT,
        .quantity = 100,
        .price = 151.00, // Higher price
        .action = Action::MODIFY
    };
    
    results = engine.processOrder(modify_order);
    
    // Should have 1 result
    ASSERT_TRUE(results.size() == 1, "Should have 1 result");
    ASSERT_TRUE(results[0].price == 151.00, "Price should be modified");
    
    std::cout << "All matching_engine_cancel_modify tests passed!" << std::endl;
}

int main() {
    std::cout << "Running MatchingEngine tests..." << std::endl;
    test_matching_engine_basic();
    test_matching_engine_priority();
    test_matching_engine_market_orders();
    test_matching_engine_cancel_modify();
    std::cout << "All MatchingEngine tests passed!" << std::endl;
    return 0;
}
