# Mylang (C)

A minimal interpreter written in C

> The name **"mylang" is temporary** and may change in the future.

## Project Structure

~~~text
├── main.c # Entry point
├── lexer.c # Lexical analysis
├── lexer.h
├── parser.c # Syntax analysis
├── parser.h
├── token.h # Token definitions
└── Makefile # Build
~~~

## Example

~~~mylang
print("Hello world")
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

## Roadmap

- [x] Hello World
- [ ] Variables
- [ ] Numbers
- [ ] Expressions
- [ ] AST
