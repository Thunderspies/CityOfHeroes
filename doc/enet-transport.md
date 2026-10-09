# ENet game transport

Game clients, TestClient, the client-login listener and MapServer use ENet
1.3.18 over UDP. The existing `NetLink`, `Packet` and bitstream interfaces
remain the application boundary. ENet handles acknowledgement, retransmission,
fragmentation, sequencing and flow control for these connections.

The wire wrapper and channel policies are documented in `netio_enet.h`.
Streaming compression remains reliable and ordered. Unordered compression
uses independent zlib packets. Oversized unreliable packets are promoted to
reliable delivery so fragmentation does not depend on every datagram arriving.
The existing DH/Blowfish handshake, protocol negotiation and application packet
IDs remain in use. Ordinary transport connections tolerate a 60-second gap in
acknowledgements; the existing `notimeout` setting extends that window.

## Transport audit

| Connection | Transport |
| --- | --- |
| Client/TestClient to client-login service | ENet UDP by default; explicit TCP retained |
| Client/TestClient to MapServer | ENet UDP |
| MapServer internal database-query UDP listener | ENet UDP |
| Server-to-server and administrative TCP links | Existing TCP |
| AuthServer-specific protocol | Existing authentication transport |
| NetTest and legacy diagnostic UDP endpoints | Existing UDP |

The legacy transport remains available for diagnostics and unchanged callers.
ENet links bypass its acknowledgement, retransmission, duplicate-ID table and
sibling-fragment machinery. Backlog accessors account for ENet application
packets rather than treating every fragment as an independent queued packet.
MapServer peer capacity includes headroom for busy maps and reconnect churn;
existing configuration still determines game admission limits.

## Deployment and compatibility

This changes the UDP transport wire format. A legacy UDP peer cannot communicate
with an ENet listener, even when its application protocol version matches.
Application version negotiation happens inside an established ENet connection;
it does not negotiate between UDP transports. Mismatched peers therefore fail
connection establishment or time out instead of falling back automatically.

Deploy the client/TestClient and all client-facing server executables together.
Drain active sessions before replacing listeners, retain the configured UDP
ports, and verify login, character selection, map entry, a map transfer, logout
and reconnect before admitting normal traffic. Rollback also requires draining
sessions and restoring the previous matching client and server executables.
TCP service links retain their current transport and payload protocols.

## Verification

Configure with `COX_BUILD_UTILITIES_TESTS=ON` to build `EnetTest` and register
the `ENetTransport` CTest. It runs an encrypted client and server over loopback
and checks the application handshake, channel delivery, ordered and stateless
compression, a 1.5 MiB fragmented payload, packet IDs, cookies, idle keepalives,
loss/lag/jitter/reordering, disconnect callbacks, reconnect and packet-pool
balance. CTest runs it serially because it binds a fixed loopback port.

```powershell
cmake --preset vs2026 -DCOX_BUILD_UTILITIES_TESTS=ON
cmake --build out/build/vs2026 --config OptDebug --target EnetTest
ctest --test-dir out/build/vs2026 -C OptDebug -R ENetTransport --output-on-failure
```

The loopback test does not establish gameplay correctness or production
performance. A live shard smoke test and representative latency, bandwidth,
CPU and memory comparisons remain release validation tasks.
