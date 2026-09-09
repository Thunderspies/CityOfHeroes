# StructParser

StructParser generates parser tables, command wrappers, tests, transactions,
late links, and autorun registration from a target's source files. It requires
a C++17 compiler and CMake 3.24 or newer, with no external library dependencies.

## Build on Linux

From the repository root:

```sh
cmake -S Utilities/StructParser -B out/build/structparser -G Ninja \
	-DCMAKE_BUILD_TYPE=Release
cmake --build out/build/structparser
```

The executable is `out/build/structparser/StructParser`. To use Clang, add
`-DCMAKE_CXX_COMPILER=clang++` when configuring a separate build directory.

`Unix Makefiles` and `Ninja Multi-Config` are also supported. With
`Ninja Multi-Config`, select the configuration at build time:

```sh
cmake -S Utilities/StructParser -B out/build/structparser-multi \
	-G "Ninja Multi-Config"
cmake --build out/build/structparser-multi --config Release
```

The executable is then `out/build/structparser-multi/Release/StructParser`.

## Cross-compile Windows targets on Linux

The repository provides MinGW presets:

```sh
cmake --preset mingw
cmake --build --preset mingw-debug
cmake --build --preset mingw-release
```

The MinGW toolchain compiles consumers for Windows. StructParser runs as a
native Linux executable built in `host-tools/StructParser`. Its separate CMake
build uses Release mode and a host compiler, independently of target toolchain
settings and compiler environment flags. Wine is not required.

Set `STRUCTPARSER_HOST_CXX_COMPILER` to choose the host compiler:

```sh
cmake --preset mingw \
	-DSTRUCTPARSER_HOST_CXX_COMPILER=/usr/bin/clang++
```

Ninja parent builds, including Multi-Config, use single-config Ninja for the
host tool. Makefiles parent builds use Unix Makefiles. Each parent build checks
StructParser's source dependencies before running generation.

## CMake integration

Add this directory once with `add_subdirectory`, then include
`cmake/StructParser.cmake`. Call `StructParse(target)` after assigning the
consumer's sources. The consumer's source directory must contain `src/`, and
the top-level source directory must contain `Common/`.

`StructParser` is the build target for the tool. `StructParser::Host` identifies
the executable to invoke on the build host. The integration orders tool
compilation, generation, and consumer compilation. Generated sources listed on
the consumer are declared as byproducts. Source entries containing `autogen`
in their names or paths are reserved for generated outputs, case-insensitively.

The source manifest and private state are stored in the consumer's build
directory, separately for each configuration. Generated files are written to
the target's `src/AutoGen` and the shared `Common/AutoGen` directories.
Configurations that share these output directories must be built sequentially.
Concurrent independent builds sharing generated output directories are
unsupported.

## Command line

```sh
StructParser --target Example --source-dir Example/src --common-dir Common \
	--project-dir Example --kind executable --sources-file sources.txt \
	--state-dir build/Example/Release/StructParser
```

All seven value options are required:

| Option | Meaning |
| --- | --- |
| `--target` | Target name, expressed as a C identifier. |
| `--source-dir` | Target source directory containing `AutoGen/`. |
| `--common-dir` | Shared source directory containing `AutoGen/`. |
| `--project-dir` | Starting directory for configuration discovery. |
| `--kind` | `executable` or `library`. |
| `--sources-file` | File containing the ordered source manifest. |
| `--state-dir` | Private state directory for one target and configuration. |

Directory and manifest paths may be absolute or relative to the working
directory. Source, common, and project directories must exist. The state
directory is created as needed.

The source manifest contains one unquoted path per line, in scan order. LF and
CRLF are accepted, blank lines ignored, and duplicate paths scanned once.
Relative source paths resolve against the working directory. An empty manifest
is valid.

The parser scans `.c` and `.h` files, matching extensions case-insensitively.
It excludes paths containing `autogen`, case-insensitively, files named
`stdtypes.h`, case-insensitively, and paths containing the text `Program Files`.

Optional flags:

- `--force` regenerates the target regardless of recorded state.
- `--verbose` prints scan and skip information.
- `--help` prints usage.

Errors return a nonzero exit status and stop dependent builds.

## Configuration

The parser searches for `StructParserVars.txt` starting in `--project-dir` and
walking toward the filesystem root, using the nearest match. The configuration
language supports variable assignments and file includes:

```text
Name = Value, OtherValue;
#include "settings/extra.txt"
```

Relative includes resolve from the including file's directory. Both slash
styles are accepted, and includes may nest up to 64 levels.

## Regeneration and output ownership

StructParser skips generation when its recorded state matches the invocation,
inputs, configuration, executable, and generated outputs. A private versioned
manifest records paths, sizes, and native filesystem timestamps, including
absent configuration discovery candidates. Changes, missing files, or invalid
state trigger complete regeneration. Editing source contents while preserving
both size and timestamp requires `--force`.

Each generation builds parser data and the identifier dictionary in memory.
Outputs are staged in temporary files, compared with their destinations, and
replaced only when their contents differ. Unchanged files keep their timestamps.
The build system manages recompilation from these generated files.

Successful state is invalidated before generation and published after output
replacement and cleanup succeed. An ownership journal records output paths
across failures so the next invocation can retry cleanup.

On first use, StructParser takes ownership of files directly inside the
target's `src/AutoGen` and `src/wiki` directories, plus files beginning with
`<target>_` directly inside `Common/AutoGen`. These directories are not scanned
recursively for cleanup. Subsequent cleanup removes only recorded outputs that
are no longer generated, after parsing succeeds.
