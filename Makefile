CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
TARGET = $(BUILD_DIR)/order
TEST_ORDER_BOOK = $(BUILD_DIR)/test_order_book
TEST_ORDER = $(BUILD_DIR)/test_order

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(filter-out $(SRC_DIR)/main.cpp, $(SRCS)))

all: $(TARGET)

$(TARGET): $(OBJS) $(BUILD_DIR)/main.o
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_ORDER_BOOK): $(OBJS) $(BUILD_DIR)/test_order_book.o
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_ORDER): $(BUILD_DIR)/test_order.o
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.hpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/test_order_book.o: $(TEST_DIR)/test_order_book.cpp $(SRC_DIR)/order_book.hpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/test_order.o: $(TEST_DIR)/test_order.cpp $(SRC_DIR)/order.hpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test_order_book: $(TEST_ORDER_BOOK)
	./$(TEST_ORDER_BOOK)

test_order: $(TEST_ORDER)
	./$(TEST_ORDER)

test: test_order_book test_order
	@echo "All tests completed successfully!"

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean test test_order test_order_book