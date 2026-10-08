# Wiki publication for issue #53

This directory stages the article requested by
[issue #53](https://github.com/Thunderspies/CityOfHeroes/issues/53). The
repository copy is reviewable source for the Wiki page, not a claim that the
page has been published. No game-source or baseline-data changes are included.

## Publish the article

1. In the CityOfHeroes Wiki, create or edit the page titled **Understanding
   Data Definitions and Parse Tables** using the contents of
   `Understanding-Data-Definitions-and-Parse-Tables.md`.
2. Add the link below to the existing Wiki Home page's developer-guide section.
   Create that section only if necessary, and preserve all other Home content.
3. Open the published page, verify its code blocks, tables, and source links,
   then follow the Home link to confirm navigation works.
4. Complete the Windows development-build and runtime checks described in the
   article, record the tested revisions/configuration and results on issue #53,
   and update the article's verification note accordingly. Close the issue only
   after publication and verification are complete.

Suggested Home entry, to add after the article is published:

```markdown
## Developer guides

- [Understanding Data Definitions and Parse Tables](https://github.com/Thunderspies/CityOfHeroes/wiki/Understanding-Data-Definitions-and-Parse-Tables): Trace Brawl's Range from a data file to C and add an optional field with explicit default behavior.
```

For command-line publication, this Markdown file belongs at the root of the
separate `CityOfHeroes.wiki.git` checkout. It does not belong in a `doc/wiki/`
subdirectory inside that Wiki checkout. Apply the Home addition to its existing
`Home.md` rather than replacing the file. See GitHub's
[Wiki editing instructions](https://docs.github.com/en/communities/documenting-your-project-with-wikis/adding-or-editing-wiki-pages).

## Verification status

The source walkthrough was checked against CityOfHeroes
`0b75ade0c801735e10c5798f641948a45cc50488` and i24
`088f20834e91e1b926344684b311140aa5f9b0e6`.

The article's C diagnostic block was compiled and executed in an isolated
harness for enabled, disabled, and development-mode cases. Markdown reference
consistency, fence balance, code formatting, and expected diagnostic strings
were checked locally. These checks do not execute the game's parser.

The actual Game/MapServer build, parser-default tests, text-versus-bin tests,
and live Brawl inspection have **not** been run. The article distinguishes
expected results from observed results. Direct Wiki publication is also still
pending; a source-repository pull request does not update the Wiki by itself.
