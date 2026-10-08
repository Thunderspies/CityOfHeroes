# Understanding Data Definitions and Parse Tables

A game-data field has three separate parts: a name written in a data file,
metadata that maps that name into a C structure, and code that gives the stored
value meaning. Adding only one of these parts does not add a working feature.

This guide follows **Brawl's `Range`** from its `.powers` file to a combat
calculation. It then adds an optional diagnostic flag to the same definition.
The flag makes the parser's behavior visible without changing combat rules.

**Prerequisites:** basic C structures and pointers, a source checkout, and a
local development installation. Follow the [source setup][source-readme] and
[data setup][data-readme] first. No prior knowledge of powers, combat evals, or
AttribMods is required.

**Source baseline:** reviewed against CityOfHeroes commit
`0b75ade0c801735e10c5798f641948a45cc50488` and i24 commit
`088f20834e91e1b926344684b311140aa5f9b0e6`, on October 8, 2026. Source links are
pinned to those revisions. The walkthrough's game-build and runtime checks
below are a verification procedure, not a claim that those checks have already
been executed.

## 1. Find the data and its parser

There are two repositories involved:

| Repository | Relevant contents |
| --- | --- |
| `Thunderspies/CityOfHeroes` | C structures, parsers, loaders, client and server implementations. |
| `Thunderspies/i24` | Plain-text game definitions and local setup scripts. |

The i24 repository is a baseline archive. Its contribution instructions ask
contributors to make their changes in a fork rather than submit gameplay
changes to the archive. Use a disposable local branch or fork for this
exercise. Binary assets needed to run the game are obtained separately through
its setup instructions. See the [data README][data-readme].

Open [`data/defs/powers/inherent_inherent.powers`][brawl] in the data checkout.
The `Power Inherent.Inherent.Brawl` block includes `Range 7`. It also contains
`Type kClick`, `AttackTypes`, and several nested `AttribMod` blocks. Keep the
complete existing definition; these individual lines are not standalone power
files.

From the **source** checkout, find the matching text field:

```sh
git grep -n -F '"Range"' -- Common/entity/powers_load.c
```

In [`ParseBasePower`][power-loader], the matching entry is:

```c
{ "Range", TOK_F32(BasePower, fRange, 0) },
```

That entry connects the spelling `Range` to the `fRange` member of `BasePower`.
It is not an instruction to call a function named `Range`.

**Search within the right structure.** Field names are local to a parse table.
For example, Brawl has a `Type` at power level and a `Type` inside an
`AttribMod`; they are interpreted by different tables. A matching word
elsewhere in the repository is not necessarily the field you are extending.
See [`ParseBasePower`][power-loader] and
[`ParseAttribModTemplate`][attribmod-header].

## 2. Read a parse-table entry

The declarations in this code use `TokenizerParseInfo` and the parser APIs use
`ParseTable`. Follow the existing table and its `TOK_*` macros rather than
writing raw field offsets yourself. The public [text parser header][parser]
describes those macros and their storage rules.

For the `Range` entry:

| Part | Meaning |
| --- | --- |
| `"Range"` | Token accepted inside this power definition. |
| `TOK_F32` | Read a floating-point value. |
| `BasePower` | Structure being populated. |
| `fRange` | Destination member within that structure. |
| `0` | Parser default when this field is omitted from a newly initialized definition. |

The macro expands into several parse-table fields, including a token type, a
member offset computed with `offsetof`, and the default. Its checks also catch
some mismatches between token storage and member size. It does **not** validate
all gameplay rules for the value. The declaration of `BasePower.fRange`
documents its units as feet. See [the macro][parser] and
[the member declaration][powers].

For this existing field, the expected parsed values are:

| Data in the Brawl block | `BasePower.fRange` after parsing |
| --- | --- |
| `Range 7` | `7.0f` |
| `Range 12` | `12.0f` |
| No `Range` line | `0.0f` |

These are **stored base values**, not guarantees about the final distance at
which an attack succeeds. Other combat processing still applies.

Defaults belong to parser initialization, not to the C language. A raw
`malloc()` does not apply parse-table defaults, and parsing into an existing
object should not be treated as a guaranteed reset. Use the established loader
or parser construction functions such as `StructCreate` when constructing
parser-managed data. Match them with the corresponding parser cleanup API.
See [structure memory handling][parser].

One implementation detail worth knowing early: at this revision, `TOK_F32`'s
macro default must be integer-valued, even though a value read from text can
have a fractional part. Do not copy an arbitrary fractional C initializer into
that macro's default argument; inspect the macro and use an appropriate
initialization or processing step when necessary. See [`TOK_F32`][parser].

## 3. Named values are explicit mappings

Brawl's `Type kClick` is interpreted through this entry in
[`ParseBasePower`][power-loader]:

```c
{ "Type", TOK_INT(BasePower, eType, kPowerType_Click), PowerTypeEnum },
```

`PowerTypeEnum` is a `StaticDefineInt` table. Its `kClick` entry maps the text to
`kPowerType_Click`. The `DEFINE_INT` and `DEFINE_END` markers describe and
terminate such a table; they are not lines that belong in the data file.
See [the definition API][defines] and [the power mappings][power-loader].

This has two practical consequences. A new C enum member does not become a
recognized data-file spelling by itself. Conversely, recognizing a spelling
does not implement new behavior for its value. The code that consumes the
value must support it too.

There are also dynamically assembled mappings. In
[`load_def.c`][def-loader], a `DefineContext` is built from `s_PowerEnums` and
additional attribute, animation, and data-defined names. `ParsePowerDefines`
wraps that context for use by parse tables. This is application-level name
resolution, not the C preprocessor's `#define` mechanism.

For instance, Brawl's `StrengthsDisallowed kRange` uses the attribute-name
mapping. That `kRange` identifies a `CharacterAttributes` offset. It is **not**
the literal number in Brawl's `Range` line and does not name the
`BasePower.fRange` member. This distinction becomes important when reading
AttribMods. See [`AddAttributeDefines`][def-loader],
[`ParseBasePower`][power-loader], and [the Brawl definition][brawl].

## 4. Lists and nested blocks use different storage

You only need a few more table patterns to navigate a power definition:

| Pattern | Example | Storage to expect |
| --- | --- | --- |
| `TOK_STRING` | A power's `Name` | A parser-managed string. |
| `TOK_INTARRAY` | `AttackTypes` | An expandable array of integer values, possibly resolved through a named-value table. |
| `TOK_STRUCT` | Repeated `AttribMod` blocks | An expandable array of pointers to child structures, each parsed using its child table. |

For example, the power table connects the text `AttribMod` to
`BasePower.ppTemplates` and `ParseAttribModTemplate`. Each block is parsed as
an `AttribModTemplate`; it does not create an active combat effect at file-load
time. The runtime effect is a separate object. See
[the power table][power-loader] and [the template declaration][attribmod-header].

These are EArrays, not ordinary fixed-size C arrays. For a pointer EArray such
as `ppTemplates`, use the established `eaSize(&object->ppTemplates)` convention
to obtain its count. Integer EArrays have the corresponding `eai*` APIs. Do not
infer the count with `sizeof(pointer)` or independently free child objects owned
by a parsed definition. See [EArray operations][earray] and
[parser ownership functions][parser].

Not every nested member is a pointer EArray: `TOK_EMBEDDEDSTRUCT` and
`TOK_OPTIONALSTRUCT` describe other arrangements. Match the declaration and
macro in front of you rather than assuming every block uses `TOK_STRUCT`.

## 5. Follow `Range` through loading and use

The source path for this example is:

```text
Brawl's .powers text
    -> ParsePowerDictionary / ParseBasePower
    -> BasePower.fRange
    -> loaded PowerDictionary
    -> a character's Power.ppowBase
    -> character_PowerRange()
```

Start in [`load_AllDefs()`][def-loader]. It creates the mappings needed by
powers and calls `load_PowerDictionary()` for the logical `defs/powers/` path.
The on-disk data checkout contains this under `data/defs/powers/`; use the
working-directory and asset setup from the data README.

[`load_PowerDictionary()`][power-loader] loads categories, power sets, and
powers. Its powers call uses the `.powers` suffix, `powers.bin`,
`ParsePowerDictionary`, and `load_PreprocPowerDictionary`. The dictionary table
maps each `Power` record to a `BasePower` using `ParseBasePower`.

The `FullName` entry uses `TOK_STRUCTPARAM`, which explains why the identity
appears after `Power` in `Power Inherent.Inherent.Brawl`. It is distinct from
the `Name "Brawl"` field inside the block. Keep that identity unchanged during
this exercise. See [the dictionary and power tables][power-loader].

Loading includes more than text conversion. Power processing performs fixups
and validation, builds lookups, connects categories and sets to powers, and
sets up back-pointers. Some values are therefore derived or adjusted after
parsing. To find a loaded definition, use the existing dictionary API, such as
`powerdict_GetBasePowerByFullName`, rather than building a separate search
mechanism. See [the loader][power-loader] and [lookup declarations][powers].

`BasePower` is shared definition data. `Power` holds character-specific state
and refers to its definition through `ppowBase`. Do not modify a loaded
`BasePower` to give a single character a different effect, and do not treat its
pointers as persistent identities. Shared storage and reload processing also
matter to pointer lifetime. See [the two structures][powers] and
[power loading/reloading][power-loader].

For a runtime consumer, find `character_PowerRange()` in
[`MapServer/src/entity/character_combat.c`][combat]. Its base calculation
multiplies `ppow->ppowBase->fRange` by `ppow->pattrStrength->fRange`.
`Common/entity/PowerInfo.c` also calls that helper when updating power range
information. This is where the parsed base value starts contributing to
runtime behavior. See [the combat implementation][combat] and
[power information updates][power-info].

Useful searches from the source checkout are:

```sh
git grep -n -F fRange -- Common/entity/powers.h MapServer/src/entity
git grep -n -F character_PowerRange -- Common Game MapServer
git grep -n -e load_AllDefs -e load_PowerDictionary -- Common/entity
```

Do not replace the runtime range calculation as part of this parser exercise.
Changing how range affects all callers is a separate gameplay task.

## 6. Understand caches before testing changes

The loader can obtain powers from shared memory or load them from text/binary
representations. `ParserLoadFiles` supports detecting changed text or parse
metadata and rebuilding its binary representation. That does not mean every
process reparses your edited file on every launch. See
[parser loading contracts][parser] and [power loading][power-loader].

Keep these distinctions in mind:

- `.powers` is source data; `powers.bin` is generated data. Edit the former.
- A new member or token is a **code change**. Rebuild the affected executables;
  a data reload cannot add fields to an already running binary.
- Matching client and server definitions matter. A member in `Common` is not
  automatically replicated, persisted to the character database, or needed by
  every executable. Follow each loader and consumer.

For a controlled first test, stop the local processes, rebuild the development
executables when needed, and launch MapServer with `-nosharedmemory` in addition
to the normal local startup arguments. Its startup output confirms that shared
memory is disabled. Regenerate bins using the
[data repository's documented procedure][data-readme] when needed, rather than
editing binary files. See [MapServer argument handling][server-init].

A restart is the baseline procedure for this guide. Do not use `/defsreload`
as a universal substitute: its handler requires development mode and disabled
shared memory, and the power reload implementation contains its own matching
and copying rules. In particular, new members are not automatically added to
manual field-by-field copies. See [the command handler][server-commands] and
[`ReloadPowers`][power-loader]. Advanced reload behavior is a later topic.

## 7. Exercise: add an optional diagnostic field

First make a data-only edit: change **only Brawl's** `Range 7` to `Range 12`,
restart the local development process, and inspect `fRange` in the loaded
`BasePower`. Restore `Range 7` afterward. This uses an existing field and needs
no new C member or parse-table entry.

Now add `DebugPrintRange`, a **new tutorial-only field**. It is not present in
the baseline and cannot be used with unmodified executables. Its contract is:
when true, print this definition's base range during development-mode power
finalization; when omitted or false, do nothing. It changes no combat values.

Work in a disposable source branch and a disposable data branch. These sample
changes are an exercise, not a proposed permanent gameplay feature. Follow
[`CONTRIBUTING.md`][contributing] for new code.

### A. Add the member

In `BasePower` in [`Common/entity/powers.h`][powers], near `fRange`, add:

```c
/* Print the base range during development-mode finalization.
 * Defaults to false and does not alter combat behavior.
 */
bool bDebugPrintRange;
```

This belongs to the shared definition, not to the runtime `Power` and not to
`CharacterAttributes`. Do not change the attribute layout for this exercise.

### B. Add the accepted text field

In `ParseBasePower` in
[`Common/entity/powers_load.c`][power-loader], near the existing `Range` entry,
add this entry before the table terminator:

```c
{ "DebugPrintRange", TOK_BOOL(BasePower, bDebugPrintRange, 0), BoolEnum },
```

`BoolEnum` already maps `kTrue`/`true` and `kFalse`/`false` to their respective
values. The `0` preserves existing behavior for every definition that omits the
new field. Keep the member type consistent with `TOK_BOOL`; do not substitute an
`int` or a C bit-field without choosing an appropriate representation.
See [the boolean macro][parser] and [the existing mapping][power-loader].

### C. Add a consumer at the appropriate loading stage

In the same file, find `powerdict_FinalizePower()`. Insert the following block
**after its initial invalid-parent guard**, before its subsequent power-link
checks:

```c
if (isDevelopmentMode() && ppow->bDebugPrintRange) {
	printf("Power %s: base range %.2f feet\n",
		ppow->pchFullName, ppow->fRange);
}
```

Use finalization rather than the text-to-bin preprocessor for this diagnostic.
The startup path calls `powerdict_FinalizePower()` after materializing the
dictionary from either text or bins. The preprocessor is only run while
building the binary representation. Loading directly from an existing shared
memory image can bypass this work, which is why the test configuration disables
shared memory. See [`load_PowerDictionary_MoveToShared`][power-loader] and
[the parser callback contracts][parser].

This output is the field's entire behavior. Registering the field alone would
only store a boolean. The format string is fixed; the power name is passed as
data, not used as a format string. Finalization can occur more than once over
a development session, so the message is not a once-per-activation event.

### D. Use the field in Brawl

Inside the existing `Power Inherent.Inherent.Brawl` block in
[`inherent_inherent.powers`][brawl], retain `Range 7` and add:

```text
DebugPrintRange kTrue
```

Do not add it inside an `AttribMod` block or apply it to every power in the
file. Rebuild both development Game and MapServer executables against the
changed common definition/parser, and use the matching data. Test the diagnostic
in MapServer with shared memory disabled. This exercise does not require a new
packet, SQL field, or database migration.

## 8. Verify explicit values and defaults

Use a debugger breakpoint after the new diagnostic block in
`powerdict_FinalizePower()` and select the invocation whose `pchFullName` is
`Inherent.Inherent.Brawl`. Inspect `fRange` and `bDebugPrintRange`. Other powers
also pass through this function, so hitting the breakpoint is not sufficient
proof that you are inspecting Brawl.

Restart for each case. The expected results are:

| Brawl data | Expected stored values | Expected diagnostic |
| --- | --- | --- |
| `Range 7`; no `DebugPrintRange` | `7.0f`, false | No message from the new block. |
| `Range 7`; `DebugPrintRange kFalse` | `7.0f`, false | No message from the new block. |
| `Range 7`; `DebugPrintRange kTrue` | `7.0f`, true | `Power Inherent.Inherent.Brawl: base range 7.00 feet` |
| `Range 12`; `DebugPrintRange kTrue` | `12.0f`, true | Same diagnostic with `12.00 feet`. |
| No `Range`; `DebugPrintRange kTrue` | `0.0f`, true | Same diagnostic with `0.00 feet`. |

Use the missing-`Range` case to inspect initialization, not as a playable power
configuration. Restore the original `Range 7` when finished.

For the enabled case, also compare a run that regenerates the relevant bins
with a second run that reuses the resulting bins, keeping shared memory
disabled. Inspect the loader path or debugger to confirm which representation
was used. The stored values and diagnostic should agree. A cached run should
not depend on a preprocessor-only side effect.

Finally, try a deliberately misspelled **field name**, such as
`DebugPrintRnage`, in the disposable data copy. Confirm that the text parse
reports the unknown field. Treat the diagnostic as a failed input even if the
loader continues; do not assume all parser errors terminate startup. Restore
the valid spelling before further tests. Unknown-field handling and
invalid-value handling are different things; the presence of a named-value
mapping is not a substitute for checking value-validation behavior.

For the original range trace, inspect a live Brawl `Power` at
`character_PowerRange()` and compare the definition's `fRange` with the runtime
strength multiplier. Do not infer correctness only from whether an enemy can
be hit: that would mix parser verification with targeting and combat rules.

## 9. Troubleshooting and next steps

| Symptom | First checks |
| --- | --- |
| New field is unknown | Is the rebuilt executable running? Is the entry in `ParseBasePower`, and is the field in the outer `Power` block? |
| Stored value is still old | Are you editing the active data checkout? Did the process take a shared-memory or cached path? |
| Flag is true but nothing prints | Is development mode active? Did execution pass through this finalizer? Is stdout being captured? |
| Default differs from expectations | Was the structure initialized through the parser, or was an existing object being reused? Does later processing change the value? |
| Server and client disagree | Do their binaries, data revisions, and generated representations match? Which side actually consumes the field? |
| Gameplay range differs from base range | Inspect the runtime calculation and its callers rather than changing the parser. |

After the exercise, revert its code and data changes and rebuild/regenerate the
local representations as needed. Do not distribute data containing the new
token to an executable that does not recognize it.

You can now trace **text -> table -> structure -> loader -> consumer** and add
an optional field without confusing parsing with behavior. The next guides can
build on this workflow to explain definition lifetime, power instances,
attributes and aspects, eval contexts, and the AttribMod lifecycle. Until those
pages exist, the source links here provide the implementation reference.

[source-readme]: https://github.com/Thunderspies/CityOfHeroes/blob/0b75ade0c801735e10c5798f641948a45cc50488/README.md
[data-readme]: https://github.com/Thunderspies/i24/blob/088f20834e91e1b926344684b311140aa5f9b0e6/README.md
[brawl]: https://github.com/Thunderspies/i24/blob/088f20834e91e1b926344684b311140aa5f9b0e6/data/defs/powers/inherent_inherent.powers
[parser]: https://github.com/Thunderspies/CityOfHeroes/blob/0b75ade0c801735e10c5798f641948a45cc50488/libs/UtilitiesLib/include/utilitieslib/utils/textparser.h
[defines]: https://github.com/Thunderspies/CityOfHeroes/blob/0b75ade0c801735e10c5798f641948a45cc50488/libs/UtilitiesLib/include/utilitieslib/utils/structDefines.h
[earray]: https://github.com/Thunderspies/CityOfHeroes/blob/0b75ade0c801735e10c5798f641948a45cc50488/libs/UtilitiesLib/include/utilitieslib/components/earray.h
[powers]: https://github.com/Thunderspies/CityOfHeroes/blob/0b75ade0c801735e10c5798f641948a45cc50488/Common/entity/powers.h
[power-loader]: https://github.com/Thunderspies/CityOfHeroes/blob/0b75ade0c801735e10c5798f641948a45cc50488/Common/entity/powers_load.c
[def-loader]: https://github.com/Thunderspies/CityOfHeroes/blob/0b75ade0c801735e10c5798f641948a45cc50488/Common/entity/load_def.c
[attribmod-header]: https://github.com/Thunderspies/CityOfHeroes/blob/0b75ade0c801735e10c5798f641948a45cc50488/Common/entity/attribmod.h
[combat]: https://github.com/Thunderspies/CityOfHeroes/blob/0b75ade0c801735e10c5798f641948a45cc50488/MapServer/src/entity/character_combat.c
[power-info]: https://github.com/Thunderspies/CityOfHeroes/blob/0b75ade0c801735e10c5798f641948a45cc50488/Common/entity/PowerInfo.c
[server-init]: https://github.com/Thunderspies/CityOfHeroes/blob/0b75ade0c801735e10c5798f641948a45cc50488/MapServer/src/svr/svr_init.c
[server-commands]: https://github.com/Thunderspies/CityOfHeroes/blob/0b75ade0c801735e10c5798f641948a45cc50488/MapServer/src/cmdparse/cmdserver.c
[contributing]: https://github.com/Thunderspies/CityOfHeroes/blob/0b75ade0c801735e10c5798f641948a45cc50488/CONTRIBUTING.md
