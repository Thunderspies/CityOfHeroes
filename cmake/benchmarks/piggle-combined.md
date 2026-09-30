Piggle master integration, 2026-10-03
====================================

The official CPM pin is eac2f235254c0dd1a5281c022b6faa287ca6643b,
including Piggle PRs #8, #9 and #10. Configuration removed the temporary
CPM_piggle_SOURCE override and fetched this exact upstream commit. No CoX
adapter or public API changes were needed. Client and MapServer builds,
all six CoX integration tests and the floating-point check passed in OptDebug.

The combined benchmark compares the previous pin, 4e74c9e, with the official
master library. Both executables use the same production CoX FileSystem.c,
UtilitiesLib and read workload. A test-only bridge exposes pg_file_read_all
to the benchmark without changing CoX's public interface.

Each mode has 12 alternating pairs, with one complete warmup in every child
before measurement. Each sample performs 2,000 selections and reads across
429 files in ../i24/data/sequencers. All passes matched 12,025,466 bytes and
checksum 849,366,704. Both libraries use Win32 MSVC /O2 /MT OptDebug, no IPO.

| Workload | Previous pin median | Master median | Paired median reduction |
| --- | ---: | ---: | ---: |
| Whole-file helper | 334.02 ms | 217.53 ms | 35.98% |
| Sized streamed reads | 289.95 ms | 220.57 ms | 24.41% |
| Native CRT control | 150.37 ms | 151.56 ms | 1.43% |

Master won all 12 pairs in both Piggle modes. The CRT control won seven
pairs, with its independent median slightly slower, showing no consistent
control improvement. Private and peak committed memory stayed around 2 MB;
the Piggle modes used 2–4 KiB less private memory at their medians.

These are warm-cache small-file read results, not whole-game startup or
archive-throughput measurements. Existing loose-file CRT paths stay as they
are. Raw samples, exact revisions, executable/library/source hashes and
memory measurements are in piggle-combined-results.json.

Local reproduction artifacts remain in the primary CoX checkout under
out/piggle-pr-bench: read-bench.c, FileSystem-bench.c, build.ps1 and run.ps1.
After the official CoX build, copy its OptDebug piggle.lib to
combined-candidate.lib and the preserved issue4-parent.lib to
combined-parent.lib. Run build.ps1 -Issue combined, then run.ps1 with
-Issue combined -Parent 4e74c9ec0a631f5e454746f5589a10d8a93aa478
-Candidate eac2f235254c0dd1a5281c022b6faa287ca6643b -Pairs 12.
The scripts retain the original PR benchmark's local paths and link flags.
