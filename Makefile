# Compiler
CXX = g++

# Default build type
BUILD_TYPE ?= release

# Flags
CXXFLAGS = -MMD -MP -std=c++20 -Werror -Wall -Wextra -Wpedantic -Wformat -Wmissing-include-dirs -Wuninitialized -Wunreachable-code -Wshadow -Wconversion -Wsign-conversion -Wredundant-decls -Winit-self -Wswitch-default -Wfloat-equal -Wunused-parameter -MMD -MP
ifeq ($(BUILD_TYPE),debug)
	CXXFLAGS += -g
	BIN_DIR = bin/debug
else ifeq ($(BUILD_TYPE),release)
	CXXFLAGS += -O2
	BIN_DIR = bin/release
else
	$(error BUILD_TYPE $(BUILD_TYPE) not supported)
endif

LDFLAG = -lstdc++

# Directories
SRC_DIR = src

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

# Rules
.PHONY: default toolchain clean

core = $(addprefix $(BIN_DIR)/, lexer.o term.o util.o)

default: $(core) $(BIN_DIR)/ntest.o
	@make tests

tests: $(core) $(BIN_DIR)/ntest.o $(BIN_DIR)/testing_main.o
	@$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$@ $^ $(LDFLAG)
	@echo 'compiling tests...'

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	@echo 'compiling [$<]...'
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -r -f bin/debug bin/release
	find . -name "*.d" -type f -delete

# Include the generated .d files
-include $(DEPS)
