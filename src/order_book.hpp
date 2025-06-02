#include <map>
#include <list>
#include <unordered_map>
#include <string>
#include "order.hpp"

class OrderBook {
public:
    OrderBook(const std::string& instrument);

    void addOrder(const Order& order);
    bool cancelOrder(int order_id);
    bool modifyOrder(const Order& order); // full replacement
    const std::string& getInstrument() const;

    const std::map<double, std::list<Order>, std::greater<double>>& getBuySide() const;
    const std::map<double, std::list<Order>>& getSellSide() const;

private:
    std::string instrument;

    // BUY side sorted from high to low price
    std::map<double, std::list<Order>, std::greater<double>> buy_orders;
    // SELL side sorted from low to high price
    std::map<double, std::list<Order>> sell_orders;

    // Quick lookup for MODIFY and CANCEL
    std::unordered_map<int, std::pair<Side, std::list<Order>::iterator>> order_lookup;

    // Helper methods to get the appropriate map
    std::map<double, std::list<Order>, std::greater<double>>& getBuyOrderMap();
    std::map<double, std::list<Order>>& getSellOrderMap();
    
    // This is kept for compatibility but has limitations
    std::map<double, std::list<Order>, std::greater<double>>& getOrderMap(Side side);
};
