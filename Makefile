IDIR=.
CXX=g++
CXXFLAGS=-I$(IDIR)
LDFLAGS=-lpthread

# Директории
BUILD_DIR=build
BIN_DIR=bin
TEST_DIR=tests

# Основные файлы проекта
SRCS=$(filter-out main.cpp, $(wildcard *.cpp))
OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
MAIN_OBJ=$(BUILD_DIR)/main.o
PROJECT=main
DEPS=Pixel.h BMPPixelMatrix.h BMPFile.h

# Тестовые файлы
TEST_SRCS=$(wildcard $(TEST_DIR)/test_*.cpp)
TEST_OBJS=$(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))
TEST_TARGET=run_tests

all: $(BIN_DIR)/$(PROJECT)

# Создание директорий
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Компиляция основных файлов
$(BUILD_DIR)/%.o: %.cpp $(DEPS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Сборка основного проекта
$(BIN_DIR)/$(PROJECT): $(OBJS) $(MAIN_OBJ) | $(BIN_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Компиляция тестов
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp $(DEPS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Сборка и запуск тестов
test: $(BIN_DIR)/$(TEST_TARGET)
	$(BIN_DIR)/$(TEST_TARGET)

$(BIN_DIR)/$(TEST_TARGET): $(OBJS) $(TEST_OBJS) | $(BIN_DIR)
	$(CXX) -o $@ $^ -lgtest -lgtest_main $(LDFLAGS)

format:
	find . \( -name '*.cpp' -o -name '*.h' \) -exec clang-format -i {} \;

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

gen-doc:
	doxygen Doxyfile

.PHONY: clean $(PROJECT) test format gen-doc
