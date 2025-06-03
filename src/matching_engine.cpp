// filepath: /Users/julienmoury/Library/Mobile Documents/com~apple~CloudDocs/Dauphine/C++/Cpp-for-finance/src/matching_engine.cpp
/**
 * @file matching_engine.cpp
 * @brief Implementation of the matching engine functionality
 */
#include "matching_engine.hpp"
#include <iostream>

// Default constructor
MatchingEngine::MatchingEngine() {}

/**
 * @brief Process an incoming order
 * 
 * This is the main entry point for order processing. It determines the type of order
 * (new, cancel, modify) and routes it to the appropriate handler.
 */
std::vector<OrderResult> MatchingEngine::processOrder(const Order& order) {
    // Check if we need to create a new order book for this instrument
    if (orderBooks.find(order.instrument) == orderBooks.end()) {
        orderBooks.emplace(order.instrument, OrderBook(order.instrument));
    }
    
    // Process order based on action
    switch (order.action) {
        case Action::NEW:
            return handleNewOrder(order);
        case Action::CANCEL:
            return handleCancelOrder(order);
        case Action::MODIFY:
            return handleModifyOrder(order);
        default:
            // Unrecognized action, return rejected
            std::vector<OrderResult> results;
            OrderResult result = createOrderResult(order, OrderStatus::REJECTED);
            results.push_back(result);
            return results;
    }
}

/**
 * @brief Get the order book for a specific instrument
 * 
 * @param instrument The instrument identifier
 * @return OrderBook* Pointer to the order book, nullptr if not found
 */
OrderBook* MatchingEngine::getOrderBook(const std::string& instrument) {
    auto it = orderBooks.find(instrument);
    if (it != orderBooks.end()) {
        return &(it->second);
    }
    return nullptr;
}

/**
 * @brief Process a new order
 * 
 * For new orders:
 * 1. Try to match against existing orders
 * 2. If not fully executed, add the remaining quantity to the book
 */
std::vector<OrderResult> MatchingEngine::handleNewOrder(const Order& order) {
    OrderBook& book = orderBooks[order.instrument];
    
    // First try to match the order
    std::vector<OrderResult> results = matchOrders(order, book);
    
    // If order was not fully executed, add remaining quantity to the book
    OrderResult& orderResult = results[0]; // First result is always the new order
    
    if (orderResult.status != OrderStatus::EXECUTED) {
        // Add to order book (remaining quantity)
        book.addOrder(order);
    }
    
    return results;
}

/**
 * @brief Process a cancel order request
 * 
 * Attempts to cancel an existing order and returns the result
 */
std::vector<OrderResult> MatchingEngine::handleCancelOrder(const Order& order) {
    OrderBook& book = orderBooks[order.instrument];
    std::vector<OrderResult> results;
    
    // Try to cancel the order
    bool canceled = book.cancelOrder(order.order_id);
    
    // Create result
    OrderResult result = createOrderResult(order, 
                                         canceled ? OrderStatus::CANCELED : OrderStatus::REJECTED);
    results.push_back(result);
    
    return results;
}

/**
 * @brief Process a modify order request
 * 
 * Attempts to modify an existing order and returns the result
 */
std::vector<OrderResult> MatchingEngine::handleModifyOrder(const Order& order) {
    OrderBook& book = orderBooks[order.instrument];
    std::vector<OrderResult> results;
    
    // Try to modify the order
    bool modified = book.modifyOrder(order);
    
    // Create result
    OrderResult result = createOrderResult(order, 
                                         modified ? OrderStatus::PENDING : OrderStatus::REJECTED);
    results.push_back(result);
    
    return results;
}

/**
 * @brief Route order to appropriate matching function based on order type
 */
std::vector<OrderResult> MatchingEngine::matchOrders(const Order& order, OrderBook& book) {
    if (order.type == Type::LIMIT) {
        return matchLimitOrder(order, book);
    } else {
        return matchMarketOrder(order, book);
    }
}

/**
 * @brief Match a limit order against the order book
 * 
 * Implements price-time priority matching:
 * - For buy orders: match against sell orders with price <= buy price
 * - For sell orders: match against buy orders with price >= sell price
 * - Orders at the same price level are matched in time priority (FIFO)
 */
std::vector<OrderResult> MatchingEngine::matchLimitOrder(const Order& order, OrderBook& book) {
    std::vector<OrderResult> results;
    
    // Initialize result for the incoming order
    OrderResult orderResult = createOrderResult(order);
    
    int remainingQuantity = order.quantity;
    bool hasMatches = false;
    
    // Get the appropriate side of the book to match against
    if (order.side == Side::BUY) {
        // For buy orders, match against sell orders
        const auto& sellSide = book.getSellSide();
        
        for (auto& [price, orders] : sellSide) {
            // Check if the prices match - only match if sell price <= buy price
            if (price > order.price) {
                break; // No more matches possible due to price ordering
            }
            
            // Match orders at this price level (respecting time priority)
            auto orderIt = orders.begin();
            while (orderIt != orders.end() && remainingQuantity > 0) {
                const Order& matchingOrder = *orderIt;
                int matchQuantity = std::min(remainingQuantity, matchingOrder.quantity);
                
                // Create a result for the matching order
                OrderResult matchResult = createOrderResult(matchingOrder);
                matchResult.executed_quantity = matchQuantity;
                matchResult.execution_price = price;
                matchResult.counterparty_id = order.order_id;
                matchResult.status = (matchQuantity == matchingOrder.quantity) 
                                   ? OrderStatus::EXECUTED 
                                   : OrderStatus::PARTIALLY_EXECUTED;
                
                // Update the incoming order result
                orderResult.executed_quantity += matchQuantity;
                orderResult.execution_price = price; // Last execution price
                orderResult.counterparty_id = matchingOrder.order_id;
                
                // Update remaining quantity
                remainingQuantity -= matchQuantity;
                hasMatches = true;
                
                // Add the match result
                results.push_back(matchResult);
                
                // Move to next order at this price level
                ++orderIt;
            }
            
            if (remainingQuantity == 0) {
                break; // Fully matched
            }
        }
    } else {
        // For sell orders, match against buy orders
        const auto& buySide = book.getBuySide();
        
        for (auto& [price, orders] : buySide) {
            // Check if the prices match - only match if buy price >= sell price
            if (price < order.price) {
                break; // No more matches possible due to price ordering
            }
            
            // Match orders at this price level (respecting time priority)
            auto orderIt = orders.begin();
            while (orderIt != orders.end() && remainingQuantity > 0) {
                const Order& matchingOrder = *orderIt;
                int matchQuantity = std::min(remainingQuantity, matchingOrder.quantity);
                
                // Create a result for the matching order
                OrderResult matchResult = createOrderResult(matchingOrder);
                matchResult.executed_quantity = matchQuantity;
                matchResult.execution_price = price;
                matchResult.counterparty_id = order.order_id;
                matchResult.status = (matchQuantity == matchingOrder.quantity) 
                                   ? OrderStatus::EXECUTED 
                                   : OrderStatus::PARTIALLY_EXECUTED;
                
                // Update the incoming order result
                orderResult.executed_quantity += matchQuantity;
                orderResult.execution_price = price; // Last execution price
                orderResult.counterparty_id = matchingOrder.order_id;
                
                // Update remaining quantity
                remainingQuantity -= matchQuantity;
                hasMatches = true;
                
                // Add the match result
                results.push_back(matchResult);
                
                // Move to next order at this price level
                ++orderIt;
            }
            
            if (remainingQuantity == 0) {
                break; // Fully matched
            }
        }
    }
    
    // Update status of the incoming order
    if (hasMatches) {
        if (remainingQuantity == 0) {
            orderResult.status = OrderStatus::EXECUTED;
        } else {
            orderResult.status = OrderStatus::PARTIALLY_EXECUTED;
        }
    }
    
    // Add the incoming order result first
    results.insert(results.begin(), orderResult);
    
    return results;
}

/**
 * @brief Match a market order against the order book
 * 
 * Market orders execute at any available price:
 * - For buy market orders: match against sell orders in price order (lowest first)
 * - For sell market orders: match against buy orders in price order (highest first)
 * - Orders at the same price level are matched in time priority (FIFO)
 * - Market orders that cannot be executed are rejected
 */
std::vector<OrderResult> MatchingEngine::matchMarketOrder(const Order& order, OrderBook& book) {
    std::vector<OrderResult> results;
    
    // Initialize result for the incoming order
    OrderResult orderResult = createOrderResult(order);
    
    int remainingQuantity = order.quantity;
    bool hasMatches = false;
    
    // Market orders execute at any price, but still respect time priority
    if (order.side == Side::BUY) {
        // For buy market orders, match against sell orders
        const auto& sellSide = book.getSellSide();
        
        for (auto& [price, orders] : sellSide) {
            // Match orders at this price level (respecting time priority)
            auto orderIt = orders.begin();
            while (orderIt != orders.end() && remainingQuantity > 0) {
                const Order& matchingOrder = *orderIt;
                int matchQuantity = std::min(remainingQuantity, matchingOrder.quantity);
                
                // Create a result for the matching order
                OrderResult matchResult = createOrderResult(matchingOrder);
                matchResult.executed_quantity = matchQuantity;
                matchResult.execution_price = price;
                matchResult.counterparty_id = order.order_id;
                matchResult.status = (matchQuantity == matchingOrder.quantity) 
                                   ? OrderStatus::EXECUTED 
                                   : OrderStatus::PARTIALLY_EXECUTED;
                
                // Update the incoming order result
                orderResult.executed_quantity += matchQuantity;
                orderResult.execution_price = price; // Last execution price
                orderResult.counterparty_id = matchingOrder.order_id;
                
                // Update remaining quantity
                remainingQuantity -= matchQuantity;
                hasMatches = true;
                
                // Add the match result
                results.push_back(matchResult);
                
                // Move to next order at this price level
                ++orderIt;
            }
            
            if (remainingQuantity == 0) {
                break; // Fully matched
            }
        }
    } else {
        // For sell market orders, match against buy orders
        const auto& buySide = book.getBuySide();
        
        for (auto& [price, orders] : buySide) {
            // Match orders at this price level (respecting time priority)
            auto orderIt = orders.begin();
            while (orderIt != orders.end() && remainingQuantity > 0) {
                const Order& matchingOrder = *orderIt;
                int matchQuantity = std::min(remainingQuantity, matchingOrder.quantity);
                
                // Create a result for the matching order
                OrderResult matchResult = createOrderResult(matchingOrder);
                matchResult.executed_quantity = matchQuantity;
                matchResult.execution_price = price;
                matchResult.counterparty_id = order.order_id;
                matchResult.status = (matchQuantity == matchingOrder.quantity) 
                                   ? OrderStatus::EXECUTED 
                                   : OrderStatus::PARTIALLY_EXECUTED;
                
                // Update the incoming order result
                orderResult.executed_quantity += matchQuantity;
                orderResult.execution_price = price; // Last execution price
                orderResult.counterparty_id = matchingOrder.order_id;
                
                // Update remaining quantity
                remainingQuantity -= matchQuantity;
                hasMatches = true;
                
                // Add the match result
                results.push_back(matchResult);
                
                // Move to next order at this price level
                ++orderIt;
            }
            
            if (remainingQuantity == 0) {
                break; // Fully matched
            }
        }
    }
    
    // Update status of the incoming order
    if (hasMatches) {
        if (remainingQuantity == 0) {
            orderResult.status = OrderStatus::EXECUTED;
        } else {
            orderResult.status = OrderStatus::PARTIALLY_EXECUTED;
        }
    } else {
        // No matches for market order
        orderResult.status = OrderStatus::REJECTED;
    }
    
    // Add the incoming order result first
    results.insert(results.begin(), orderResult);
    
    return results;
}

/**
 * @brief Create an order result with default values
 * 
 * Initializes an OrderResult object with values from the order and default 
 * execution values.
 * 
 * @param order The original order
 * @param status The initial status (default: PENDING)
 * @return OrderResult The initialized order result
 */
OrderResult MatchingEngine::createOrderResult(const Order& order, OrderStatus status) {
    OrderResult result;
    
    // Copy fields from the order
    result.timestamp = order.timestamp;
    result.order_id = order.order_id;
    result.instrument = order.instrument;
    result.side = order.side;
    result.type = order.type;
    result.quantity = order.quantity;
    result.price = order.price;
    result.action = order.action;
    
    // Initialize additional fields
    result.status = status;
    result.executed_quantity = 0;
    result.execution_price = 0.0f;
    result.counterparty_id = 0;
    
    return result;
}
