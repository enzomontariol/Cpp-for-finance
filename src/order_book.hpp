/**
 * @file order_book.hpp
 * @brief Defines the OrderBook class that maintains the order book for a specific instrument
 * 
 * The OrderBook maintains:
 * - Buy orders sorted from highest to lowest price
 * - Sell orders sorted from lowest to highest price
 * - Quick lookup mechanism for order modifications and cancellations
 */
#pragma once
#include <map>
#include <list>
#include <unordered_map>
#include <string>
#include "order.hpp"

/**
 * @class OrderBook
 * @brief Maintains the order book for a specific instrument
 */
class OrderBook {
public:
    /**
     * @brief Default constructor required for std::unordered_map
     */
    OrderBook() : instrument("") {}
    
    /**
     * @brief Constructor with instrument name
     * 
     * @param instrument The instrument identifier
     */
    OrderBook(const std::string& instrument);

    /**
     * @brief Add a new order to the book
     * 
     * @param order The order to add
     */
    void addOrder(const Order& order);
    
    /**
     * @brief Cancel an existing order
     * 
     * @param order_id The ID of the order to cancel
     * @return bool True if successful, false if order not found
     */
    bool cancelOrder(int order_id);
    
    /**
     * @brief Modify an existing order (full replacement)
     * 
     * @param order The new order details with same ID
     * @return bool True if successful, false if order not found
     */
    bool modifyOrder(const Order& order);
    
    /**
     * @brief Get the instrument name
     * 
     * @return const std::string& The instrument identifier
     */
    const std::string& getInstrument() const;

    /**
     * @brief Get the buy side of the book
     * 
     * @return const std::map<double, std::list<Order>, std::greater<double>>& 
     *         Buy orders sorted from highest to lowest price
     */
    const std::map<double, std::list<Order>, std::greater<double>>& getBuySide() const;
    
    /**
     * @brief Get the sell side of the book
     * 
     * @return const std::map<double, std::list<Order>>& 
     *         Sell orders sorted from lowest to highest price
     */
    const std::map<double, std::list<Order>>& getSellSide() const;

private:
    std::string instrument;  // Instrument identifier

    // BUY side sorted from high to low price (best prices first)
    std::map<double, std::list<Order>, std::greater<double>> buy_orders;
    
    // SELL side sorted from low to high price (best prices first)
    std::map<double, std::list<Order>> sell_orders;

    // Quick lookup for MODIFY and CANCEL operations
    // Maps order_id to pair of (side, iterator to order in the list)
    std::unordered_map<int, std::pair<Side, std::list<Order>::iterator>> order_lookup;

    /**
     * @brief Helper to get the buy order map for internal use
     */
    std::map<double, std::list<Order>, std::greater<double>>& getBuyOrderMap();
    
    /**
     * @brief Helper to get the sell order map for internal use
     */
    std::map<double, std::list<Order>>& getSellOrderMap();
    
    /**
     * @brief Legacy helper for compatibility
     * @note Has limitations and should be used with caution
     */
    std::map<double, std::list<Order>, std::greater<double>>& getOrderMap(Side side);
};
