# Mylang (C)

A minimal interpreter written in C

> The name **"mylang" is temporary** and may change in the future.

## Project Structure

~~~text
├── main.c       # Entry point
├── lexer.c      # Lexical analysis
├── lexer.h
├── parser.c     # Syntax analysis
├── parser.h
├── token.h      # Token definitions
├── env.c        # Variable store
├── env.h
└── Makefile     # Build
~~~

## Example

~~~mylang
x = "hello"
print(x)
print("Hello world")

// expressions
sum = (1 + 2) * 3
print(sum)

// comparison & condition
score = 85
if (score >= 60) {
    print("pass")
} else {
    print("fail")
}

// loops
i = 1
sum = 0
while (i <= 10) {
    sum = sum + i
    i = i + 1
}
print(sum)

// for loop
for (k = 0; k < 5; k = k + 1) {
    print(k)
}
~~~

## Build

Make

~~~sh
make 
~~~

Run

~~~sh
./run test.txt
~~~

## Features

- [x] `print()` statement
- [x] Variables (string & integer)
- [x] Expressions: `+`, `-`, `*`, `/`, `()`
- [x] Comparison operators: `==`, `!=`, `<`, `>`, `<=`, `>=`
- [x] Operator precedence: `* /` > `+ -` > comparison
- [x] `if/else` conditional
- [x] `while` loop
- [x] `for` loop (init; cond; incr)
- [x] Block statements `{ ... }`
- [x] Single-line comments `//`
- [ ] AST
- [ ] Float numbers
- [ ] Strings in expressions
- [ ] Functions
