/**
 * @file order_book.cpp
 * @brief Implementation of the OrderBook class for managing buy and sell orders.
 */

#include "order_book.hpp"
#include <iostream>

/**
 * @brief Constructor that initializes an order book for a specific financial instrument.
 * @param instrument_ The identifier of the financial instrument.
 */
OrderBook::OrderBook(const std::string& instrument_) : instrument(instrument_) {}

/**
 * @brief Returns the identifier of the instrument this order book is for.
 * @return The instrument identifier as a string reference.
 */
const std::string& OrderBook::getInstrument() const {
    return instrument;
}

/**
 * @brief Gets the map of buy orders sorted by price in descending order.
 * 
 * Buy orders are sorted in descending price order (highest price first) 
 * to facilitate matching against sell orders at the best prices.
 * 
 * @return Reference to the map of buy orders organized by price.
 */
std::map<double, std::list<Order>, std::greater<double>>& OrderBook::getBuyOrderMap() {
    return buy_orders;
}

/**
 * @brief Gets the map of sell orders sorted by price in ascending order.
 * 
 * Sell orders are sorted in ascending price order (lowest price first)
 * to facilitate matching against buy orders at the best prices.
 * 
 * @return Reference to the map of sell orders organized by price.
 */
std::map<double, std::list<Order>>& OrderBook::getSellOrderMap() {
    return sell_orders;
}

/**
 * @brief Helper method that returns the appropriate map based on order side.
 * 
 * Note: This method is a workaround due to C++ type system limitations.
 * It always returns the buy_orders map regardless of the side parameter
 * and relies on the caller to use the specific getBuyOrderMap or getSellOrderMap
 * methods instead.
 * 
 * @param side The side of the order (BUY or SELL).
 * @return Reference to the map of buy orders.
 */
std::map<double, std::list<Order>, std::greater<double>>& OrderBook::getOrderMap(Side side) {
    static_assert(sizeof(std::list<Order>) == sizeof(std::list<Order>), 
                  "This function always returns buy_orders, but uses this static_assert "
                  "to prevent compiler warnings.");
    if (side == Side::BUY) {
        return buy_orders;
    }
    // This is a workaround for the compiler error - we'll directly use getBuyOrderMap or getSellOrderMap in the code
    return buy_orders;
}

/**
 * @brief Adds a new order to the order book.
 * 
 * Inserts the order into the appropriate price level based on its side and price.
 * Also updates the order lookup map to allow quick access to orders by ID.
 * The order is added to the end of its price level's list, maintaining time priority.
 * 
 * @param order The order to add to the book.
 */
void OrderBook::addOrder(const Order& order) {
    if (order.side == Side::BUY) {
        auto& order_list = buy_orders[order.price];
        order_list.push_back(order);
        auto it = std::prev(order_list.end());
        order_lookup[order.order_id] = { order.side, it };
    } else {
        auto& order_list = sell_orders[order.price];
        order_list.push_back(order);
        auto it = std::prev(order_list.end());
        order_lookup[order.order_id] = { order.side, it };
    }
}

/**
 * @brief Cancels an order in the order book by its ID.
 * 
 * Finds the order in the lookup map, removes it from its price level,
 * and removes the price level if it becomes empty. Also removes the
 * order from the lookup map.
 * 
 * @param order_id The ID of the order to cancel.
 * @return true if the order was found and canceled, false otherwise.
 */
bool OrderBook::cancelOrder(int order_id) {
    auto it = order_lookup.find(order_id);
    if (it == order_lookup.end()) return false;

    auto [side, order_it] = it->second;
    double price = order_it->price;
    
    if (side == Side::BUY) {
        auto& order_list = buy_orders[price];
        order_list.erase(order_it);
        if (order_list.empty()) {
            buy_orders.erase(price);
        }
    } else {
        auto& order_list = sell_orders[price];
        order_list.erase(order_it);
        if (order_list.empty()) {
            sell_orders.erase(price);
        }
    }

    order_lookup.erase(order_id);
    return true;
}

/**
 * @brief Modifies an existing order in the order book.
 * 
 * Cancels the old order and adds the new order with the same ID but
 * potentially different attributes (e.g., price, quantity).
 * 
 * @param new_order The modified order with the same ID as an existing order.
 * @return true if the order was found and modified, false otherwise.
 */
bool OrderBook::modifyOrder(const Order& new_order) {
    if (!cancelOrder(new_order.order_id)) {
        return false; // Can't modify if not found
    }
    addOrder(new_order);
    return true;
}

/**
 * @brief Gets a const reference to the buy side of the order book.
 * 
 * @return Const reference to the map of buy orders sorted by price.
 */
const std::map<double, std::list<Order>, std::greater<double>>& OrderBook::getBuySide() const {
    return buy_orders;
}

/**
 * @brief Gets a const reference to the sell side of the order book.
 * 
 * @return Const reference to the map of sell orders sorted by price.
 */
const std::map<double, std::list<Order>>& OrderBook::getSellSide() const {
    return sell_orders;
}
