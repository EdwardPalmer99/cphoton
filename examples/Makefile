SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

CC				:= gcc
CFLAGS			:= -std=gnu11 -g -Wall -Wno-missing-braces -O2

LIB_DIR			:= ../lib
LIB_BIN_DIR		:= ../bin/lib

BUILD_DIR		:= ../.build/examples
BIN_DIR			:= ../bin/examples

LDFLAGS			:= -L$(LIB_BIN_DIR) -lcphoton
INCLUDE_FLAGS	:= $(addprefix -I, $(shell find $(LIB_DIR) -type d))

define do_compile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE_FLAGS)
endef

define do_link
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)
endef

$(BUILD_DIR)/%.o: %.c
	$(call do_compile)

$(BIN_DIR)/batcave: $(BUILD_DIR)/BatCave.o
	$(call do_link)

$(BIN_DIR)/menger-cube: $(BUILD_DIR)/MengerCube.o 
	$(call do_link)

all: $(BIN_DIR)/batcave $(BIN_DIR)/menger-cube

.PHONY: clean 
clean: 
	rm -rf $(BUILD_DIR) $(BIN_DIR)