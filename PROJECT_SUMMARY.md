# Mylang — Project Summary

A minimal C-based interpreter for a custom language. No AST — parses and executes
on the fly using lexer position save/restore for loops.

## File Structure

```
├── main.c        # Entry point: reads source file, calls parser()
├── lexer.c/h     # Lexical analysis: tokenizes source into Token stream
├── parser.c/h    # Syntax analysis & execution: parses and runs statements
├── env.c/h       # Variable store: int/string key-value pairs
├── token.h       # Token type definitions (enum + struct)
├── Makefile      # Build (gcc -Wall -Wextra -g)
├── test.txt      # Test program (run: ./build/run test.txt)
├── PROJECT_SUMMARY.md  # This file
└── README.md           # Overview & usage examples
```

## Architecture

### Three-layer design

```
Source → [Lexer] → Token stream → [Parser] → Variable store
                                    ↓
                            executes on the fly
                            (no AST)
```

### Key design decisions

| Decision | Rationale |
|----------|-----------|
| **No AST** | Keeps the interpreter lightweight. Loops use lexer position save/restore (`lexer_get_pos`/`lexer_set_pos`) to re-parse conditions on each iteration |
| **Parser drives execution** | `parse_*` functions both parse AND execute — they return values (expressions) or execute side effects (statements) |
| **Static global state** | `lexer.c` uses `static int pos` + global `Token current_token`; `env.c` uses `static Var vars[MAX_VARS]` — no dynamic allocation needed for state |

### Expression chain (precedence)

```
parse_expr       → parse_comparison
parse_comparison → parse_addsub
parse_addsub     → parse_term
parse_term       → parse_primary
parse_primary    → number | identifier | '(' expr ')'
```

Precedence (high → low): `* /` → `+ -` → `== != < > <= >=`

### Statement dispatch

`parser()` → `parse_statement()` dispatches to:
- `parse_print()` — `print(expr)`
- `parse_assign()` — `name = value`
- `parse_block()` — `{ ... }`
- `parse_if()` — `if (cond) { ... } else { ... }`
- `parse_while()` — `while (cond) { ... }`
- `parse_for()` — `for (init; cond; incr) { ... }`

## Language Features

### Implemented (✅)
- `print()` — print string or integer expression
- Variables — `int` and `string` types (auto-deduced from assignment)
- Expressions — `+`, `-`, `*`, `/`, `()` with correct precedence
- Comparison — `==`, `!=`, `<`, `>`, `<=`, `>=` (lowest precedence)
- `if/else` — conditional with optional else
- `while` — loop with condition re-evaluated each iteration
- `for` — `for (init; cond; incr) { ... }` with separate position save for condition and increment
- `{ }` — block statements
- `//` — single-line comments

### Not implemented (❌)
- AST — deliberately omitted for simplicity
- Float numbers — all numbers are `int`
- String expressions — strings only in assignment and `print()`
- Functions / user-defined procedures

## Module Details

### Lexer (`lexer.c`)
- **Single-pass** tokenizer: `lex()` returns one `Token` per call
- **Keyword recognition**: uses `strncmp` + boundary check (non-alnum, non-`_`)
- **Multi-char operators**: `==`, `!=`, `<=`, `>=` checked before single-char switch
- **Comments**: `//` skips to end of line inside `skip_space()`
- **Position save/restore**: `lexer_get_pos()` / `lexer_set_pos()` exposed for loop re-parsing
- **String handling**: `copy_string()` mallocs a copy; caller must `free()`

### Parser (`parser.c`)
- **Recursive descent**: one function per grammar rule
- **Semicolon handling in `parse_for`**: manually checks `current_token.type == TOKEN_SEMICOLON` then saves position before calling `advance()` — `expect(TOKEN_SEMICOLON)` would advance past the separator, breaking position save
- **Block consumption**: `skip_block()` handles nested `{ }` with depth counter; `parse_block()` executes statements inside `{ }` — both call `expect(TOKEN_LBRACE)` to consume `{` themselves (no double-consumption)
- **For-loop execution order**: Condition → Block → Increment → Condition (NOT Condition → Increment → Block)
- **Error handling**: `syntax_error()` prints message + current token value, then `exit(1)` — no recovery

### Environment (`env.c`)
- **Static array**: `Var vars[MAX_VARS]` (256 entries) — no dynamic resize
- **Duplicate handling**: `env_set()` updates existing variable if name matches
- **Memory**: `strdup()` allocates; `env_free()` frees all at end of execution

## Code Style Conventions

All source files follow these rules:

| Aspect | Convention |
|--------|-----------|
| Indentation | 4 spaces (no tabs) |
| Braces | K&R style (opening brace on same line: `) {`) |
| Naming | `snake_case` for functions and variables |
| Comments | English, `//` style |
| `if`/`while`/`for` | Space after keyword: `if (cond)` |
| Commas | Space after comma in function calls |
| `=` | Single space on both sides |
| Includes | `#include <stdio.h>` (space after `#include`) |

## Build & Run

```sh
make           # builds build/run
make dev       # builds and runs test.txt
./build/run <file>  # run any .txt source
```

## Current Status

- **Compilation**: zero warnings with `-Wall -Wextra`
- **Test**: `test.txt` runs successfully (output below)
- **Test output**:
  ```
  hello
  world
  5
  6
  B
  55
  0
  1
  2
  3
  4
  ```