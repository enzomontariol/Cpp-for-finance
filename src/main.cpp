/**
 * @file main.cpp
 * @brief Main entry point for the matching engine application.
 * 
 * This file implements the main function that orchestrates the flow of the application:
 * 1. Parsing order data from an input CSV file
 * 2. Processing orders through the matching engine
 * 3. Writing results to an output CSV file
 * 4. Displaying statistics and order book status
 */

#include "csv_parser.hpp"
#include "csv_writer.hpp"
#include "matching_engine.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>

/**
 * @brief Helper function to print order details to the console.
 * 
 * Formats and displays the key attributes of an order for debugging purposes.
 * 
 * @param order The order to display.
 */
void printOrder(const Order& order) {
    std::cout << "Order #" << order.order_id << " - "
              << sideToString(order.side) << " " 
              << order.quantity << " "
              << order.instrument << " @ "
              << std::fixed << std::setprecision(2) << order.price
              << " [" << actionToString(order.action) << "] "
              << (order.type == Type::MARKET ? "MARKET" : "LIMIT")
              << std::endl;
}

/**
 * @brief Helper function to print order result details to the console.
 * 
 * Formats and displays the results of order processing, including execution
 * information if applicable.
 * 
 * @param result The order result to display.
 */
void printOrderResult(const OrderResult& result) {
    std::cout << "Result for Order #" << result.order_id << ": "
              << statusToString(result.status);
    
    if (result.executed_quantity > 0) {
        std::cout << " - Executed " << result.executed_quantity 
                  << " @ " << std::fixed << std::setprecision(2) << result.execution_price
                  << " (Counterparty: " << result.counterparty_id << ")";
    }
    
    std::cout << std::endl;
}

/**
 * @brief Main function - entry point of the application.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return 0 if execution completes successfully, non-zero otherwise.
 */
int main(int argc, char* argv[]) {
    // Check arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    
    // Record start time for performance measurement
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Parse the input file
    CSVParser parser(inputFile);
    std::vector<Order> orders = parser.parse();
    
    std::cout << "Loaded " << orders.size() << " orders from " << inputFile << std::endl;
    
    // Create the writer for the output file
    CSVWriter writer(outputFile);
    writer.writeHeader();
    
    // Create the matching engine
    MatchingEngine engine;
    
    // Process all orders
    for (const auto& order : orders) {
        // Display order for debugging
        std::cout << "\nProcessing ";
        printOrder(order);
        
        // Process the order and get results
        std::vector<OrderResult> results = engine.processOrder(order);
        
        // Write all results to the output file and display them
        for (const auto& result : results) {
            writer.writeOrderResult(result);
            printOrderResult(result);
        }
    }
    
    // Record end time and calculate processing time
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Print statistics
    std::cout << "\nProcessed " << orders.size() << " orders in " 
              << duration.count() << " milliseconds" << std::endl;
    std::cout << "Results written to " << outputFile << std::endl;
    
    // Print order book status for each instrument
    std::cout << "\nFinal Order Book Status:" << std::endl;
    std::vector<std::string> instruments;
    
    // Get all unique instruments from the orders
    for (const auto& order : orders) {
        if (std::find(instruments.begin(), instruments.end(), order.instrument) == instruments.end()) {
            instruments.push_back(order.instrument);
        }
    }
    
    // Print order book for each instrument
    for (const auto& instrument : instruments) {
        OrderBook* book = engine.getOrderBook(instrument);
        if (book) {
            std::cout << "\n== Order Book for " << instrument << " ==" << std::endl;
            
            // Display the buy side of the order book
            std::cout << "BUY side:" << std::endl;
            const auto& buySide = book->getBuySide();
            if (buySide.empty()) {
                std::cout << "  (empty)" << std::endl;
            } else {
                for (auto it = buySide.rbegin(); it != buySide.rend(); ++it) {
                    std::cout << "  Price " << std::fixed << std::setprecision(2) << it->first 
                              << ": " << it->second.size() << " orders" << std::endl;
                }
            }
            
            // Display the sell side of the order book
            std::cout << "SELL side:" << std::endl;
            const auto& sellSide = book->getSellSide();
            if (sellSide.empty()) {
                std::cout << "  (empty)" << std::endl;
            } else {
                for (auto it = sellSide.begin(); it != sellSide.end(); ++it) {
                    std::cout << "  Price " << std::fixed << std::setprecision(2) << it->first 
                              << ": " << it->second.size() << " orders" << std::endl;
                }
            }
        }
    }
    
    return 0;
}