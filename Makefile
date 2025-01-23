# Reference: https://makefiletutorial.com/#the-all-target
CFLAGS	:= -std=c11 -g -Wall -O2
CC		:= gcc

TARGET_EXEC 	:= cphoton
INSTALL_DIR		:= install
BUILD_DIR		:= build
SRC_DIR 		:= src

# Find all source files:
SRCS	:= $(shell find $(SRC_DIR) -name '*.c' -o -name '*.m')

# Find all object files:
_OBJS	:= $(patsubst %.m, %.o, $(patsubst %.c, %.o, $(SRCS)))
OBJS	:= $(addprefix $(BUILD_DIR)/, $(_OBJS))

# Find all directories so compiler can find headers:
INC_DIRS := $(shell find $(SRC_DIR) -type d)

# Prepend directories with '-I':
INC_FLAGS := $(addprefix -I, $(INC_DIRS))

# Linker flags:
LDFLAGS := $(INC_FLAGS)

define do_compile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
endef

# Build step for c source files:
$(BUILD_DIR)/%.o: %.c
	$(call do_compile)

# Build step for obj-c source files:
$(BUILD_DIR)/%.o: %.m
	$(call do_compile)

# Final build step:
$(INSTALL_DIR)/$(TARGET_EXEC): $(OBJS)
	mkdir -p $(INSTALL_DIR)
	$(CC) $(OBJS) -o $@
	
.PHONY: debug
debug:
	@echo "srcs: $(SRCS)"
	@echo "objs: $(OBJS)"

.PHONY: clean 
clean: 
	rm -rf $(BUILD_DIR) $(INSTALL_DIR)