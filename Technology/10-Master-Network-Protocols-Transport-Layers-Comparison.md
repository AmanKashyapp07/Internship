# Types of Network Protocols & Transport Layers — Interview Notes

A guide to speak about OSI/TCP-IP models, network packet physics, transport layer
trade-offs (TCP vs. UDP vs. QUIC), and application protocols (HTTP/1.1, HTTP/2, HTTP/3,
WebSocket, gRPC) with the confidence of an engineer who has debugged production packet drops,
tuned socket buffers, and optimized network latencies.

---

## What They Are & Why They Are Needed

### 1. What is a Network Protocol?
A network protocol is an agreed-upon set of rules, packet formatting standards,
and state machines that dictate how data is serialized, routed, multiplexed, and
error-corrected across physical networks and operating system kernels.

```
+---------------------------------------------------------------------------------------------------+
| LAYER (TCP/IP MODEL)      | PROTOCOLS                               | PHYSICAL UNIT / FUNCTION    |
+---------------------------------------------------------------------------------------------------+
| 4. Application Layer      | HTTP/1.1, HTTP/2, HTTP/3, gRPC, WS, DNS | Messages & Application Data |
+---------------------------------------------------------------------------------------------------+
| 3. Transport Layer        | TCP (Reliable / Stream), UDP / QUIC     | Segments / Datagrams (Ports)|
+---------------------------------------------------------------------------------------------------+
| 2. Internet / Network     | IP (IPv4, IPv6), ICMP, BGP Anycast      | Packets (IP Routing)        |
+---------------------------------------------------------------------------------------------------+
| 1. Link / Physical Layer  | Ethernet, Wi-Fi (802.11), Fiber Optics  | Frames / Raw Bits           |
+---------------------------------------------------------------------------------------------------+
```

### 2. Why Are They Needed?
1. **The Physics of Unreliable Networks:** Physical wires, fiber optic cables, and radio waves drop packets, corrupt bits, and reorder data; transport protocols (like TCP) provide ordered reliability over chaos.
2. **Bandwidth & Latency Optimization:** Eliminating unnecessary round trips (RTTs), compressing redundant metadata (HPACK/QPACK), and multiplexing concurrent streams over a single connection prevents network saturation.
3. **Head-of-Line (HOL) Blocking Elimination:** Evolution from HTTP/1.1 to HTTP/2 and HTTP/3 represents a decade-long battle to stop a single dropped packet from freezing unrelated data streams.

---

## 1. TCP vs. UDP (Transport Layer Physics)

**What it is:** The two foundational Transport Layer (Layer 4) protocols governing
how bytes flow between IP ports.

**Strengths**
- **TCP (Transmission Control Protocol):** Connection-oriented, strictly ordered, reliable byte stream.
  - *3-Way Handshake:* `SYN` $\to$ `SYN-ACK` $\to$ `ACK` establishes connection and syncs sequence numbers.
  - *Flow Control:* Sliding Window protocol prevents sender from overwhelming receiver's buffer.
  - *Congestion Control:* Algorithms (CUBIC, BBR) dynamically detect network bottlenecks and adjust bandwidth.
- **UDP (User Datagram Protocol):** Connectionless, lightweight datagram protocol with zero handshake overhead.
  - Sends fire-and-forget packets without acknowledging receipt, tracking state, or retransmitting lost packets.

**Weaknesses**
- TCP: High latency overhead (1-RTT handshake + TLS handshake); Head-of-Line blocking (a single dropped packet pauses the entire stream until retransmitted).
- UDP: Zero reliability guarantees; packets can arrive out of order, duplicated, or get dropped completely.

**When to use:** TCP for web pages, APIs, database connections, and financial transactions;
UDP for DNS lookups, real-time gaming inputs, live video streaming, WebRTC media, and QUIC (HTTP/3).

**Interview-ready line:** "TCP trades latency and connection overhead for guaranteed, ordered byte delivery;
UDP eliminates handshakes and Head-of-Line blocking for real-time applications that prioritize freshness over completeness."

---

## 2. HTTP/1.1 (Persistent Connections & HOL Blocking)

**What it is:** The text-based application protocol that powered the early web (RFC 2616),
introducing persistent TCP connections (`Connection: keep-alive`).

**Strengths**
- Universal simplicity: human-readable ASCII text format (`GET /index.html HTTP/1.1\r\nHost: ...`).
- Native edge CDN cacheability via `Cache-Control`, `ETag`, and `304 Not Modified` headers.
- Persistent connections: reuses a single TCP connection for multiple sequential HTTP requests.

**Weaknesses**
- **Application-Layer Head-of-Line (HOL) Blocking:** Requests on a single TCP connection must be
  served in strict FIFO order; if request #1 is slow, requests #2–6 are blocked.
- Browser 6-Connection Limit: browsers open 6 separate TCP connections per domain to simulate
  parallelism, consuming massive server memory and CPU context switches.
- Verbose uncompressed headers: large repetitive cookies and headers transmitted in plain text on every request.

**When to use:** Simple legacy APIs, internal webhook receivers, and edge proxy health checks.

**Interview-ready line:** "HTTP/1.1 introduced keep-alive persistence, but its plain-text format
and strict FIFO serialization created application-level Head-of-Line blocking, forcing browsers to open 6 separate TCP sockets per domain."

---

## 3. HTTP/2 (Binary Framing & Multiplexing)

**What it is:** A major transport overhaul of HTTP (RFC 7540) that replaces plain text
with a **Binary Framing Layer**, multiplexing concurrent streams over a single TCP connection.

**Strengths**
- **True Connection Multiplexing:** Multiple bidirectional logical **Streams** run concurrently
  over a single TCP socket; responses return out-of-order as interleaved binary frames without blocking.
- **HPACK Header Compression:** Uses static/dynamic Huffman tables to compress repetitive headers by up to 85%.
- **Stream Prioritization:** Clients assign dependency weights to streams (e.g. prioritize CSS/JS over footer images).
- Single TCP connection per domain: eliminates browser multi-socket connection overhead and TLS handshakes.

**Weaknesses**
- **TCP-Level Head-of-Line Blocking:** Because HTTP/2 multiplexes all streams over a *single* TCP socket,
  if physical packet loss occurs, the OS TCP stack pauses **all streams** until the missing packet is retransmitted.
- Server Push complexity: rarely used in production and removed from Chrome due to cache invalidation issues.

**When to use:** Modern web applications, API gateways, gRPC microservice communication, and content-rich websites.

**Interview-ready line:** "HTTP/2 solved application Head-of-Line blocking by multiplexing binary frames over a single TCP socket,
but it exposed TCP-level Head-of-Line blocking on lossy mobile networks."

---

## 4. HTTP/3 & QUIC (UDP Transport & 0-RTT Handshakes)

**What it is:** The next-generation HTTP standard (RFC 9114) that runs on top of
**QUIC**, a user-space transport protocol built directly on **UDP** instead of TCP.

**Strengths**
- **Zero TCP Head-of-Line Blocking:** Because QUIC handles stream multiplexing inside user-space
  over UDP, a dropped packet on Stream A delays only Stream A; Streams B through Z continue processing unaffected.
- **0-RTT Connection Resumption:** Combines transport and TLS 1.3 cryptographic handshakes into a
  single exchange; returning clients send encrypted data on the very first packet.
- **Connection Migration (Mobile IP Hopping):** Connections are identified by a 64-bit **Connection ID (CID)**
  rather than the IP 4-tuple (`SrcIP:SrcPort:DstIP:DstPort`). When a user switches from Wi-Fi to Cellular 5G, active downloads and video streams continue without dropping or re-authenticating.

**Weaknesses**
- High CPU utilization: UDP processing in Linux kernel historically consumes more CPU cycles than hardware-offloaded TCP.
- Enterprise firewall blocking: some corporate firewalls block UDP port 443, requiring automatic fallback to HTTP/2.

**When to use:** Mobile applications (Uber, TikTok, YouTube), high-packet-loss mobile networks,
and global low-latency web delivery (Cloudflare, Google).

**Interview-ready line:** "HTTP/3 runs QUIC over UDP to eliminate TCP Head-of-Line blocking completely,
enabling 0-RTT handshakes and seamless Connection Migration when mobile devices switch between Wi-Fi and Cellular."

---

## 5. WebSockets (RFC 6455 Persistent Full-Duplex TCP)

**What it is:** A persistent, full-duplex, bidirectional communication channel over
a single TCP connection designed for continuous real-time streaming.

**Strengths**
- Sub-millisecond latency: minimal 2-to-10 byte framing overhead per message with zero
  repeated HTTP request headers.
- True bidirectional streaming: both client and server push text or raw binary `ArrayBuffers` at will.
- Initiates via standard HTTP: upgrades seamlessly via `HTTP 101 Switching Protocols`.

**Weaknesses**
- Stateful connections: sockets bind to specific server memory, complicating horizontal
  scaling (requires sticky sessions or a Redis Pub/Sub mesh).
- Custom message protocol required: no built-in request/response semantics or status codes.

**When to use:** Real-time chat apps, multiplayer gaming, live trading tickers, and collaborative whiteboards (Figma).

**Interview-ready line:** "WebSockets trade statelessness for raw bidirectional latency —
I use them when continuous two-way interaction is mandatory, scaling them with Redis Pub/Sub backplanes."

---

## 6. gRPC & Protocol Buffers (High-Performance RPC)

**What it is:** An open-source, high-performance Remote Procedure Call (RPC) framework
developed by Google that uses **Protocol Buffers (Protobuf)** over **HTTP/2**.

**Strengths**
- Extreme binary serialization speed: Protobuf encodes structured data into compact binary
  streams using varints and field tags (5–10x smaller and 20x faster to parse than JSON).
- Strict Interface Definition Language (`.proto`): contract-first design with automated client/server
  stub code generation across polyglot languages (Go, Java, C++, Python, Node.js).
- 4 Streaming Paradigms: Unary (Req/Res), Server Streaming, Client Streaming, and Bidirectional Streaming.

**Weaknesses**
- Not browser-native: browsers cannot make direct gRPC calls without a proxy layer (`grpc-web` or Envoy).
- Binary payloads are not human-readable: harder to debug with standard `curl` without reflection tooling.

**When to use:** Internal microservice-to-microservice communication (east-west traffic in Kubernetes)
and high-throughput low-latency internal APIs.

**Interview-ready line:** "gRPC is my default for internal microservices where contract safety and sub-5ms latency matter —
its Protobuf binary serialization over HTTP/2 eliminates JSON parsing overhead completely."

---

## 7. TLS 1.2 vs. TLS 1.3 (Cryptographic Handshake Optimization)

**What it is:** Transport Layer Security protocols providing **Confidentiality, Integrity,
and Authentication** across the internet.

**Strengths**
- **TLS 1.3 Handshake Speed (1-RTT):** Reduces connection setup time from 2 round trips (TLS 1.2)
  down to **1 RTT** by combining cipher negotiation with the Diffie-Hellman key exchange.
- **0-RTT Early Data:** Returning clients send encrypted application data in the first handshake flight.
- Modernized Cryptographic Security: eliminated insecure legacy ciphers (RC4, DES, MD5, SHA-1) and
  static RSA key exchanges, enforcing **Perfect Forward Secrecy (PFS)** via Ephemeral Diffie-Hellman (`ECDHE`).

**Weaknesses**
- 0-RTT Early Data is vulnerable to **Replay Attacks**; application servers must treat 0-RTT requests
  as non-idempotent or restrict them strictly to safe `GET` requests.

**When to use:** Enforced on all public HTTPS web traffic, API gateways, and internal mTLS service meshes.

**Interview-ready line:** "TLS 1.3 cut cryptographic connection latency in half by dropping the handshake from 2-RTT to 1-RTT,
mandating Perfect Forward Secrecy and removing vulnerable legacy ciphers."

---

## 8. DNS & Anycast Routing (Resolution & Global Traffic)

**What it is:** The global routing and naming infrastructure that translates human-readable
domains (`api.example.com`) into IP addresses and routes packets to the nearest datacenter.

**Strengths**
- **Recursive vs. Authoritative Resolution:** Browser $\to$ Recursive Resolver $\to$ Root Nameserver (`.`)
  $\to$ TLD Nameserver (`.com`) $\to$ Authoritative Nameserver (`example.com`), cached with TTLs.
- **Anycast BGP Routing:** Multiple physical datacenters worldwide advertise the **exact same IP address**;
  internet BGP routers route client packets to the topologically closest geographic datacenter (powers Cloudflare, Google DNS 8.8.8.8, and AWS Route 53).
- Fast failover: if a datacenter fails, BGP withdraws the route, instantly rerouting traffic to the next closest PoP.

**Weaknesses**
- DNS propagation delay: cached records across global ISPs take time to update based on TTLs.
- UDP amplification attacks: DNS operates primarily over UDP Port 53, making it susceptible to spoofed DDoS reflection attacks (mitigated via RRL and DNSSEC).

**When to use:** Global traffic management, edge CDN routing, multi-region failover, and DDoS mitigation.

**Interview-ready line:** "Anycast BGP routing assigns the same IP address to dozens of global datacenters,
letting the internet's routing fabric automatically send users to the topologically nearest Point of Presence."

---

## Side-by-Side Comparison

| Protocol | Transport | Framing Format | Multiplexing | Handshake Latency | Head-of-Line Blocking | Best Production Role |
|---|---|---|---|---|---|---|
| **HTTP/1.1** | TCP | Plain ASCII Text | No (FIFO per socket)| 1 RTT TCP + TLS | Application HOL (FIFO)| Simple legacy APIs |
| **HTTP/2** | TCP | Binary Frames | Yes (Streams on 1 TCP)| 1 RTT TCP + TLS | **TCP HOL (on packet loss)**| Web apps, Public APIs |
| **HTTP/3** | **QUIC (UDP)**| Binary Frames | **Yes (Independent Streams)**| **0–1 RTT (Combined)**| **Zero HOL Blocking** | Mobile apps, Global Web |
| **WebSocket**| TCP | Binary/Text Frames| Full-Duplex Socket | HTTP 101 Upgrade | TCP HOL | Live Chat, Gaming, Tickers|
| **gRPC** | HTTP/2 (TCP)| Protobuf (Binary)| Yes (HTTP/2 Streams)| 1 RTT TCP + TLS | TCP HOL | Internal Microservices |
| **DNS** | UDP / TCP (53)| Binary Wire Format | No (Req/Res) | 0 RTT (UDP) | None (Stateless) | Name Resolution, Anycast |

---

## Decision Framework (say this out loud in interviews)

1. **Public web app or API with mobile and desktop consumers** → HTTP/2 (or HTTP/3 on CDN edge).
2. **Internal microservice-to-microservice communication in Kubernetes** → gRPC with Protobuf over HTTP/2.
3. **Continuous two-way real-time communication (chat, multiplayer, collaborative canvas)** → WebSockets (RFC 6455).
4. **Mobile apps operating on lossy Wi-Fi/5G networks with frequent network hopping** → HTTP/3 with QUIC.
5. **Streaming AI LLM completions (ChatGPT UI) or live notifications** → Server-Sent Events (SSE) over HTTP/2.
6. **Global low-latency DNS and multi-region edge routing** → Anycast BGP Routing + Route 53 / Cloudflare.
7. **Transport layer security** → Enforce TLS 1.3 (1-RTT handshake + Perfect Forward Secrecy).

---

## What separates a senior answer from a junior one

### 1. Head-of-Line (HOL) Blocking Evolution
- **Junior:** "HTTP/2 is always faster than HTTP/1.1, and HTTP/3 is just newer."
- **Senior:** *"A senior explains the mechanical evolution across network layers:
  - **HTTP/1.1 suffered from Application-Layer HOL Blocking:** A slow response blocked subsequent requests on the same TCP socket, forcing browsers to open 6 separate connections.
  - **HTTP/2 solved Application HOL via Binary Stream Multiplexing**, but because it multiplexes over a single TCP connection, it introduced **TCP-Layer HOL Blocking**: a single dropped packet stalls the OS TCP window, pausing all concurrent streams.
  - **HTTP/3 over QUIC (UDP) completely eliminates TCP HOL Blocking**: each stream is an independent entity inside QUIC. A dropped packet on Stream 1 halts only Stream 1; Streams 2 through 100 continue delivering frames without a microsecond of delay."*

### 2. TCP Congestion Control Physics (CUBIC vs. BBR)
- **Junior:** "TCP handles speed automatically."
- **Senior:** *"A senior understands congestion control algorithms:
  - **CUBIC (Loss-Based):** Assumes packet loss equals network congestion; upon packet drop, it halves the congestion window. On high-speed long-distance links or lossy Wi-Fi, CUBIC drastically underutilizes available bandwidth.
  - **BBR (Bottleneck Bandwidth and RTT - Google):** Model-based congestion control that measures maximum delivery rate and minimum round-trip time. It pumps data at the physical link capacity without filling buffer queues, delivering up to **10x higher throughput and lower latency** on lossy networks."*

### 3. TLS 1.3 0-RTT Security Trade-offs (Replay Attacks)
- **Junior:** "TLS 1.3 has 0-RTT, so we should enable it everywhere to make our app instant."
- **Senior:** *"0-RTT Early Data sends encrypted application data in the very first packet using pre-shared keys from a previous session. However, **0-RTT data is vulnerable to Replay Attacks**: an eavesdropping attacker can capture the initial 0-RTT packet and replay it to the server multiple times. A senior enables 0-RTT **strictly for idempotent `GET` requests** and blocks 0-RTT on financial transactions (`POST /checkout`) or state-mutating endpoints."*

### 4. QUIC Connection Migration in Mobile Architecture
- **Junior:** "QUIC is just UDP."
- **Senior:** *"TCP connections are bound to the socket 4-tuple (`SrcIP`, `SrcPort`, `DstIP`, `DstPort`). When a user walking down the street steps out of Wi-Fi range and switches to Cellular 5G, the phone's IP changes, breaking the TCP socket and forcing a complete 3-way handshake + TLS re-negotiation. **QUIC uses an opaque 64-bit Connection ID (CID)** that is independent of IP addresses. When the network interface hops, the client simply transmits the same CID from its new IP, and the QUIC server seamlessly resumes the stream with zero reconnection latency."*

### 5. Transport Selection: WebSockets vs. gRPC vs. SSE
- **Junior:** "We should use WebSockets for all real-time and streaming APIs."
- **Senior:** *"A senior matches the transport to communication directionality and operational cost:
  - **WebSockets:** Stateful, full-duplex TCP for two-way continuous interactions (gaming, chat, cursors), requiring sticky sessions and Redis Pub/Sub meshes.
  - **Server-Sent Events (SSE):** Lightweight, mono-directional server push over standard HTTP/2 (`text/event-stream`), ideal for AI LLM token streaming with zero WebSocket cluster overhead.
  - **gRPC Streaming:** Binary Protobuf over HTTP/2 for internal microservice pipelines requiring high throughput and strict compile-time type safety."*
