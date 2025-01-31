SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS		:= -std=gnu11 -g -Wall -Wno-missing-braces -O2 -fPIC
CC			:= gcc
LDFLAGS 	:= -shared -lm

TARGET 		:= libcphoton.so

BUILD_DIR		:= ../build/lib
PREFIX 			:= /usr

SOURCES			:= $(shell find -name '*.c')
HEADERS			:= $(shell find -name '*.h')
OBJECTS			:= $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(SOURCES)))

INCLUDE_FLAGS	:= $(addprefix -I, $(shell find $(LIB_DIR) -type d))

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE_FLAGS)

$(LIB_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

all: $(TARGET)

install: $(TARGET)
	install -d  $(PREFIX)/include/cphoton/
	install -m 644 $(TARGET) $(PREFIX)/lib/
	install -m 644 $(HEADERS) $(PREFIX)/include/cphoton/

uninstall:
	rm -rf $(PREFIX)/lib/$(TARGET) $(PREFIX)/include/cphoton

.PHONY: debug
debug:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"

.PHONY: clean 
clean: 
	rm -rf $(BUILD_DIR) $(TARGET)