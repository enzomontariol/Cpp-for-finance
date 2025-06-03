/**
 * @file csv_writer.cpp
 * @brief Implementation of the CSVWriter class for writing order results to CSV files.
 */

#include "csv_writer.hpp"
#include <iostream>

/**
 * @brief Constructs a CSV writer for the specified file.
 * 
 * Opens the specified file for writing. If the file cannot be opened,
 * an error message is printed to standard error.
 * 
 * @param filename The path to the CSV file to be written.
 */
CSVWriter::CSVWriter(const std::string& filename) : filename_(filename) {
    file_.open(filename);
    if (!file_.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier CSV " << filename_ << std::endl;
    }
}

/**
 * @brief Destructor that closes the file if it's open.
 */
CSVWriter::~CSVWriter() {
    if (file_.is_open()) {
        file_.close();
    }
}

/**
 * @brief Writes the CSV header row with column names.
 * 
 * Writes a comma-separated list of column names as the first row in the CSV file.
 */
void CSVWriter::writeHeader() {
    file_ << "timestamp,order_id,instrument,side,type,quantity,price,action,status,executed_quantity,execution_price,counterparty_id" << std::endl;
}

/**
 * @brief Writes an order result to the CSV file.
 * 
 * Formats and writes all fields of the OrderResult to the CSV file as a single row.
 * Uses the conversion functions from order.hpp to convert enum values to their string representations.
 * 
 * @param result The OrderResult to write.
 */
void CSVWriter::writeOrderResult(const OrderResult& result) {
    file_ << result.timestamp << ","
          << result.order_id << ","
          << result.instrument << ","
          << sideToString(result.side) << ","
          << typeToString(result.type) << ","
          << result.quantity << ","
          << result.price << ","
          << actionToString(result.action) << ","
          << statusToString(result.status) << ","
          << result.executed_quantity << ","
          << result.execution_price << ","
          << result.counterparty_id << std::endl;
}
