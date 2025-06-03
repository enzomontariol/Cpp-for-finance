/**
 * @file csv_writer.hpp
 * @brief Defines the CSVWriter class for writing order results to CSV files.
 * 
 * This file provides functionality to write the results of order processing by the matching
 * engine to CSV files. It uses the OrderResult structure and enum conversions defined in order.hpp.
 */

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "order.hpp"

/**
 * @class CSVWriter
 * @brief A class for writing order results to CSV files.
 * 
 * The CSVWriter writes order execution results to a CSV file in a format that
 * includes all the original order information plus execution details.
 * It uses the OrderResult structure and related enums defined in order.hpp.
 */
class CSVWriter {
public:
    /**
     * @brief Constructs a CSV writer for the specified file.
     * @param filename The path to the CSV file to be written.
     */
    explicit CSVWriter(const std::string& filename);
    
    /**
     * @brief Destructor that closes the file if it's open.
     */
    ~CSVWriter();
    
    /**
     * @brief Writes the CSV header row with column names.
     */
    void writeHeader();
    
    /**
     * @brief Writes an order result to the CSV file.
     * @param result The OrderResult to write.
     */
    void writeOrderResult(const OrderResult& result);
    
private:
    std::string filename_;   ///< The path to the CSV file
    std::ofstream file_;     ///< The output file stream
};
