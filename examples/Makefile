SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS			:= -std=gnu11 -g -Wall -Wno-missing-braces -O2
CC				:= gcc

BUILD_DIR		:= ../build/examples
LDFLAGS			:=  -lcphoton -lm
PREFIX			:= /usr

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ 

all: batcave menger-cube

batcave: $(BUILD_DIR)/BatCave.o
	$(CC) -o batcave $(BUILD_DIR)/BatCave.o $(LDFLAGS)

menger-cube: $(BUILD_DIR)/MengerCube.o 
	$(CC) -o menger-cube $(BUILD_DIR)/MengerCube.o $(LDFLAGS)

install: batcave menger-cube 
	install batcave $(PREFIX)/bin/
	install menger-cube $(PREFIX)/bin/

uninstall:
	rm -rf $(PREFIX)/bin/batcave $(PREFIX)/bin/menger-cube

.PHONY: clean 
clean: 
	rm -rf $(BUILD_DIR) batcave menger-cube