CC := gcc
CC_FLAGS := -Wall -Werror -Wno-error=pointer-to-int-cast -g
LD_FLAGS := -rdynamic

OBJ_DIR := obj
BIN_DIR := bin
INC_DIR := ../

BADBOY_OUTPUT := $(BIN_DIR)/badboy
BADBOY_SRC = badboy.c ../airbag.c
BADBOY_OBJS = $(patsubst ./%.c, $(OBJ_DIR)/%.o, $(BADBOY_SRC))

AUTOTEST_OUTPUT := $(BIN_DIR)/autotest
AUTOTEST_SRC = autotest.c
AUTOTEST_OBJS = $(patsubst ./%.c, $(OBJ_DIR)/%.o, $(AUTOTEST_SRC))



TARGET := badboy
OUTPUT := $(BIN_DIR)/$(TARGET)


.PHONY: all clean badboy autotest

all: badboy autotest

badboy: $(BADBOY_OUTPUT)
autotest: $(AUTOTEST_OUTPUT)

$(BADBOY_OUTPUT): $(BADBOY_OBJS)
	@echo "-> Linking object files..."
	@mkdir -p $(BIN_DIR)

	$(CC) $(LD_FLAGS) -o $@ $^

$(AUTOTEST_OUTPUT): $(AUTOTEST_OBJS)
	@echo "-> Linking object files..."
	@mkdir -p $(BIN_DIR)

	$(CC) $(LD_FLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)/$(dir $*)

	@echo "-> Compiling $<"
	$(CC) $(CC_FLAGS) -c $< -I$(INC_DIR) -o $(OBJ_DIR)/$*.o


clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

