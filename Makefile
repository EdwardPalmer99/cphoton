SHELL = /bin/sh

BUILD_DIR	:= .build

lib: 
	$(MAKE) -C $@

examples:
	$(MAKE) -C $@

all: lib examples

clean:
	rm -rf $(BUILD_DIR)

.PHONY: lib examples clean