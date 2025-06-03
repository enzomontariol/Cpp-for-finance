#include "../src/csv_parser.hpp"
#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <string>

// Simple test harness function
#define TEST(name) void test_##name()

// Assertion with message
#define ASSERT_TRUE(condition, message) \
    if (!(condition)) { \
        std::cerr << "Assertion failed: " << message << std::endl; \
        exit(1); \
    }

// Create a temporary CSV file for testing
std::string createTempCSVFile() {
    std::string filename = "test_data.csv";
    std::ofstream file(filename);
    
    file << "timestamp,order_id,instrument,side,type,quantity,price,action\n";
    file << "1617278400000000000,1,AAPL,BUY,LIMIT,100,150.25,NEW\n";
    file << "1617278400000000100,2,AAPL,SELL,LIMIT,50,150.25,NEW\n";
    file << "1617278400000000200,3,MSFT,BUY,MARKET,75,0,NEW\n";
    
    file.close();
    return filename;
}

// Test CSV parsing functionality
TEST(csv_parser_basic) {
    std::string filename = createTempCSVFile();
    CSVParser parser(filename);
    
    std::vector<Order> orders = parser.parse();
    
    // Verify that we got the correct number of orders
    ASSERT_TRUE(orders.size() == 3, "Should have parsed 3 orders");
    
    // Verify first order
    ASSERT_TRUE(orders[0].timestamp == 1617278400000000000, "First order timestamp incorrect");
    ASSERT_TRUE(orders[0].order_id == 1, "First order ID incorrect");
    ASSERT_TRUE(orders[0].instrument == "AAPL", "First order instrument incorrect");
    ASSERT_TRUE(orders[0].side == Side::BUY, "First order side incorrect");
    ASSERT_TRUE(orders[0].type == Type::LIMIT, "First order type incorrect");
    ASSERT_TRUE(orders[0].quantity == 100, "First order quantity incorrect");
    ASSERT_TRUE(orders[0].price == 150.25f, "First order price incorrect");
    ASSERT_TRUE(orders[0].action == Action::NEW, "First order action incorrect");
    
    // Verify second order
    ASSERT_TRUE(orders[1].timestamp == 1617278400000000100, "Second order timestamp incorrect");
    ASSERT_TRUE(orders[1].order_id == 2, "Second order ID incorrect");
    ASSERT_TRUE(orders[1].instrument == "AAPL", "Second order instrument incorrect");
    ASSERT_TRUE(orders[1].side == Side::SELL, "Second order side incorrect");
    ASSERT_TRUE(orders[1].type == Type::LIMIT, "Second order type incorrect");
    ASSERT_TRUE(orders[1].quantity == 50, "Second order quantity incorrect");
    ASSERT_TRUE(orders[1].price == 150.25f, "Second order price incorrect");
    ASSERT_TRUE(orders[1].action == Action::NEW, "Second order action incorrect");
    
    // Verify third order (MARKET order)
    ASSERT_TRUE(orders[2].timestamp == 1617278400000000200, "Third order timestamp incorrect");
    ASSERT_TRUE(orders[2].instrument == "MSFT", "Third order instrument incorrect");
    ASSERT_TRUE(orders[2].side == Side::BUY, "Third order side incorrect");
    ASSERT_TRUE(orders[2].type == Type::MARKET, "Third order type incorrect");
    ASSERT_TRUE(orders[2].quantity == 75, "Third order quantity incorrect");
    ASSERT_TRUE(orders[2].price == 0.0f, "Third order price should be 0 for MARKET order");
    
    // Clean up the temporary file
    std::remove(filename.c_str());
    
    std::cout << "All csv_parser_basic tests passed!" << std::endl;
}

// Test error handling for non-existent file
TEST(csv_parser_file_error) {
    CSVParser parser("non_existent_file.csv");
    std::vector<Order> orders = parser.parse();
    
    // Verify that we got an empty vector due to file error
    ASSERT_TRUE(orders.empty(), "Should return empty vector for non-existent file");
    
    std::cout << "All csv_parser_file_error tests passed!" << std::endl;
}

int main() {
    test_csv_parser_basic();
    test_csv_parser_file_error();
    
    std::cout << "All CSVParser tests passed successfully!" << std::endl;
    return 0;
}