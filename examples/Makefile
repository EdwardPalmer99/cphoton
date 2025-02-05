SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

CC				:= gcc
CFLAGS			:= -std=gnu11 -g -Wall -Wno-missing-braces -O2

# NB: need to provide abs path for linking!
EXAMPLES_DIR	:= $(dir $(abspath $(firstword Makefile)))
LIB_BIN_DIR		:= $(EXAMPLES_DIR)../bin/lib
LIB_DIR			:= ../lib

BUILD_DIR		:= ../.build/examples
BIN_DIR			:= ../bin/examples

LDFLAGS			:= -L$(LIB_BIN_DIR) -Wl,-rpath,$(LIB_BIN_DIR) -lcphoton
INCLUDE_FLAGS	:= -I$(LIB_DIR)

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

test: $(BIN_DIR)/batcave
	$(shell ./$(BIN_DIR)/batcave --path=/var/tmp/cphoton-render.ppm --width=256 --height=160 --samples-per-pixel=10 --maxDepth=10)
	rm -f /var/tmp/cphoton-render.ppm

print_location:
	@echo $(dir $(abspath $(firstword Makefile)))

.PHONY: clean 
clean: 
	rm -rf $(BUILD_DIR) $(BIN_DIR)