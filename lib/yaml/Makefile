SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

CC			:= g++
CFLAGS		:= -g -Wall -Wno-missing-braces -O2

LIB_DIR		:= .
BUILD_DIR	:= .build

TARGET		:= yaml

SOURCES		:= $(shell find $(LIB_DIR) -name '*.cpp')
OBJECTS		:= $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(SOURCES)))

LIB_BIN_DIR		:= ../../bin/lib

LDFLAGS			:= -L$(LIB_BIN_DIR) -Wl,-rpath,$(LIB_BIN_DIR) -lcphoton
INCLUDE_FLAGS	:= -I../

define do_compile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE_FLAGS)
endef

define do_link
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)
endef

$(BUILD_DIR)/%.o: %.cpp
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