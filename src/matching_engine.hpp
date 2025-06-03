/**
 * @file matching_engine.hpp
 * @brief Main matching engine implementation that processes orders and matches them
 * 
 * The matching engine is responsible for:
 * - Managing order books for different instruments
 * - Processing new, modify, and cancel orders
 * - Matching orders according to price-time priority
 * - Handling limit and market orders
 */
#pragma once
#include "order.hpp"
#include "order_book.hpp"
#include "csv_writer.hpp"
#include <unordered_map>
#include <vector>
#include <string>

class MatchingEngine {
public:
    /**
     * @brief Default constructor
     */
    MatchingEngine();
    
    /**
     * @brief Process an order and return the results
     * 
     * @param order The order to process
     * @return std::vector<OrderResult> The results of processing the order
     */
    std::vector<OrderResult> processOrder(const Order& order);
    
    /**
     * @brief Get the order book for a specific instrument
     * 
     * @param instrument The instrument identifier
     * @return OrderBook* Pointer to the order book, nullptr if not found
     */
    OrderBook* getOrderBook(const std::string& instrument);
    
private:
    // Maps instrument to order book
    std::unordered_map<std::string, OrderBook> orderBooks;
    
    /**
     * @brief Handle a new order
     * 
     * @param order The new order to process
     * @return std::vector<OrderResult> The results of processing the order
     */
    std::vector<OrderResult> handleNewOrder(const Order& order);
    
    /**
     * @brief Handle a cancel order request
     * 
     * @param order The cancel order request
     * @return std::vector<OrderResult> The results of processing the request
     */
    std::vector<OrderResult> handleCancelOrder(const Order& order);
    
    /**
     * @brief Handle a modify order request
     * 
     * @param order The modify order request
     * @return std::vector<OrderResult> The results of processing the request
     */
    std::vector<OrderResult> handleModifyOrder(const Order& order);
    
    /**
     * @brief Match orders for a specific instrument
     * 
     * @param order The order to match
     * @param book The order book to match against
     * @return std::vector<OrderResult> The results of matching
     */
    std::vector<OrderResult> matchOrders(const Order& order, OrderBook& book);
    
    /**
     * @brief Match a limit order
     * 
     * @param order The limit order to match
     * @param book The order book to match against
     * @return std::vector<OrderResult> The results of matching
     */
    std::vector<OrderResult> matchLimitOrder(const Order& order, OrderBook& book);
    
    /**
     * @brief Match a market order
     * 
     * @param order The market order to match
     * @param book The order book to match against
     * @return std::vector<OrderResult> The results of matching
     */
    std::vector<OrderResult> matchMarketOrder(const Order& order, OrderBook& book);
    
    /**
     * @brief Create an order result with default values
     * 
     * @param order The order to create a result for
     * @param status The initial status (default: PENDING)
     * @return OrderResult The initialized order result
     */
    OrderResult createOrderResult(const Order& order, OrderStatus status = OrderStatus::PENDING);
};
