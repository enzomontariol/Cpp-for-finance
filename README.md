# Cpp for Finance

## Overview
This project implements a financial trading system in C++, focusing on high-performance order book functionality for market simulation and analysis. It provides a robust framework for handling limit orders, managing order books, and processing trading data.

## Components

### Implemented Components
- **Order Structure**: Definition of orders and related enumerations ([Documentation](docs/src/order.md))
- **Order Book**: Implementation of a price-time priority limit order book ([Documentation](docs/src/order_book.md))

### Planned Components
- **CSV Parser**: Tool for importing order data from CSV files ([Documentation](docs/src/csv_parser.md))
- **Main Application**: Entry point that ties all components together ([Documentation](docs/src/main.md))

## Getting Started

### Building the Project
```bash
make
```

### Running Tests
```bash
./build/test_order
./build/test_order_book
```

## Project Structure
- **src/**: Source code for the main components
- **tests/**: Unit tests for the components
- **docs/**: Documentation for each component
- **data/**: Sample data files for testing
- **build/**: Compiled object files and executables
- **benchmarks/**: Performance benchmarking tools

## Future Development
- Implementation of the CSV parser for data import
- Development of the main application entry point
- Addition of matching engine functionality
- Performance optimizations and benchmarks
 
