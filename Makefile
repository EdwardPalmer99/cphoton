BUILD_DIR		:= .build 
BIN_DIR			:= bin

LIB_DIR			:= lib 
EXAMPLES_DIR	:= examples

.PHONY: all $(LIB_DIR) $(EXAMPLES_DIR)

all: $(EXAMPLES_DIR)

$(EXAMPLES_DIR) $(LIB_DIR):
	$(MAKE) --directory=$@ all

$(EXAMPLES_DIR): $(LIB_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
