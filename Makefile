# ==========================================================================
# Makefile pour le projet de Matching Engine C++
# Date: Juin 2025
# ==========================================================================

# ===== Configuration du compilateur =====
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic -O2

# ===== Structure des répertoires =====
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
DATA_DIR = data

# ===== Définition des exécutables =====
TARGET = $(BUILD_DIR)/order
TEST_ORDER_BOOK = $(BUILD_DIR)/test_order_book
TEST_ORDER = $(BUILD_DIR)/test_order
TEST_CSV_PARSER = $(BUILD_DIR)/test_csv_parser
TEST_CSV_WRITER = $(BUILD_DIR)/test_csv_writer
TEST_MATCHING_ENGINE = $(BUILD_DIR)/test_matching_engine
BENCHMARK = $(BUILD_DIR)/benchmark

# ===== Configuration automatique des fichiers objets =====
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(filter-out $(SRC_DIR)/main.cpp, $(SRCS)))

# ===== Cibles principales =====
all: $(TARGET)

# ===== Règles de construction des exécutables =====
$(TARGET): $(OBJS) $(BUILD_DIR)/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_ORDER_BOOK): $(OBJS) $(BUILD_DIR)/test_order_book.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_ORDER): $(BUILD_DIR)/test_order.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_CSV_PARSER): $(OBJS) $(BUILD_DIR)/test_csv_parser.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_CSV_WRITER): $(OBJS) $(BUILD_DIR)/test_csv_writer.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_MATCHING_ENGINE): $(OBJS) $(BUILD_DIR)/test_matching_engine.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BENCHMARK): $(OBJS) $(BUILD_DIR)/benchmark.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# ===== Règles de compilation des fichiers source =====
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ===== Règles spéciales pour les fichiers sans .hpp correspondant =====
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/benchmark.o: $(TEST_DIR)/benchmark.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ===== Compilation des tests =====
$(BUILD_DIR)/test_order_book.o: $(TEST_DIR)/test_order_book.cpp $(SRC_DIR)/order_book.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/test_order.o: $(TEST_DIR)/test_order.cpp $(SRC_DIR)/order.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/test_csv_parser.o: $(TEST_DIR)/test_csv_parser.cpp $(SRC_DIR)/csv_parser.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/test_csv_writer.o: $(TEST_DIR)/test_csv_writer.cpp $(SRC_DIR)/csv_writer.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/test_matching_engine.o: $(TEST_DIR)/test_matching_engine.cpp $(SRC_DIR)/matching_engine.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ===== Cibles d'exécution =====
run: $(TARGET)
	./$(TARGET) $(DATA_DIR)/input.csv $(DATA_DIR)/output.csv

# ===== Cibles d'exécution des tests =====
test_order_book: $(TEST_ORDER_BOOK)
	./$(TEST_ORDER_BOOK)

test_order: $(TEST_ORDER)
	./$(TEST_ORDER)

test_csv_parser: $(TEST_CSV_PARSER)
	./$(TEST_CSV_PARSER)

test_csv_writer: $(TEST_CSV_WRITER)
	./$(TEST_CSV_WRITER)

test_matching_engine: $(TEST_MATCHING_ENGINE)
	./$(TEST_MATCHING_ENGINE)

test: test_order_book test_order test_csv_parser test_csv_writer test_matching_engine
	@echo "All tests completed successfully!"

# ===== Benchmarking =====
bench: $(BENCHMARK)
	./$(BENCHMARK)

# ===== Nettoyage =====
clean:
	rm -rf $(BUILD_DIR)

# ===== Création des répertoires =====
init:
	@mkdir -p $(BUILD_DIR) $(DATA_DIR)
	@echo "Workspace initialized successfully!"

# ===== Déclaration des cibles factices =====
.PHONY: all clean test test_order test_order_book run bench init