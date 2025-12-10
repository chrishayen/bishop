.PHONY: all build clean test run configure rebuild install

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

install: build
	@mkdir -p ~/.local/bin
	@cp $(BUILD_DIR)/nog ~/.local/bin/
	@echo "Installed nog to ~/.local/bin/"
