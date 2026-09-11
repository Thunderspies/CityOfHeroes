This monorepo contains the code for the client, developer utilities, and
several server-side services for the Cryptic engine that were used for City of
Heroes up until the game was cancelled in 2012. This is a fork of the Ourodev's
repo that was intended for porting to the latest Visual C compiler, but it also
has some minor feature additions.

# Dependencies

Most dependencies are vendored in the [3rdparty](3rdparty). Compile
dependencies will be used automatically during compilation. Runtime
dependencies include:

## SQL Server

https://www.microsoft.com/en-us/sql-server/sql-server-downloads

The character database is stored in SQL Server. Any version of SQL Server seems
to work. LocalDB is a lighter version of SQL Server that is recommended for
local testing and private use. Instructions to install and administrate SQL
Server is outside the scope of this guide.

* From the provided link, download the free SQL Server Express installer
* During installation, choose "Custom"
* Click through the default install wizard options until prompted for features
* On the features page, uncheck everything and then choose "LocalDB"
* Continue with defaults until installation is complete

NOTE: If you're prompted for something about "Azure", just disable that too

## ODBC17

https://learn.microsoft.com/en-us/sql/connect/odbc/download-odbc-driver-for-sql-server?view=sql-server-ver17

The ODBC driver is used by the game server to interface with the SQL Server.
It's basically the bridge between the game and the database. [ODBC 17
x86](https://go.microsoft.com/fwlink/?linkid=2361647) is the only driver that's
supported. Just download and run the installer.

# Building

Install Visual Studio 2026 with the **Desktop development with C++** workload,
including the Windows SDK and CMake tools. The Visual Studio 2026 generator
requires CMake 4.2 or newer. Git and network access are needed for CPM to fetch
build dependencies on the first configure.

From the repository root in a Visual Studio Developer PowerShell or command
prompt, configure once and build either configuration:

```text
cmake --preset vs2026
cmake --build --preset vs2026-optdebug
cmake --build --preset vs2026-release
```

The executables will be in the out/ folder.

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

First start the DBServer
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
