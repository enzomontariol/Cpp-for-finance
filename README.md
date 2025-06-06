# Cpp for Finance

## Overview

Cpp for Finance is a complete financial trading system written in C++. It is designed for market simulation and financial data analysis, with a strong focus on performance, efficient order book management, and modularity to facilitate extension and experimentation.

---

## Main Features

- **Limit Order Management**: Create, insert, modify, and cancel orders.
- **High-Performance Order Book**: Price-time priority, efficient transaction management.
- **CSV Data Import**: Load order flows from CSV files for realistic simulations.
- **Unit Tests**: Comprehensive tests to ensure component robustness.
- **Benchmarks**: Tools to measure the performance of the order book.

---

## Project Structure

```
src/         → Main source code (structures, order book, CSV parser, main)
tests/       → Unit tests for each component
docs/        → Detailed module documentation
data/        → Example data files (CSV)
build/       → Compiled objects and executables
benchmarks/  → Performance measurement scripts and tools
```

---

## Components

- **Order Structure**: Structures and enums for orders ([Documentation](docs/src/order.md))
- **Order Book**: Limit order book with price-time priority ([Documentation](docs/src/order_book.md))
- **CSV Parser**: Import orders from CSV files ([Documentation](docs/src/csv_parser.md))
- **Main Application**: Main entry point ([Documentation](docs/src/main.md))
- **Matching Engine**: Order matching engine

---

## Getting Started

### Compilation

Make sure you have `make` and a C++ compiler (g++ recommended):

```bash
make
```

### Running the Program

To run the main program, execute:

```bash
make run
```

### Running the Tests

To run all unit tests, execute:

```bash
make test
```

### Running Benchmarks

To run performance benchmarks, execute:

```bash
make bench
```

---

## Example Usage

See the [main documentation](docs/src/main.md) for usage examples and market simulation scenarios.

---

## Roadmap

- [x] CSV parser for data import
- [x] Interactive main application
- [x] Advanced matching engine
- [x] Optimizations and benchmarks

---

## License

This project is licensed under the MIT License.

---

