IDIR=.
CXX=g++
CXXFLAGS=-I$(IDIR)
LDFLAGS=-lpthread

# Directories
BUILD_DIR := build
BIN_DIR := bin
SRC_DIR := src
INCLUDE_DIR := include
TEST_DIR := tests

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
SRCS := $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp)) # Excluded main.cpp for testing
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
TARGET := main 

TEST_SRCS := $(wildcard $(TEST_DIR)/test_*.cpp)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))
TEST_TARGET := run_tests

DEPS := $(wildcard $(INCLUDE_DIR)/*.h)

all: $(BIN_DIR)/$(TARGET)

# Build main executable (without gtest)
$(BIN_DIR)/$(TARGET): $(OBJS) $(BUILD_DIR)/main.o
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

test: $(BIN_DIR)/$(TEST_TARGET)
	$(BIN_DIR)/$(TEST_TARGET)

$(BIN_DIR)/$(TEST_TARGET): $(OBJS) $(TEST_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ -lgtest -lgtest_main $(LDFLAGS)

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp $(DEPS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

format:
	find . \( -name '*.cpp' -o -name '*.h' \) -print0 | xargs -0 clang-format -i

gen-docs:
	doxygen Doxyfile

.PHONY: clean $(PROJECT) test format gen-doc
