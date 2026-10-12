# mylang language specification

Status: draft - every section below is open until the maintainer fills it in.
Owner: the maintainer. This document is the source of truth for the language; agents
implement it and do not edit it.

Legend: `done` = implemented, `planned` = specified but not implemented, `open` = not
decided yet.

How to use this document:

- Fill in the `open` cells. A decision counts only once it is written here.
- Every example row points at a file under `tests/`, so `make test` keeps this document
  honest.
- Record a changed decision in section 10 instead of editing the body silently.

## 0. Overview

`open`: one paragraph - what mylang is for, and what it is deliberately not for.

## 1. Source text and lexical structure

| item | rule | status |
|---|---|---|
| source encoding | open | open |
| whitespace | open | open |
| line comments | open | open |
| block comments | open | open |
| integer literal | open | open |
| string literal | open | open |
| identifier | open | open |
| keywords | open | open |
| punctuation and operators | open | open |

Open points:

- Are newlines significant?
- Are two statements allowed on one line?
- What happens on an unterminated string, or on a character that is not a token?

## 2. Values and types

| type | literal form | representation | status |
|---|---|---|---|
| open | open | open | open |

Conversion and mixed-type rules:

| expression | behaviour | status |
|---|---|---|
| `"5" + 1` | open | open |
| `1 + "5"` | open | open |
| `1 == "5"` | open | open |
| `print(1 + 2)` | open | open |

Open points:

- Is a variable's type fixed after its first assignment?
- What happens when a value is used where another type is required?

## 3. Expressions

Precedence, highest level first:

| level | operators | associativity | status |
|---|---|---|---|
| open | open | open | open |

Open points:

- Do operators of equal precedence associate left or right? (`10 - 2 - 3`)
- Are unary operators allowed, and at which level? (`-5`, `!x`)

Evaluation rules:

| case | behaviour | status |
|---|---|---|
| division by zero | open | open |
| modulo by zero | open | open |
| integer overflow | open | open |

## 4. Statements

Copy the block below once per statement kind.

### 4.1 `<statement>` - status: open

Syntax:

```ebnf
open
```

Semantics: `open` - two or three sentences: what it does, where its values come from, and
when it fails.

Errors:

| condition | message | exit status | status |
|---|---|---|---|
| open | open | open | open |

Examples:

| test file | program | output |
|---|---|---|
| `tests/<name>.my` | open | open |

Blocks and assertions:

| question | answer | status |
|---|---|---|
| Are braces required after `if` / `while` / `for`? | open | open |
| Is `else if` supported? | open | open |
| Is an empty block allowed? | open | open |

Variables and scoping:

| question | answer | status |
|---|---|---|
| Does a block introduce a new scope? | open | open |
| Can a variable be declared without a value? | open | open |
| What happens when an undefined variable is read? | open | open |

## 5. Programs

| question | answer | status |
|---|---|---|
| Where does execution start? | open | open |
| Can a program be empty? | open | open |
| Is a statement separator required? | open | open |
| Are top-level declarations allowed? | open | open |

## 6. Diagnostics

| aspect | rule | status |
|---|---|---|
| message format | open | open |
| output stream (stdout or stderr) | open | open |
| exit status | open | open |
| line and column numbers | open | open |
| one error per run, or several | open | open |
| syntax errors compared to runtime errors | open | open |

## 7. Examples index

Every row must point at a file that exists under `tests/`.

| specification section | test file | program | output |
|---|---|---|---|
| open | open | open | open |

## 8. Non-goals

`open`: features that are deliberately out of scope, one per line.

## 9. Open questions

Unresolved points, one per line; move a line into the body once it is decided.

- open

## 10. Decision log

One entry per decision that changes this document, newest first.

- open
