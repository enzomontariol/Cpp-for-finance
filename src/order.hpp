/**
 * @file order.hpp
 * @brief Defines the Order structure and related enums
 * 
 * This file contains:
 * - The Order structure that represents a trading order
 * - Enums for Side (BUY/SELL), Type (MARKET/LIMIT), Action (NEW/MODIFY/CANCEL)
 * - OrderStatus enum for tracking execution status
 * - OrderResult structure for returning results of order processing
 * - Helper functions for enum conversions
 */
#pragma once
#include <cstdint>
#include <string>

/**
 * @enum Side
 * @brief Represents the side of an order (BUY or SELL)
 */
enum class Side { BUY, SELL };

/**
 * @enum Type
 * @brief Represents the type of an order (MARKET or LIMIT)
 */
enum class Type { MARKET, LIMIT };

/**
 * @enum Action
 * @brief Represents the action to take on an order
 */
enum class Action { NEW, MODIFY, CANCEL };

/**
 * @struct Order
 * @brief Represents a trading order
 */
struct Order {
    uint64_t timestamp;      // Timestamp in nanoseconds
    int order_id;            // Unique order identifier
    std::string instrument;  // Trading instrument (e.g., "AAPL")
    Side side;               // BUY or SELL
    Type type;               // MARKET or LIMIT
    int quantity;            // Number of units
    float price;             // Price per unit (ignored for MARKET orders)
    Action action;           // NEW, MODIFY, or CANCEL
};

/**
 * @brief Convert Side enum to string
 */
inline std::string sideToString(Side side) {
    return side == Side::BUY ? "BUY" : "SELL";
}

/**
 * @brief Convert Type enum to string
 */
inline std::string typeToString(Type type) {
    return type == Type::LIMIT ? "LIMIT" : "MARKET";
}

/**
 * @brief Convert Action enum to string
 */
inline std::string actionToString(Action action) {
    switch (action) {
        case Action::NEW: return "NEW";
        case Action::MODIFY: return "MODIFY";
        case Action::CANCEL: return "CANCEL";
        default: return "UNKNOWN";
    }
}

/**
 * @enum OrderStatus
 * @brief Represents the status of an order after processing
 */
enum class OrderStatus {
    PENDING,              // Order is in the book
    PARTIALLY_EXECUTED,   // Order is partially executed
    EXECUTED,             // Order is fully executed
    CANCELED,             // Order is canceled
    REJECTED              // Order is rejected
};

/**
 * @brief Convert OrderStatus enum to string
 */
inline std::string statusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::PENDING: return "PENDING";
        case OrderStatus::PARTIALLY_EXECUTED: return "PARTIALLY_EXECUTED";
        case OrderStatus::EXECUTED: return "EXECUTED";
        case OrderStatus::CANCELED: return "CANCELED";
        case OrderStatus::REJECTED: return "REJECTED";
        default: return "UNKNOWN";
    }
}

/**
 * @struct OrderResult
 * @brief Represents the result of order processing
 */
struct OrderResult {
    uint64_t timestamp;      // Timestamp in nanoseconds
    int order_id;            // Unique order identifier
    std::string instrument;  // Trading instrument (e.g., "AAPL")
    Side side;               // BUY or SELL
    Type type;               // MARKET or LIMIT
    int quantity;            // Original order quantity
    float price;             // Original order price
    Action action;           // NEW, MODIFY, or CANCEL
    OrderStatus status;      // Status after processing
    int executed_quantity;   // Quantity executed (if any)
    float execution_price;   // Execution price (if executed)
    int counterparty_id;     // ID of the counterparty order (if executed)
};