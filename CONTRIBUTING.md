Contributions will be rejected unless they meet the following criteria.

# Code Style

Follow the [Linux kernel coding style][style] with these extra restrictions.

- Keep line width 80 characters or under
- Declare local variables close to where they're first used
- Only indent with tabs and only use tabs for indenting
- Any number of spaces may be used for alignment/readability in these cases:
  - Never at the start of a line
  - Never for indenting

Since this is inherited code, only new code needs to follow this style guide.

[style]: https://docs.kernel.org/process/coding-style.html

# Documentation

Comments should document interfaces, contracts, assumptions, and non-obvious
behavior. Do not add comments that merely restate what the code already makes
clear.

Documentation is required for:

* Functions, types, constants, macros, and other interfaces declared in public
  headers
* Global state and interfaces that expose or modify global state

Function documentation should describe, where applicable:

* The function's purpose and intended usage
* Each parameter's direction:
  * Input
  * Input/output
  * Output
* How input/output and output parameters are modified
* Whether pointer parameters may be `NULL`
* Accepted input values, ranges, and other preconditions
* Return value semantics, including possible values or ranges
* Error-reporting mechanisms that callers are expected to check, such as
  `errno` or module-specific error state
* Ownership and lifetime requirements for memory and other resources
  * Whether ownership is transferred
  * Whether the caller or module is responsible for releasing a resource
  * How long returned pointers or handles remain valid
* Intended side effects, including:
  * Global state that influences the function
  * Global state that the function modifies
  * Files, sockets, processes, or other external resources that are opened,
    closed, created, modified, or removed
* Thread-safety or reentrancy requirements when relevant

File-local (`static`) functions, objects, and types do not require
documentation when their purpose and behavior are clear from the
implementation. Local variables do not require comments unless their purpose,
units, invariants, or other behavior would otherwise be unclear.

# Commits

Keep changes in commits as complete and concise as possible. Do not attempt to
fix two things at once, but don't itemize tasks and goals into individual
commits that are so small that any one commit would be pointless in isolation.
A commit should be treated as a product just as much as the code, because it
will be used by a maintainer to apply patches to other forks or revert changes
that introduce errors.

## Titles

A maximum of 72 characters is allowed for commit titles. Use imperative-tense
for titles so that the following statement makes sense when the title is used
to fill in the blank:

> The purpose of this commit is to BLANK

For instance, a commit title of "fix a segfault in the text formatter" still
makes sense when used in the sentence "The purpose of this commit is to *fix a
segfault in the text formatter*".

## Descriptions

The rest of the commit message should have a body explaining the following:

- The purpose of the commit
- High level implementation details
- Any known issues, limitations, or follow-up work introduced by this commit

## Fixups

If a commit is intended to fix an issue or complete follow-up work in another
commit, then submit it as a fixup commit using `--fixup` on the other commit.

# Dependencies

This project uses CPM in CMake to pull dependencies. If a dependency is needed,
add it with `CPMAddPackage`. Dependencies may be patched only so that they can
build with CMake and CPM. Use the `PATCHES` parameter in `CPMAddPackage` to
apply patches. Do not patch a dependency to port it to a specific compiler or
link with another library for instance.
