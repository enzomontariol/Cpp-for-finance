#include "../src/csv_writer.hpp"
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>

// Simple test harness function
#define TEST(name) void test_##name()

// Assertion with message
#define ASSERT_TRUE(condition, message) \
    if (!(condition)) { \
        std::cerr << "Assertion failed: " << message << std::endl; \
        exit(1); \
    }

// Test basic CSV writing functionality
TEST(csv_writer_basic) {
    // Create a temporary filename
    std::string filename = "test_output.csv";
    
    // Create some sample order results
    std::vector<OrderResult> results;
    
    // First order result
    OrderResult result1;
    result1.timestamp = 1617278400000000000;
    result1.order_id = 1;
    result1.instrument = "AAPL";
    result1.side = Side::BUY;
    result1.type = Type::LIMIT;
    result1.quantity = 100;
    result1.price = 150.25f;
    result1.action = Action::NEW;
    result1.status = OrderStatus::PENDING;
    result1.executed_quantity = 0;
    result1.execution_price = 0.0f;
    result1.counterparty_id = 0;
    
    // Second order result
    OrderResult result2;
    result2.timestamp = 1617278400000000100;
    result2.order_id = 2;
    result2.instrument = "AAPL";
    result2.side = Side::SELL;
    result2.type = Type::LIMIT;
    result2.quantity = 50;
    result2.price = 150.25f;
    result2.action = Action::NEW;
    result2.status = OrderStatus::EXECUTED;
    result2.executed_quantity = 50;
    result2.execution_price = 150.25f;
    result2.counterparty_id = 1;
    
    results.push_back(result1);
    results.push_back(result2);
    
    // Write to the CSV file
    {
        CSVWriter writer(filename);
        writer.writeHeader();
        
        for (const auto& result : results) {
            writer.writeOrderResult(result);
        }
    } // Close the file by destroying the writer
    
    // Read back the file to verify contents
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open(), "Failed to open the output file for verification");
    
    std::string line;
    std::vector<std::string> lines;
    
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    file.close();
    
    // Verify the number of lines (header + 2 results)
    ASSERT_TRUE(lines.size() == 3, "Should have 3 lines in the output file");
    
    // Verify the header
    ASSERT_TRUE(lines[0] == "timestamp,order_id,instrument,side,type,quantity,price,action,status,executed_quantity,execution_price,counterparty_id", 
               "Header line doesn't match expected format");
    
    // Verify first order result (pending)
    ASSERT_TRUE(lines[1].find("PENDING") != std::string::npos, "First line should contain PENDING status");
    ASSERT_TRUE(lines[1].find("BUY") != std::string::npos, "First line should contain BUY side");
    ASSERT_TRUE(lines[1].find("LIMIT") != std::string::npos, "First line should contain LIMIT type");
    ASSERT_TRUE(lines[1].find("NEW") != std::string::npos, "First line should contain NEW action");
    
    // Verify second order result (executed)
    ASSERT_TRUE(lines[2].find("EXECUTED") != std::string::npos, "Second line should contain EXECUTED status");
    ASSERT_TRUE(lines[2].find("SELL") != std::string::npos, "Second line should contain SELL side");
    ASSERT_TRUE(lines[2].find("50") != std::string::npos, "Second line should contain quantity 50");
    ASSERT_TRUE(lines[2].find("1") != std::string::npos, "Second line should contain counterparty_id 1");
    
    // Clean up the temporary file
    std::remove(filename.c_str());
    
    std::cout << "All csv_writer_basic tests passed!" << std::endl;
}

// Test CSV writer error handling
TEST(csv_writer_error) {
    // Try to write to an invalid location
    std::string filename = "/invalid/path/file.csv";
    
    CSVWriter writer(filename);
    // Should not crash even if file can't be opened
    
    OrderResult result;
    result.timestamp = 1617278400000000000;
    result.order_id = 1;
    result.instrument = "AAPL";
    result.side = Side::BUY;
    result.type = Type::LIMIT;
    result.quantity = 100;
    result.price = 150.25f;
    result.action = Action::NEW;
    result.status = OrderStatus::PENDING;
    
    // These operations should not crash even if file is not open
    writer.writeHeader();
    writer.writeOrderResult(result);
    
    std::cout << "All csv_writer_error tests passed!" << std::endl;
}

// Test status/side/type/action conversion functions
TEST(csv_writer_conversions) {
    // Test status to string conversion
    ASSERT_TRUE(statusToString(OrderStatus::EXECUTED) == "EXECUTED", "EXECUTED status conversion incorrect");
    ASSERT_TRUE(statusToString(OrderStatus::PARTIALLY_EXECUTED) == "PARTIALLY_EXECUTED", "PARTIALLY_EXECUTED status conversion incorrect");
    ASSERT_TRUE(statusToString(OrderStatus::PENDING) == "PENDING", "PENDING status conversion incorrect");
    ASSERT_TRUE(statusToString(OrderStatus::CANCELED) == "CANCELED", "CANCELED status conversion incorrect");
    ASSERT_TRUE(statusToString(OrderStatus::REJECTED) == "REJECTED", "REJECTED status conversion incorrect");
    
    // Test side to string conversion
    ASSERT_TRUE(sideToString(Side::BUY) == "BUY", "BUY side conversion incorrect");
    ASSERT_TRUE(sideToString(Side::SELL) == "SELL", "SELL side conversion incorrect");
    
    // Test type to string conversion
    ASSERT_TRUE(typeToString(Type::MARKET) == "MARKET", "MARKET type conversion incorrect");
    ASSERT_TRUE(typeToString(Type::LIMIT) == "LIMIT", "LIMIT type conversion incorrect");
    
    // Test action to string conversion
    ASSERT_TRUE(actionToString(Action::NEW) == "NEW", "NEW action conversion incorrect");
    ASSERT_TRUE(actionToString(Action::MODIFY) == "MODIFY", "MODIFY action conversion incorrect");
    ASSERT_TRUE(actionToString(Action::CANCEL) == "CANCEL", "CANCEL action conversion incorrect");
    
    std::cout << "All csv_writer_conversions tests passed!" << std::endl;
}

int main() {
    test_csv_writer_basic();
    test_csv_writer_error();
    test_csv_writer_conversions();
    
    std::cout << "All CSVWriter tests passed successfully!" << std::endl;
    return 0;
}
