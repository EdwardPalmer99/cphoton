BUILD_DIR		:= .build 
BIN_DIR			:= bin

LIB_DIR			:= lib 
EXAMPLES_DIR	:= examples
DRIVER_DIR 		:= driver

.PHONY: all $(LIB_DIR) $(EXAMPLES_DIR) $(DRIVER_DIR)

all: $(EXAMPLES_DIR) $(DRIVER_DIR)

$(EXAMPLES_DIR) $(LIB_DIR) $(DRIVER_DIR):
	$(MAKE) --directory=$@ all

$(EXAMPLES_DIR): $(LIB_DIR)

$(DRIVER_DIR): $(LIB_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
