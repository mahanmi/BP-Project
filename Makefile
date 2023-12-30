SRC_DIR = src
BUILD_DIR = build
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAMES = $(patsubst $(SRC_DIR)/%.cpp,%,$(SRC_FILES))
INCLUDE_PATHS = -Iinc
LIBRARY_PATHS = -Llib/SDL2 -Llib/SDL2_image -Llib/SDL2_mixer -Llib/SDL2_ttf -Llib/SDL2_gfx
COMPILER_FLAGS = -std=c++11 -Wall -o0 -g
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx

all: $(OBJ_NAMES)

$(OBJ_NAMES): %: $(SRC_DIR)/%.cpp
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $< -o $(BUILD_DIR)/$@
