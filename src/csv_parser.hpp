/**
 * @file csv_parser.hpp
 * @brief Defines the CSVParser class responsible for parsing order data from CSV files.
 * 
 * This class provides functionality to read and parse CSV files containing order data for 
 * the matching engine. Each row in the CSV file represents an order with its attributes.
 */

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "order.hpp"

/**
 * @class CSVParser
 * @brief A class for parsing order data from CSV files.
 * 
 * The CSVParser reads CSV files containing order data and converts each row
 * into an Order object that can be processed by the matching engine.
 * The expected CSV format includes columns for all order attributes like
 * timestamp, order_id, instrument, side, type, quantity, price, and action.
 */
class CSVParser {
public:
    /**
     * @brief Constructs a CSV parser for the specified file.
     * @param filename The path to the CSV file to be parsed.
     */
    explicit CSVParser(const std::string& filename);
    
    /**
     * @brief Parses the CSV file and returns a vector of Order objects.
     * @return A vector containing all the orders read from the CSV file.
     */
    std::vector<Order> parse();

private:
    std::string filename_; ///< The path to the CSV file to be parsed.
};