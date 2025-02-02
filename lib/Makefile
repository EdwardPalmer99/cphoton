SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

CC			:= gcc
CFLAGS		:= -std=gnu11 -g -Wall -Wno-missing-braces -O2 -fPIC

LIB_DIR		:= .
BUILD_DIR	:= ../.build/lib

TARGET 		:= libcphoton.so

SOURCES		:= $(shell find $(LIB_DIR) -name '*.c')
OBJECTS		:= $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(SOURCES)))

LDFLAGS 		:= -shared -lm
INCLUDE_FLAGS	:= $(addprefix -I, $(shell find . -type d))

define do_compile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE_FLAGS)
endef

define do_link
	$(CC) -o $@ $^ $(LDFLAGS)
endef

$(BUILD_DIR)/%.o: %.c
	$(call do_compile)

$(TARGET): $(OBJECTS)
	$(call do_link)

all: $(TARGET)

.PHONY: debug
debug:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"

.PHONY: clean 
clean: 
	rm -rf $(BUILD_DIR)