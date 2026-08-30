# Computer Networking Foundations, Transport Layer Mechanics & Domain Resolution

> **Scope:** Deep Architectural Analysis of Network Reference Models (OSI 7-Layer vs. TCP/IP 4-Layer), Transport Layer Mechanics (TCP Stream Reliability vs. UDP Lightweight Datagrams), TCP Connection State Machines (3-Way Handshake, 4-Way Teardown, `TIME_WAIT` Invariants), Cryptographic Transport Security (TLS 1.2/1.3 Handshake Protocols), Hierarchical Domain Name System (DNS) Resolution Trees, and Flow Control vs. Congestion Control Mathematics.

---

# Table of Contents
1. [Network Reference Models: OSI 7-Layer vs. TCP/IP 4-Layer Architecture](#1-network-reference-models-osi-7-layer-vs-tcpip-4-layer-architecture)
2. [Transport Layer Mechanics: TCP vs. UDP](#2-transport-layer-mechanics-tcp-vs-udp)
3. [TCP Connection Lifecycle State Machines](#3-tcp-connection-lifecycle-state-machines)
4. [Transport Layer Security (TLS 1.2 / 1.3) Handshake Mechanics](#4-transport-layer-security-tls-12--13-handshake-mechanics)
5. [Domain Name System (DNS) Hierarchical Resolution Flow](#5-domain-name-system-dns-hierarchical-resolution-flow)
6. [Flow Control vs. Congestion Control Dynamics](#6-flow-control-vs-congestion-control-dynamics)
7. [Core Network Foundations Reference Matrix](#7-core-network-foundations-reference-matrix)

---

# 1. Network Reference Models: OSI 7-Layer vs. TCP/IP 4-Layer Architecture

```
+--------------------------+--------------------------+------------------------------------------------+
| OSI 7-LAYER MODEL        | TCP/IP 4-LAYER MODEL     | PROTOCOLS & DATA TRANSMISSION UNITS            |
+--------------------------+--------------------------+------------------------------------------------+
| 7. Application Layer     |                          | HTTP, HTTPS, DNS, SSH, FTP, SMTP, WebSockets   |
| 6. Presentation Layer    | Application Layer        | (Data Payloads & Messages)                     |
| 5. Session Layer         |                          |                                                |
+--------------------------+--------------------------+------------------------------------------------+
| 4. Transport Layer       | Transport Layer          | TCP (Segments), UDP (Datagrams), Port Numbers  |
+--------------------------+--------------------------+------------------------------------------------+
| 3. Network Layer         | Internet / Network Layer | IP (IPv4, IPv6), ICMP, Routers (Packets)       |
+--------------------------+--------------------------+------------------------------------------------+
| 2. Data Link Layer       | Link / Network Interface | Ethernet, Wi-Fi, Switches (MAC Frames)         |
| 1. Physical Layer        | Layer                    | Optical Fiber, Copper Cables, Transceivers     |
+--------------------------+--------------------------+------------------------------------------------+
```

### Layer Addressing & Boundary Roles:
- **Layer 2 (Data Link - MAC Addressing):** 48-bit physical hardware addresses (`00:1A:2B:3C:4D:5E`) used for frame delivery within the local physical segment / broadcast domain.
- **Layer 3 (Network - IP Addressing):** 32-bit (IPv4) or 128-bit (IPv6) logical addresses used for routing packets across autonomous network boundaries.

---

# 2. Transport Layer Mechanics: TCP vs. UDP

```
+----------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | TRANSMISSION CONTROL PROTOCOL (TCP)  | USER DATAGRAM PROTOCOL (UDP)         |
+----------------------------------------------------------------------------------------------------+
| Connection Model     | Connection-oriented (3-Way Handshake)| Connectionless (Zero setup overhead) |
| Delivery Guarantee   | 100% Guaranteed (ACKs + Retransmit)  | Best-effort (Packets may be dropped) |
| Stream Ordering      | Strictly in-order (Sequence numbers) | Unordered (Datagrams arrive randomly)|
| Flow/Congestion Ctrl | Integrated (Sliding Window, AIMD)    | Absent (Transmits at application rate|
| Framing Overhead     | 20-60 Bytes Header                   | Minimal 8 Bytes Header               |
| Primary Domain       | Web (HTTP/HTTPS), Database Drivers,  | Real-Time Media (WebRTC, VoIP),      |
|                      | File Transfer (SFTP), SSH, Messaging | Online Gaming, DNS Queries, HTTP/3   |
+----------------------------------------------------------------------------------------------------+
```

---

# 3. TCP Connection Lifecycle State Machines

```
TCP 3-Way Handshake (Connection Establishment):
Client (Active Open)                                             Server (Passive Open)
  |                                                                | (LISTEN)
  |--- 1. SYN (seq = x) ------------------------------------------>| (SYN_RCVD)
  |<-- 2. SYN-ACK (seq = y, ack = x + 1) --------------------------|
  |--- 3. ACK (ack = y + 1) -------------------------------------->|
  |                                                                |
  [ ESTABLISHED ]                                                  [ ESTABLISHED ]
```

```
TCP 4-Way Teardown (Connection Termination):
Client (Active Close)                                            Server (Passive Close)
  |--- 1. FIN (seq = u) ------------------------------------------>| (CLOSE_WAIT)
  |<-- 2. ACK (ack = u + 1) ---------------------------------------|
  |                                                                |
  |<-- 3. FIN (seq = w) -------------------------------------------| (LAST_ACK)
  |--- 4. ACK (ack = w + 1) -------------------------------------->|
  |                                                                |
  [ TIME_WAIT (2 * MSL) ]                                          [ CLOSED ]
```

### The `TIME_WAIT` State & 2MSL Invariant:
The active-closing endpoint remains in the `TIME_WAIT` state for **$2 \times \text{MSL}$ (Maximum Segment Lifetime)**, typically 60 to 120 seconds:
1. **Guarantees Final ACK Delivery:** If the server fails to receive the final ACK, it retransmits its FIN; the client must remain active to acknowledge it.
2. **Flushes Old Duplicate Segments:** Prevents delayed packets from a previous connection from corrupting subsequent connections instantiated on the same IP:Port 4-tuple.

---

# 4. Transport Layer Security (TLS 1.2 / 1.3) Handshake Mechanics

HTTPS encapsulates HTTP communications within an encrypted TLS cryptographic tunnel operating on port 443:

```
TLS 1.3 Cryptographic Handshake (1-RTT Fast Path):
Client                                                           Server
  |                                                                |
  |--- 1. ClientHello (Key Share: ECDH Public Key, Cipher Suites)->|
  |<-- 2. ServerHello (Key Share: ECDH Public Key, Certificate) ---| (Server Authenticated via CA)
  |                                                                |
  |<== [ Shared Symmetric Session Key Established (AES-GCM) ] ====>|
  |                                                                |
  |--- 3. Encrypted Application Data (HTTP Payload) -------------->|
```

1. **Identity Authentication (Asymmetric Cryptography):** The server presents an X.509 certificate signed by a trusted Certificate Authority (CA), authenticated via RSA or ECDSA digital signatures.
2. **Session Key Negotiation (ECDHE):** Elliptic Curve Diffie-Hellman Ephemeral key exchange generates an ephemeral shared secret with **Forward Secrecy**.
3. **Data Encryption (Symmetric Cryptography):** High-speed symmetric ciphers (AES-256-GCM, ChaCha20-Poly1305) encrypt all application payloads.

---

# 5. Domain Name System (DNS) Hierarchical Resolution Flow

DNS is a globally distributed, hierarchical database translating human-readable hostnames into 32-bit/128-bit IP addresses:

```
DNS Hierarchical Resolution Path:
[ Client Resolver ] ---> [ Local OS / Hosts Cache ] ---> [ Recursive DNS Server (8.8.8.8) ]
                                                                      |
                   +--------------------------------------------------+--------------------------------------------------+
                   |                                                  |                                                  |
                   v                                                  v                                                  v
         1. Root Server (.)                               2. TLD Server (.com)                       3. Authoritative Nameserver
         (Returns .com TLD IP)                            (Returns google.com Nameserver IP)         (Returns definitive A record IP)
```

- **Record Types:** `A` (IPv4 address), `AAAA` (IPv6 address), `CNAME` (Canonical alias), `MX` (Mail exchanger), `TXT` (SPF/DKIM verification).
- **Time-to-Live (TTL):** Specifies the duration (in seconds) intermediate caching resolvers may retain records before querying authoritative servers.

---

# 6. Flow Control vs. Congestion Control Dynamics

```
+----------------------------------------------------------------------------------------------------+
| MECHANISM            | TARGET PROTECTED                     | GOVERNING ALGORITHM & INVARIANT      |
+----------------------------------------------------------------------------------------------------+
| Flow Control         | Protects the **Receiver's Buffer**   | Sliding Window (`rwnd`): Receiver    |
|                      | from sender memory exhaustion        | advertises available buffer capacity |
| Congestion Control   | Protects the **Network Routers**     | Congestion Window (`cwnd`): Sender   |
|                      | from packet drop queue collapses     | dynamically probes capacity via AIMD |
+----------------------------------------------------------------------------------------------------+
```

### Additive Increase Multiplicative Decrease (AIMD):
$$\text{Max Transmit Capacity} = \min(\text{rwnd}, \, \text{cwnd})$$
- **Slow Start:** `cwnd` doubles exponentially each RTT until reaching `ssthresh`.
- **Congestion Avoidance:** `cwnd` increments linearly ($+1 \text{ MSS}$ per RTT).
- **Packet Loss Detection:** Halves `cwnd` on triple-duplicate ACKs (Fast Retransmit); collapses `cwnd` to 1 MSS on timeout.

---

# 7. Core Network Foundations Reference Matrix

```
+----------------------------------------------------------------------------------------------------+
| NETWORK PROTOCOL     | OSI LAYER LEVEL      | RELIABILITY MODEL    | STATE MACHINE COMPLEXITY      |
+----------------------------------------------------------------------------------------------------+
| IP (IPv4/IPv6)       | Layer 3 (Network)    | Best-Effort Datagram | Stateless Packet Forwarding   |
| TCP                  | Layer 4 (Transport)  | Guaranteed In-Order  | Stateful 3-Way Handshake/Teardown|
| UDP                  | Layer 4 (Transport)  | Connectionless Unrel | Stateless Framing             |
| TLS 1.3              | Layer 4/7 Security   | Cryptographic Tunnel | 1-RTT Key Negotiation         |
| DNS Protocol         | Layer 7 (Application)| UDP Port 53 Primary  | Hierarchical Recursive Tree   |
+----------------------------------------------------------------------------------------------------+
```
