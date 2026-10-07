"""Create/save a real hero, restart both servers, and verify it through TestClient.

Requires Win32 developer builds and a complete local game data directory. All
writes and logs go into a fresh fixture; the supplied data is copied unchanged.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import socket
import sqlite3
import subprocess
import time
import uuid


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--bin", type=Path, required=True)
    parser.add_argument("--data-root", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--timeout", type=int, default=600)
    parser.add_argument("--reuse-fixture", type=Path, help="Reuse an earlier smoke-test fixture, resetting its disposable database")
    args = parser.parse_args()
    repo = Path(__file__).resolve().parents[2]
    binaries, source = args.bin.resolve(), args.data_root.resolve()
    for name in ("DbServer.exe", "MapServer.exe", "TestClient.exe"):
        if not (binaries / name).is_file():
            raise RuntimeError(f"Missing {binaries / name}")
    if not (source / "data/server/db/templates/ents.template").is_file():
        raise RuntimeError("Generate templates in the game data directory first")
    # Fail before starting if the local DBServer or MapServer ports are in use.
    for kind, port in ((socket.SOCK_STREAM, 6997), (socket.SOCK_DGRAM, 7000), (socket.SOCK_DGRAM, 6995)):
        with socket.socket(socket.AF_INET, kind) as probe:
            probe.bind(("127.0.0.1", port))
    fixture = args.reuse_fixture.resolve() if args.reuse_fixture else args.output.resolve() / ("sqlite-smoke-" + uuid.uuid4().hex[:12])
    marker = fixture / ".sqlite-smoke-fixture"
    if args.reuse_fixture and (not marker.is_file() or marker.read_text() != "CityOfHeroes SQLite smoke fixture\n"):
        raise RuntimeError("Only a fixture created by this test can be reused")
    fixture.mkdir(parents=True, exist_ok=bool(args.reuse_fixture))
    marker.write_text("CityOfHeroes SQLite smoke fixture\n")
    # The engine's development-data locator requires both data/ and tools/.
    (fixture / "tools").mkdir(exist_ok=True)
    print(f"Fixture and logs: {fixture}", flush=True)
    if not args.reuse_fixture:
        shutil.copytree(source / "data", fixture / "data")
        if (source / "piggs").is_dir():
            shutil.copytree(source / "piggs", fixture / "piggs")
    for path in binaries.iterdir():
        if path.suffix.lower() in (".exe", ".dll"):
            shutil.copy2(path, fixture / path.name)
    shutil.copy2(repo / "data/server/db/servers.cfg", fixture / "data/server/db/servers.cfg")
    database = fixture / "db/local/coh.sqlite"
    if args.reuse_fixture:
        for suffix in ("", "-wal", "-shm"):
            Path(str(database) + suffix).unlink(missing_ok=True)
        for name in ("create.report", "verify.report", "result.json"):
            (fixture / name).unlink(missing_ok=True)
    processes, logs = [], []
    env = {("Path" if k.upper() == "PATH" else k.upper()): v for k, v in os.environ.items()}

    def launch(name, arguments, label):
        stdout, stderr = fixture / (label + ".stdout.log"), fixture / (label + ".stderr.log")
        out, err = stdout.open("wb"), stderr.open("wb")
        logs.extend((out, err))
        process = subprocess.Popen([str(fixture / name), *arguments], cwd=fixture, env=env,
                                   stdout=out, stderr=err, stdin=subprocess.DEVNULL,
                                   creationflags=subprocess.CREATE_NO_WINDOW if os.name == "nt" else 0)
        processes.append(process)
        return process, stdout, stderr

    def wait_ready(process, files, marker):
        deadline = time.monotonic() + args.timeout
        while time.monotonic() < deadline:
            if process.poll() is not None:
                raise RuntimeError(f"Server exited {process.returncode}; see {files}")
            if any(marker in p.read_text(errors="replace") for p in files):
                return
            time.sleep(0.5)
        raise RuntimeError(f"Timed out waiting for {marker}; see {files}")

    def start_servers(round_number):
        db, out, err = launch("DbServer.exe", ["-nogui"], f"db-{round_number}")
        wait_ready(db, (out, err), "DbServer Ready.")
        game, out, err = launch("MapServer.exe", ["-db", "127.0.0.1", "-notimeout", "-nosharedmemory",
            "-hidetrans", "-map_id", "29", "-nogui", "-assertmode", "8256", "-nopopup"], f"map-{round_number}")
        wait_ready(game, (out, err), "Server ready.")
        return db, game

    def client(mode, character=None):
        report = fixture / (mode + ".report")
        arguments = ["-dontpause", "-hideconsole", "-nosharedmemory", "-fakeauth", "-db", "127.0.0.1", "-server", "127.0.0.1",
            "-localmapserver", "-authname", "SQLiteSmoke", "-persistencetest", mode, "-testreport", str(report)]
        if character:
            arguments += ["-character", character]
        process, out, err = launch("TestClient.exe", arguments, "client-" + mode)
        try:
            result = process.wait(timeout=args.timeout)
        except subprocess.TimeoutExpired:
            raise RuntimeError(f"TestClient timed out; see {out} and {err}")
        if result or not report.is_file():
            raise RuntimeError(f"TestClient {mode} failed ({result}); see {out} and {err}")
        return dict(line.split("=", 1) for line in report.read_text(encoding="utf-8").splitlines())

    def snapshot(hero):
        with sqlite3.connect(f"file:{database.as_posix()}?mode=ro", uri=True) as connection:
            row = connection.execute("SELECT ContainerId,Name,AuthName,InfluencePoints FROM Ents WHERE ContainerId=?", (int(hero["dbid"]),)).fetchone()
            if not row or row[1] != hero["name"] or row[3] != 7654321:
                raise RuntimeError(f"Saved character differs: {row}")
            if connection.execute("PRAGMA foreign_key_check").fetchone():
                raise RuntimeError("Foreign key integrity check failed")
            template = (fixture / "data/server/db/templates/ents.template").read_text(errors="replace")
            tables = sorted(set(re.findall(r"^([A-Za-z0-9_]+)\[", template, re.MULTILINE)))
            children = {table: connection.execute(f'SELECT COUNT(*) FROM "{table}" WHERE ContainerId=?', (int(hero["dbid"]),)).fetchone()[0] for table in tables}
            if not any(children.values()):
                raise RuntimeError("Character has no persisted subtable rows")
            return {"hero": row, "children": children}

    def stop(process):
        if process.poll() is None:
            process.terminate()
            try:
                process.wait(timeout=15)
            except subprocess.TimeoutExpired:
                process.kill(); process.wait(timeout=15)

    try:
        print("Starting fresh SQLite shard", flush=True)
        db, game = start_servers(1)
        created = client("create")
        before = snapshot(created)
        (fixture / "before.json").write_text(json.dumps(before, indent=2), encoding="utf-8")
        # Abrupt termination also verifies committed WAL recovery.
        stop(game); stop(db)
        print("Restarting DBServer and MapServer; resuming saved hero", flush=True)
        db, game = start_servers(2)
        restored = snapshot(created)
        (fixture / "restored.json").write_text(json.dumps(restored, indent=2), encoding="utf-8")
        if before != restored:
            raise RuntimeError("Saved character or child rows changed during server restart; compare before.json and restored.json")
        verified = client("verify", created["name"])
        after = snapshot(verified)
        (fixture / "after.json").write_text(json.dumps(after, indent=2), encoding="utf-8")
        if created != verified or before["hero"] != after["hero"]:
            raise RuntimeError("Character identity changed on resume; compare before.json and after.json")
        if any(after["children"][table] < count for table, count in before["children"].items()):
            raise RuntimeError("Persisted child rows disappeared on resume; compare before.json and after.json")
        (fixture / "result.json").write_text(json.dumps({"passed": True, **after}, indent=2), encoding="utf-8")
        print(f"PASS: {created['name']} ({created['dbid']}) survived server restart", flush=True)
    finally:
        for process in reversed(processes):
            stop(process)
        for stream in logs:
            stream.close()


if __name__ == "__main__":
    main()
