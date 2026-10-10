This monorepo contains the code for the client, developer utilities, and
several server-side services for the Cryptic engine that were used for City of
Heroes up until the game was cancelled in 2012. This is a fork of the Ourodev's
repo that was intended for porting to the latest Visual C compiler, but it also
has some minor feature additions.

[![Join our chat on the Thunderspy Discord](https://img.shields.io/badge/Join%20our%20chat-Thunderspy%20Discord-5865F2?logo=discord&logoColor=white)](https://discord.gg/eNPY22FbaB)

# Building

Currently, only x86 (32-bit) builds are supported in Visual Studio 2026 or
MSBuild. Release builds are for distributing publicly to players, and OptDebug
builds enable developer mode for creating game content or map editing.

From the repository root in a Visual Studio Developer PowerShell or command
prompt, configure once and build either configuration. This project provides
CMake presets for Release and OptDebug.

```text
cmake --preset vs2026
cmake --build --preset vs2026-optdebug
cmake --build --preset vs2026-release
```

The executables will be in the out/ folder.

## Continuous integration

CMake CI automatically builds and tests OptDebug for pull requests. Changes
limited to Markdown, `doc/` or `docs/` directories, `data/`, `Assets/DBSchemas/`,
or the root `.gitignore` skip the Windows build. A lightweight check still
reports the result so documentation and runtime-only pull requests can merge.
New PR updates cancel older CI runs for that PR.

Branch pushes do not run CMake CI. Use the workflow's **Run workflow** button
for a full manual build. Pushing a `v*` tag still prepares an OptDebug release.

# Usage

The [data repo](https://github.com/Thunderspies/i24) has all the other
resources and instructions needed for running a local test and development
server. Check that out for a quick start. The instructions here should explain
in more detail than is necessary to get strted.

For local testing, at the minimum, the DBServer, one MapServer, and the Game
client need to run together. MapServer and Game won't run without all of the
game data in pigg archive files or in the data folder. The DBServer needs the
minimal [servers.cfg](data/server/db/servers.cfg) config file to start it with
"fake auth" mode that accepts a user without a password.

This project has a server configuration to use SQLite, which is only suitable
for a single user to test locally. To host a server for multiple players,
installing SQL Server and ODBC 17 is recommended, but that's outside the scope
of this quickstart guide.

Before starting the DBServer for the first time, or whenever the data or code
is changed in some instances, templates must be generated, so it can create its
own database schema.
```batch
start MapServer.exe -templates
```

If templates are already generated, start the DBServer
```batch
start DBserver.exe
```

Then, start the MapServer to connect to the DBServer
```batch
start MapServer.exe -db 127.0.0.1 -map_id 1
```

Finally, start the game client to use the localmapserver.
```batch
start CityOfHeroes.exe -db 127.0.0.1 -localmapserver 1 -notimeout 1 -noaudio 1
```

This will enable logging in with a fake account and any password. Creating or
selecting any character will send them straight to Atlas Park (map ID 1).

For a headless smoke test, build `TestClient` and run it from the data repository:

```text
TestClient.exe -db 127.0.0.1 -server 127.0.0.1 -localmapserver -fakeauth -authname SmokeTest -dontpause -hideconsole -nosharedmemory -nolevel -disconnect
```

`-fakeauth` skips TestClient's AccountServer slot-redemption requirement for
character creation. DBServer must have `UseFakeAuth 1`; the switch requires
`-db` (or `-cs`) and cannot be combined with `-auth`. `-disconnect` exits after
connecting to the map; omit it to keep the test client connected. Run unattended
tests with an external timeout.
