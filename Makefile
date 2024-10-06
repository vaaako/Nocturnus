CXX = clang++
CXXFLAGS = -std=c++20 -I./include -Wall -Wextra -MMD -MP

BUILD_DIR = build
OBJS_DIR = $(BUILD_DIR)/objs
TEST_OBJS_DIR = $(BUILD_DIR)/testobjs

SRC_FILES = $(wildcard src/*.cpp src/*/*.cpp)
TEST_SRC_FILES = $(wildcard test/*.cpp test/*/*.cpp)
OBJS = $(patsubst src/%.cpp, $(OBJS_DIR)/%.o, $(SRC_FILES))

TEST_OBJS = $(patsubst test/%.cpp, $(TEST_OBJS_DIR)/%.o, $(TEST_SRC_FILES))

# Provisory name
TARGET = nocturnus

# Default is build library
all: $(TARGET)

# Build library
$(TARGET) : $(OBJS)
	ar rcs $(BUILD_DIR)/lib$@.a $(OBJS)

# Compile library and test files
dev: $(TARGET) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) -o $(BUILD_DIR)/$(TARGET) -L./$(BUILD_DIR) -lnocturnus

# Change paramters if compiling to windows
win: TARGET = nocturnus.exe
win: CXXFLAGS += -mconsole -static
win: $(TARGET) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) -o $(BUILD_DIR)/$(TARGET) -L./$(BUILD_DIR) -lnocturnus

# Compile library files
$(OBJS_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compiler test files
$(TEST_OBJS_DIR)/%.o: test/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*$(TARGET)* $(OBJS_DIR) $(TEST_OBJS_DIR)

.PHONY: all clean dev win
