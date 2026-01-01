.PHONY: all build clean test run configure rebuild install docs docker helix-test helix-install

BUILD_DIR := build

all: build

configure:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..

build: configure
	@cmake --build $(BUILD_DIR) --parallel
	@$(BUILD_DIR)/bishop golden.b && mv golden $(BUILD_DIR)/golden

clean:
	@rm -rf $(BUILD_DIR)

rebuild: clean build

test: build
	@$(BUILD_DIR)/bishop test tests/

run: build
	@$(BUILD_DIR)/bishop $(ARGS)

install: build
	@mkdir -p ~/.local/bin
	@mkdir -p ~/.local/lib/bishop
	@mkdir -p ~/.local/include/bishop
	@mkdir -p ~/.local/include/bishop/fiber_asio
	@cp $(BUILD_DIR)/bishop ~/.local/bin/
	@cp $(BUILD_DIR)/lib/libbishop_std_runtime.a ~/.local/lib/bishop/
	@cp $(BUILD_DIR)/lib/libbishop_http_runtime.a ~/.local/lib/bishop/
	@cp $(BUILD_DIR)/lib/libllhttp.a ~/.local/lib/bishop/
	@cp $(BUILD_DIR)/include/bishop/std.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/std.hpp.gch ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/error.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/http.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/http.hpp.gch ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/fs.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/crypto.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/net.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/net.hpp.gch ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/process.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/regex.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/time.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/math.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/random.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/log.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/sync.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/channel.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/json.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/markdown.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/algo.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/yaml.hpp ~/.local/include/bishop/
	@cp $(BUILD_DIR)/include/bishop/fiber_asio/*.hpp ~/.local/include/bishop/fiber_asio/
	@cp $(BUILD_DIR)/include/llhttp.h ~/.local/include/
	@echo "Installed bishop to ~/.local/bin/"
	@echo "Installed runtime libraries to ~/.local/lib/bishop/"
	@echo "Installed headers to ~/.local/include/"
	@echo "Installed precompiled headers to ~/.local/include/bishop/"
	@$(MAKE) helix-install

docs: build
	@$(BUILD_DIR)/docgen src/ docs/reference/

helix-test:
	@echo "Testing Helix tree-sitter grammar..."
	@cd helix && npm install --silent
	@cd helix && npx tree-sitter generate
	@cd helix && npx tree-sitter test
	@echo "Helix grammar tests passed"

helix-install:
	@echo "Installing Helix syntax highlighting..."
	@mkdir -p ~/.config/helix/runtime/queries/bishop
	@mkdir -p ~/.config/helix/runtime/grammars
	@cp helix/queries/bishop/*.scm ~/.config/helix/runtime/queries/bishop/
	@cd helix && npm install --silent && npx tree-sitter generate
	@if [ -f helix/libtree-sitter-bishop.so ]; then \
		cp helix/libtree-sitter-bishop.so ~/.config/helix/runtime/grammars/bishop.so; \
	elif [ -f helix/bishop.so ]; then \
		cp helix/bishop.so ~/.config/helix/runtime/grammars/bishop.so; \
	else \
		cd helix && cc -shared -fPIC -o bishop.so src/parser.c -I src && \
		cp bishop.so ~/.config/helix/runtime/grammars/bishop.so; \
	fi
	@echo "Installed query files to ~/.config/helix/runtime/queries/bishop/"
	@echo "Installed grammar to ~/.config/helix/runtime/grammars/bishop.so"
	@echo "NOTE: Add bishop language config to ~/.config/helix/languages.toml"

docker:
	@cp ~/.claude/CLAUDE.md docker/CLAUDE.md
	@cp ~/.claude/.credentials.json docker/.credentials.json
	@cp ~/clank-hayen.2025-12-28.private-key.pem docker/app-private-key.pem
	@docker build -t claude-agent docker/
	@rm docker/CLAUDE.md docker/.credentials.json docker/app-private-key.pem
	@echo "Built claude-agent image"
