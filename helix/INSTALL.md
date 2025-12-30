# Installing Bishop Syntax Highlighting for Helix

This guide explains how to set up syntax highlighting for the Bishop programming language in the Helix editor.

## Quick Start

### Option 1: Automatic Installation (Recommended)

Once the tree-sitter grammar is published, add to your Helix `languages.toml`:

```toml
[[language]]
name = "bishop"
scope = "source.bishop"
file-types = ["b"]
comment-token = "//"
indent = { tab-width = 4, unit = "    " }

[[grammar]]
name = "bishop"
source = { git = "https://github.com/chrishayen/tree-sitter-bishop", rev = "main" }
```

Then run:
```bash
hx --grammar fetch
hx --grammar build
```

### Option 2: Local Installation

#### Step 1: Copy Query Files

```bash
# Linux
mkdir -p ~/.config/helix/runtime/queries/bishop
cp /path/to/bishop/helix/queries/bishop/*.scm ~/.config/helix/runtime/queries/bishop/

# macOS
mkdir -p ~/Library/Application\ Support/Helix/runtime/queries/bishop
cp /path/to/bishop/helix/queries/bishop/*.scm ~/Library/Application\ Support/Helix/runtime/queries/bishop/

# Windows (PowerShell)
mkdir -p "$env:APPDATA\helix\runtime\queries\bishop"
cp /path/to/bishop/helix/queries/bishop/*.scm "$env:APPDATA\helix\runtime\queries\bishop\"
```

#### Step 2: Add Language Configuration

Add to your `~/.config/helix/languages.toml`:

```toml
[[language]]
name = "bishop"
scope = "source.bishop"
file-types = ["b"]
comment-token = "//"
indent = { tab-width = 4, unit = "    " }
roots = ["bishop.toml"]
auto-pairs = true

[[grammar]]
name = "bishop"
source = { path = "/path/to/bishop/helix" }
```

#### Step 3: Build the Grammar

```bash
cd /path/to/bishop/helix
npm install
npx tree-sitter generate
hx --grammar build
```

## Verification

Open a `.b` file in Helix to test:

```bash
hx examples/http_server.b
```

You should see syntax highlighting for:
- Keywords (`fn`, `struct`, `if`, `else`, `for`, `while`, `return`, etc.)
- Types (`int`, `str`, `bool`, `List<T>`, `Channel<T>`, etc.)
- Strings (double and single quoted)
- Numbers (integers and floats)
- Comments (line `//` and doc `///`)
- Function definitions and calls
- Struct definitions and instantiations
- Error handling (`fail`, `or`, `match err`)
- Concurrency (`go`, `Channel`, `select`)

## Features

### Syntax Highlighting
Full highlighting for all Bishop language constructs including:
- Primitive types: `int`, `str`, `bool`, `f32`, `f64`, `u32`, `u64`
- Generic types: `List<T>`, `Channel<T>`, `Pair<K,V>`, `Tuple<...>`
- Module-qualified types: `http.Request`, `http.Response`
- Optional types: `int?`
- Pointer types: `&int`
- Function types: `fn(int, int) -> int`

### Code Navigation
Text objects for navigating:
- Functions (`mif`, `maf` - inner/around function)
- Classes/Structs (`mic`, `mac` - inner/around class)
- Parameters (`mip`, `map` - inner/around parameter)
- Comments (`miq`, `maq` - inner/around comment)
- Loops (`mil`, `mal` - inner/around loop)
- Conditionals (`mii`, `mai` - inner/around if)

### Auto-Indentation
Automatic indentation for:
- Function and method bodies
- Struct definitions
- Control flow blocks (if, while, for)
- Match expressions
- Select statements

### Scope-Aware Highlighting
Local variable tracking for accurate reference highlighting.

## Troubleshooting

### No Highlighting Appears

1. Verify query files exist:
   ```bash
   ls ~/.config/helix/runtime/queries/bishop/
   # Should show: highlights.scm, locals.scm, indents.scm, etc.
   ```

2. Check grammar is built:
   ```bash
   hx --health bishop
   ```

3. Verify file type detection:
   Open a `.b` file and check the status bar shows "bishop"

### Grammar Build Fails

1. Ensure Node.js 18+ is installed
2. Update tree-sitter-cli:
   ```bash
   npm update tree-sitter-cli
   ```
3. Regenerate:
   ```bash
   npx tree-sitter generate
   ```

### Wrong Colors

Ensure your Helix theme supports the capture names used. Standard themes (e.g., `catppuccin`, `gruvbox`, `onedark`) work correctly.

## File Structure

```
helix/
├── grammar.js           # Tree-sitter grammar definition
├── package.json         # Node.js package configuration
├── tree-sitter.json     # Tree-sitter configuration
├── languages.toml       # Helix language configuration template
├── INSTALL.md           # This installation guide
├── test/
│   └── corpus/          # Grammar test cases
│       ├── definitions.txt
│       ├── expressions.txt
│       ├── control_flow.txt
│       └── error_handling.txt
└── queries/
    └── bishop/
        ├── highlights.scm   # Syntax highlighting rules
        ├── locals.scm       # Scope-aware highlighting
        ├── indents.scm      # Auto-indentation rules
        ├── injections.scm   # Language injection rules
        └── textobjects.scm  # Code navigation objects
```

## Contributing

To improve syntax highlighting:

1. Edit the relevant `.scm` file in `queries/bishop/`
2. Test with `npx tree-sitter highlight yourfile.b`
3. Verify against various Bishop code examples
4. Submit a PR to the Bishop repository

## License

MIT License - Same as the Bishop project.
