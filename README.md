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
├── Makefile     # Build
├── tests/       # Test programs and their expected output
└── tools/       # Test runner
~~~

## Example

~~~mylang
x = "hello"
print(x)
print("Hello world")
~~~

## Build

Make

~~~sh
make 
~~~

Run

~~~sh
./build/run test.txt
~~~

## Development

~~~sh
make        # build -> build/run
make test   # run every tests/*.my and compare with tests/<name>.expected
make dev    # build and run test.txt
~~~

Every test is a pair of files:

~~~text
tests/<name>.my         the program to run
tests/<name>.expected   expected output (stdout + stderr)
tests/<name>.exit       expected exit status (optional, default 0)
~~~

CI builds with `-Wall -Wextra -Werror` and runs `make test` on every push.

## Roadmap

- [x] Hello World
- [x] Variables
- [ ] Numbers
- [ ] Expressions
- [ ] AST
