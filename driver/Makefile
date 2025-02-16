SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .o

CXX			:= g++
CXXFLAGS	:= -g -Wall -Wno-missing-braces -O2

DRIVER_DIR	:= $(dir $(abspath $(firstword Makefile)))
LIB_BIN_DIR	:= $(DRIVER_DIR)../bin/lib
LIB_DIR		:= ../lib

BUILD_DIR	:= ../.build/driver
BIN_DIR 	:= ../bin/driver

SOURCES		:= $(shell find . -name '*.cpp')
OBJECTS		:= $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(SOURCES)))

LDFLAGS			:= -L$(LIB_BIN_DIR) -Wl,-rpath,$(LIB_BIN_DIR) -lcphoton
INCLUDE_FLAGS	:= -I. -I$(LIB_DIR)

define do_compile
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INCLUDE_FLAGS)
endef

define do_link
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $^ $(LDFLAGS)
endef

$(BUILD_DIR)/%.o: %.cpp
	$(call do_compile)

$(BIN_DIR)/cphoton-yaml: $(OBJECTS)
	$(call do_link)

all: $(BIN_DIR)/cphoton-yaml

.PHONY: debug
debug:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"

.PHONY: clean 
clean: 
	rm -rf $(BUILD_DIR)