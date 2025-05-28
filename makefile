# Compiler dan flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude -Iinclude/SDL3 -Iinclude/zketch

# Direktori
SRC_DIR := src
MODULES_DIR := $(SRC_DIR)/modules
OBJ_DIR := build/src
BIN_DIR := bin
BIN := $(BIN_DIR)/zketch.exe

# File sumber dan object
SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(MODULES_DIR)/*.cpp)
OBJECTS := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Library
LIB_DIR := lib
LIBS := -L$(LIB_DIR) -lSDL3 -lSDL3_image -lSDL3_ttf

# Target default
all: $(BIN)

# Buat direktori jika belum ada dan kompilasi .cpp jadi .o
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linking semua object file jadi executable
$(BIN): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LIBS)

# Jalankan program
run: all
	@./$(BIN) && $(MAKE) clean

# Bersihkan build
clean:
	@echo Cleaning...
	@rm -rf build $(BIN)

.PHONY: all run clean
