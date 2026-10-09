# AGENTS.md

Instructions for coding agents working on **mylang**, a minimal interpreter written in C.

## What this project is

A small language interpreter: source text -> lexer -> parser, executed on the fly
(there is no AST). It is deliberately tiny and readable; keeping it that way matters
more than adding features quickly.

## Where the language is defined

The language itself (syntax, semantics, types, error messages) is specified by the
maintainer in the syntax specification document kept in this repository. That document
is the source of truth; this file only says how to work on the code.

- Agents implement what the specification says. Do not invent syntax, and do not treat a
  PR description, a comment, or this file as a spec.
- When the specification is silent or ambiguous on a point the task depends on, ask
  before choosing a behaviour.
- The maintainer owns the specification. Agents do not edit it unless asked to.

## Ground rules

1. **English only.** Code comments, docs, commit messages, PR titles and bodies, test
   fixtures: everything in the repository is written in English. Chat with the
   maintainer may be in Chinese.
2. **One feature per pull request.** A PR adds exactly one language capability (one
   operator family, one statement type, one builtin). If a change touches two features,
   split it into two PRs.
3. **Never push to `main`.** Work on a short-lived branch and open a PR. The maintainer
   reviews and merges.
4. **No unrelated changes in the same PR.** Specifically: never reformat, re-indent, or
   rewrite comments in code you are not otherwise changing. A repo-wide style rewrite
   bundled with a feature made an earlier change impossible to review; it had to be
   reverted. If you think reformatting is needed, propose it as its own PR and wait for
   approval.
5. **Every behaviour change ships with a test.** A PR that adds syntax also adds
   `tests/*.my` + `tests/*.expected` cases covering it, including at least one case for
   the way it fails.
6. **The build must stay warning-free** with `-Wall -Wextra -Werror`, and `make test`
   must be green before a PR is opened.
7. **Do not rewrite published history.** No force-pushes to `main`, no rebases of
   `main`, no rewriting tags. The tag `hw-v1` is the hand-written baseline and must never
   be moved or deleted.
8. **Stay inside the task.** Do not touch files, settings, CI, or repository metadata
   that the task does not require. Ask first if it seems necessary.

## Workflow

- Branch names: `agent/<topic>` (for example `agent/line-comments`).
- Commit messages: Conventional Commits style prefix + imperative subject, for example
  `feat: add // line comments`, `fix: handle division by zero`, `test: cover empty input`,
  `docs: document the print statement`.
- The commit body explains *what* and *why*, not *how*. Keep it short.
- AI-assisted commits end with the trailer:

  ```
  Assisted-by: AI
  ```

- PR description contains three sections:

  ```md
  ### What changed
  ### Verification   (exact commands and observed output)
  ### Known limitations
  ```

- Merge with **Rebase and merge** (or squash), so `main` stays free of merge commits.
- After a PR is merged, delete its branch.

## Build and test

```sh
make                                    # build -> build/run
make CFLAGS='-Wall -Wextra -Werror -g'  # strict build; run this before opening a PR
make test                               # run every test in tests/
make dev                                # build and run test.txt
make clean
```

## Tests

Each case is a set of files in `tests/`:

```
tests/<name>.my        the program to run
tests/<name>.expected  expected combined stdout + stderr
tests/<name>.exit      expected exit status (optional, default 0)
```

`tools/run-tests.sh [binary]` runs them all and diffs the output; `make test` wraps it.
Add cases for new syntax, and update an existing `.expected` file when a change
intentionally alters output (for example a better error message).

## Code style

Match the file you are editing; the codebase is not uniform. In practice:

- C99, compiled with `gcc`.
- 4-space indentation, `{` on the same line as the statement or function signature.
- English `//` comments, used sparingly.
- File-local helpers are `static`; prototypes for them sit at the top of the `.c` file;
  public functions are declared in the matching header with include guards.
- Token values are owned by the token and freed by the consumer; `env_set` copies its
  arguments (`strdup`), so callers keep ownership of what they pass in.
- Prefer the smallest change that fits the existing structure over a new abstraction.

## Architecture notes

- `lexer.c` keeps a static position and a global `current_token`; `lexer_reset()` resets
  the position before a new run.
- `lexer_get_pos()` / `lexer_set_pos()` let the parser re-read a section of the source.
  Loops use this to re-evaluate their condition on every iteration; that is why there is
  no AST.
- `parser.c` does parsing and execution in the same pass: `parse_*` functions either
  return a value (expressions) or perform a side effect (statements).
- `env.c` holds the variables: a static array of 256 `Var` entries, each either `VAR_INT`
  or `VAR_STRING`; integers are stored as strings and converted with `atoi`.
- Errors go through `syntax_error()`, which prints to stdout and calls `exit(1)`.
