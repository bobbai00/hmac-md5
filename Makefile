CC := g++
FLAGS := -std=c++11 -w
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
TEST_DIR := test
INCLUDE := -I./$(INC_DIR)


$(BIN_DIR)/main: $(BUILD_DIR)/main.o $(BUILD_DIR)/HMD5.o $(BUILD_DIR)/InputConverter.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE) $(LIB) $^ -o $@
	./bin/main

test: $(BUILD_DIR)/test.o $(BUILD_DIR)/InputConverter.o $(BUILD_DIR)/HMD5.o
	$(CC) $(FLAGS) $(INCLUDE) $(LIB) $^ -o $@
	./test
	@rm test


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) $(LIB) -c -o $@ $<

.PHONY: clean


clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)