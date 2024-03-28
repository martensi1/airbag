CC := gcc
CC_FLAGS := -Wall -Werror -Wno-error=pointer-to-int-cast -g
LD_FLAGS := -rdynamic

OBJ_DIR := obj
BIN_DIR := bin
INC_DIR := ../

BADBOY_OUTPUT := $(BIN_DIR)/badboy
BADBOY_SRC = badboy.c .airbag.c
BADBOY_OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(BADBOY_SRC))

AUTOTEST_OUTPUT := $(BIN_DIR)/autotest
AUTOTEST_SRC = autotest.c
AUTOTEST_OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(AUTOTEST_SRC))


define compile
	@mkdir -p $(OBJ_DIR)/$(dir $*)
	@echo "-> Compiling $<"
	$(CC) $(CC_FLAGS) -c $< -I$(INC_DIR) -o $(OBJ_DIR)/$*.o -Wa,-aln=$(OBJ_DIR)/$*.s
endef

define link
	@echo "-> Linking object files..."
	@mkdir -p $(BIN_DIR)
	$(CC) $(LD_FLAGS) -o $@ $^
endef


.PHONY: all clean badboy autotest

all: badboy autotest

badboy: $(BADBOY_OUTPUT)
autotest: $(AUTOTEST_OUTPUT)

$(BADBOY_OUTPUT): $(BADBOY_OBJS)
	@$(link)

$(AUTOTEST_OUTPUT): $(AUTOTEST_OBJS)
	$(link)

$(OBJ_DIR)/%.o: %.c
	$(compile)

.airbag.c: ../airbag.c
	cp $< $@

clean:
	rm -rf $(OBJ_DIR)/ $(BIN_DIR)/
	rm -f .airbag.c

