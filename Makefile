ifneq ($(OS),Windows_NT)
GXX=g++
GXXFLAGS=-std=c++20 -Wall -Wpedantic -Wextra -Wconversion -Werror
SRC_DIR=src
TEST_DIR=testing
OBJ_DIR=obj

subdirs=$(wildcard $(SRC_DIR)/*/ $(TEST_DIR)/*/)
outDirs=$(patsubst %,$(OBJ_DIR)/%,$(subdirs))

SRC_FILES=$(shell find . -wholename "./src/*.cpp")
SRC_FILES+=$(shell find . -wholename "./testing/*.cpp")
OBJ_FILES=$(patsubst .%.cpp,$(OBJ_DIR)%.o,$(SRC_FILES))

default:
	@mkdir -p $(OBJ_DIR)/$(SRC_DIR)
	@mkdir -p $(OBJ_DIR)/$(TEST_DIR)
ifneq ($(outDirs),)
	@mkdir -p $(outDirs)
endif
	@make all
	cd ./testing; \
	./testing.exe

all: $(OBJ_FILES)
	$(GXX) $(GXXFLAGS) $^ -o ./testing/testing.exe

$(OBJ_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(GXX) $(GXXFLAGS) -c $< -o $@

$(OBJ_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(GXX) $(GXXFLAGS) -c $< -o $@

clean:
	rm -r -f $(OBJ_DIR) main
else
default:
	@echo Error: Windows system dectected, this Makefile is meant for a linux based system
endif