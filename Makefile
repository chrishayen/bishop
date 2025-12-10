.PHONY: all build clean test run configure rebuild

BUILD_DIR := build

all: build

configure:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..

build: configure
	@cmake --build $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)

rebuild: clean build

test: build
	@cd $(BUILD_DIR) && ctest --output-on-failure

run: build
	@$(BUILD_DIR)/nog $(ARGS)
