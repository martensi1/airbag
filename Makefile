CC := gcc
CC_FLAGS := -Wall -Werror -Wno-error=pointer-to-int-cast -g
LD_FLAGS := -rdynamic

OBJ_DIR := obj
BIN_DIR := bin

C_FILES = $(wildcard ./*.c)
C_OBJS = $(patsubst ./%.c, $(OBJ_DIR)/%.o, $(C_FILES))

TARGET := test
OUTPUT := $(BIN_DIR)/$(TARGET)


.PHONY: all clean

all: $(OUTPUT)

run: $(OUTPUT)
	@echo "-> Running $(OUTPUT)"
	@$(OUTPUT)

$(OUTPUT): $(C_OBJS)
	@echo "-> Linking object files..."
	@mkdir -p $(BIN_DIR)

	$(CC) $(LD_FLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)/$(dir $*)

	@echo "-> Compiling $<"
	$(CC) $(CC_FLAGS) -c $< -o $(OBJ_DIR)/$*.o


clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

