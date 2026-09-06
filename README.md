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
