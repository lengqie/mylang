#!/bin/sh
# Run every tests/*.my and compare its output with tests/<name>.expected.
#
#   tests/<name>.my        the program to run
#   tests/<name>.expected  expected stdout+stderr
#   tests/<name>.exit      expected exit status (optional, default 0)
#
# Usage: tools/run-tests.sh [binary]   (default: build/run)

set -u

BIN=${1:-build/run}
TESTS_DIR=tests

if [ ! -x "$BIN" ]; then
    echo "run-tests: no executable at '$BIN' (build it first: make)" >&2
    exit 1
fi

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

pass=0
fail=0

for src in "$TESTS_DIR"/*.my; do
    name=$(basename "$src" .my)
    expected="$TESTS_DIR/$name.expected"

    if [ ! -f "$expected" ]; then
        echo "FAIL $name: missing $expected"
        fail=$((fail + 1))
        continue
    fi

    want_exit=0
    if [ -f "$TESTS_DIR/$name.exit" ]; then
        want_exit=$(cat "$TESTS_DIR/$name.exit")
    fi

    "$BIN" "$src" >"$tmp/out" 2>&1
    got_exit=$?

    if [ "$got_exit" != "$want_exit" ]; then
        echo "FAIL $name: exit status $got_exit, expected $want_exit"
        fail=$((fail + 1))
        continue
    fi

    if diff -u "$expected" "$tmp/out" >"$tmp/diff"; then
        echo "ok   $name"
        pass=$((pass + 1))
    else
        echo "FAIL $name:"
        sed 's/^/     /' "$tmp/diff"
        fail=$((fail + 1))
    fi
done

echo "$pass passed, $fail failed"
[ "$fail" -eq 0 ]
