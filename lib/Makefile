SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .c .o

CC			:= gcc
CXX 		:= g++

CFLAGS		:= -std=gnu11 -g -Wall -Wno-missing-braces -O2 -fPIC
CXXFLAGS	:= -g -Wall -Wno-missing-braces -O2

LIB_DIR		:= .
BUILD_DIR	:= ../.build/lib
BIN_DIR		:= ../bin/lib

TARGET 		:= libcphoton.so

SOURCES		:= $(shell find $(LIB_DIR) -name '*.c')
OBJECTS		:= $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(SOURCES)))

SOURCES_CPP	:= $(shell find $(LIB_DIR) -name '*.cpp')
OBJECTS_CPP	:= $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(SOURCES_CPP)))

# NB: need to link against math library (Linux) and C++ standard library since using C++ wrappers.
LDFLAGS 		:= -shared -lm -lstdc++
INCLUDE_FLAGS	:= -I$(LIB_DIR)

define do_compile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE_FLAGS)
endef

define do_compile_cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INCLUDE_FLAGS)
endef

define do_link
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)
endef

$(BUILD_DIR)/%.o: %.c
	$(call do_compile)

$(BUILD_DIR)/%.o: %.cpp
	$(call do_compile_cpp)

$(BIN_DIR)/$(TARGET): $(OBJECTS) $(OBJECTS_CPP)
	$(call do_link)

all: $(BIN_DIR)/$(TARGET)

.PHONY: debug
debug:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"

.PHONY: clean 
clean: 
	rm -rf $(BUILD_DIR) $(BIN_DIR)