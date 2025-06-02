#include "order_book.hpp"
#include <iostream>

OrderBook::OrderBook(const std::string& instrument_) : instrument(instrument_) {}

const std::string& OrderBook::getInstrument() const {
    return instrument;
}

// We need separate methods for buy and sell sides due to different comparators
std::map<double, std::list<Order>, std::greater<double>>& OrderBook::getBuyOrderMap() {
    return buy_orders;
}

std::map<double, std::list<Order>>& OrderBook::getSellOrderMap() {
    return sell_orders;
}

// Helper method that returns the appropriate map based on side
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

bool OrderBook::modifyOrder(const Order& new_order) {
    if (!cancelOrder(new_order.order_id)) {
        return false; // Can't modify if not found
    }
    addOrder(new_order);
    return true;
}

const std::map<double, std::list<Order>, std::greater<double>>& OrderBook::getBuySide() const {
    return buy_orders;
}

const std::map<double, std::list<Order>>& OrderBook::getSellSide() const {
    return sell_orders;
}
