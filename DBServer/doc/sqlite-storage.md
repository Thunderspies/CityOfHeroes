# SQLite for local development

SQLite is intended for one developer running DBServer, one MapServer, and a
client with fake authentication. It is bundled into DBServer. SQL Server and
PostgreSQL remain selectable; SQLite is not intended for production shards.

Build the Win32 developer executables using the root README. Place DBServer,
MapServer, TestClient or the game client, and their runtime DLLs in a complete
game data checkout. Generate templates with `MapServer.exe -templates` before
starting DBServer, and regenerate them after changing container definitions.
Copy this repository's `data/server/db/servers.cfg` to that checkout.

The local configuration contains:

```text
SqlDbProvider sqlite
SqlDbName "db/local/coh.sqlite"
SqlAllowDDL 1
SqlAddAttributes 1
SqlAddColumnOrTable 1
SqlDeleteColumnOrTable 0
SqlRebuildTable 0
SqlAlterColumn 0
UseFakeAuth 1
```

`SqlDbName` is a file path relative to the server working directory, or an
absolute path. DBServer creates its parent directories and initializes tables
and attributes from the generated templates. `SqlLogin` and `SqlInit` are unused
by SQLite. Start DBServer and MapServer normally, then connect the client using
`-db 127.0.0.1 -localmapserver` and any fakeauth account.
With TestClient, also pass `-fakeauth -server 127.0.0.1 -authname <account>`;
its DBServer and local MapServer addresses are separate settings.

SQLite uses a single connection on the DBServer main thread. Queued requests
retain their order and deliver callbacks after execution. Foreground reads
drain preceding work. Container updates, whole-container deletions, native
batches, and startup schema/cleanup are transactional. WAL, foreign keys, and
full durability are enabled. A busy writer waits at most five seconds per
operation, then returns a clear lock error rather than retrying indefinitely.

New tables and trailing nullable columns can be added. Removed, reordered,
renamed, or changed columns, attribute remapping, and changed existing foreign
keys require a reset. DBServer reports the incompatible table and leaves the
startup transaction uncommitted. To reset this disposable database, stop both
servers, then remove `coh.sqlite`, `coh.sqlite-wal`, and `coh.sqlite-shm` from the
configured directory. The next startup creates a fresh database; all saved
characters in the removed file are lost. Keep WAL sidecars with the database
when copying it while running; prefer copying only after shutdown.

Container persistence and the ordinary login/MapServer column requests are
supported, including DISTINCT, TOP/LIMIT, ordering, joins, and null predicates.
The adapter translates bare `dbo.` qualifiers and `ISNULL()` tokens while
preserving quoted text and comments. Native admin SQL must use SQLite syntax;
SQL Server procedures, DBCC commands, schema rebuild tools, and full offline
administration are outside this local backend's scope. Unsupported SQL reports
an error and its SQL text. Explicit transaction-control SQL is rejected;
DBServer owns transaction boundaries.

## Verification

Configure with `-DCOX_BUILD_DBSERVER_TESTS=ON`, build DBServer and the three
test executables, and run CTest. `DatabaseSqliteTests` uses real temporary files
to check values, locking, rollback, foreign keys, additive schemas, and restart
identity allocation. `ContainerSqliteTests` runs the production container
serializer and FIFO inside DBServer, including deferred reads after writes.

For the full login/create/save/restart test, build TestClient and MapServer,
provide complete game data with generated templates, and run:

```text
python DBServer/tests/sqlite_local_smoke.py --bin out/build/vs2026/bin/OptDebug --data-root D:/game/i24 --output out/sqlite-runtime
```

The script copies the game data into a unique fixture, creates a hero through
TestClient, changes influence through MapServer, saves it, stops both servers,
restarts them, and resumes the same hero. It checks identity, influence, child
rows, and foreign-key integrity and writes `result.json` only on success. Its
servers are stopped on success or failure; logs and the fixture remain for
inspection. Run with no other local shard listening on the standard ports.
For another run, `--reuse-fixture <printed-fixture-path>` skips the asset copy
and resets that fixture's disposable database and reports.
This test requires game assets and is opt-in: set `COX_SQLITE_GAME_DATA_ROOT`
when configuring to register it in CTest. The hosted workflow runs the storage
and container tests without game assets.

To retain SQL Server, restore the original settings and enable the DDL operations
appropriate to your existing schema:

```text
SqlDbProvider mssql
SqlDbName cohdb
SqlInit "create database cohdb;"
SqlLogin "Driver={ODBC Driver 17 for SQL Server};Server=(localdb)\mssqllocaldb;Persist Security Info=False;Trusted_Connection=yes;"
```

SQLite files and SQL Server databases are separate stores; changing providers
does not migrate saved characters.
