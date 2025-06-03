/**
 * @file csv_parser.cpp
 * @brief Implementation of the CSVParser class for reading order data from CSV files.
 */

#include "csv_parser.hpp"
#include <iostream>

/**
 * @brief Constructs a CSV parser for the specified file.
 * @param filename The path to the CSV file to be parsed.
 */
CSVParser::CSVParser(const std::string& filename) : filename_(filename) {}

/**
 * @brief Parses the CSV file and returns a vector of Order objects.
 * 
 * This method reads the specified CSV file line by line, skipping the header row,
 * and converts each subsequent row into an Order object. It handles conversions
 * from string representations to the appropriate enum values for Side, Type, and Action.
 * 
 * @return A vector containing all the orders read from the CSV file.
 */
std::vector<Order> CSVParser::parse() {
    std::vector<Order> orders;
    std::ifstream file(filename_);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier CSV " << filename_ << std::endl;
        return orders;
    }

    // Skip the header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Order order;

        // Parse each column from the CSV row into the Order object
        std::getline(ss, token, ','); order.timestamp = std::stoull(token);
        std::getline(ss, token, ','); order.order_id = std::stoi(token);
        std::getline(ss, token, ','); order.instrument = token;
        
        // Convert string side to enum Side
        std::getline(ss, token, ','); 
        token.erase(0, token.find_first_not_of(" \t"));  // Left trim
        token.erase(token.find_last_not_of(" \t") + 1);  // Right trim
        if (token == "BUY") {
            order.side = Side::BUY;
        } else if (token == "SELL") {
            order.side = Side::SELL;
        }
        
        // Convert string type to enum Type
        std::getline(ss, token, ','); 
        token.erase(0, token.find_first_not_of(" \t"));  // Left trim
        token.erase(token.find_last_not_of(" \t") + 1);  // Right trim
        if (token == "LIMIT") {
            order.type = Type::LIMIT;
        } else if (token == "MARKET") {
            order.type = Type::MARKET;
        }
        
        std::getline(ss, token, ','); order.quantity = std::stoi(token);
        std::getline(ss, token, ','); order.price = std::stof(token);
        
        // Convert string action to enum Action
        std::getline(ss, token, ','); 
        token.erase(0, token.find_first_not_of(" \t"));  // Left trim
        token.erase(token.find_last_not_of(" \t") + 1);  // Right trim
        if (token == "NEW") {
            order.action = Action::NEW;
        } else if (token == "MODIFY") {
            order.action = Action::MODIFY;
        } else if (token == "CANCEL") {
            order.action = Action::CANCEL;
        }

        orders.push_back(order);
    }

    return orders;
}