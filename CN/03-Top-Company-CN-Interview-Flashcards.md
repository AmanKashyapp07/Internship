# Computer Networks Comprehensive Reference & Protocol Deep-Dives

> **Scope:** Deep Architectural Analysis of 50 Core Networking Systems Concepts: Reference Models, Frame Encapsulation, Hardware Switching vs. Routing, Transport Layer Mechanics (TCP Congestion Engines, SYN Cookies, Head-of-Line Blocking), Network Layer Protocols (IPv4/IPv6, CIDR Subnetting, NAT/PAT, BGP Path-Vector Routing, ICMP Traceroute), Application Protocols (HTTP/1.1 vs. HTTP/2 Multiplexing vs. HTTP/3 QUIC, TLS 1.3 Handshakes, gRPC Protobuf, WebSockets), and Network Security & Caching Topologies.

---

# Table of Contents
1. [Network Reference Models & Physical Framing (Deep Dives 1-8)](#1-network-reference-models--physical-framing-deep-dives-1-8)
2. [Transport Layer Mechanics & Congestion Control (Deep Dives 9-18)](#2-transport-layer-mechanics--congestion-control-deep-dives-9-18)
3. [Network Layer, Addressing & Routing Protocols (Deep Dives 19-26)](#3-network-layer-addressing--routing-protocols-deep-dives-19-26)
4. [Application Layer Protocols & Security Handshakes (Deep Dives 27-36)](#4-application-layer-protocols--security-handshakes-deep-dives-27-36)
5. [Domain Resolution, Edge Caching & Perimeter Defense (Deep Dives 37-50)](#5-domain-resolution-edge-caching--perimeter-defense-deep-dives-37-50)

---

# 1. Network Reference Models & Physical Framing (Deep Dives 1-8)

### 1. Structural Comparison: OSI 7-Layer vs. TCP/IP 4-Layer Models
The **OSI Model** is an abstract theoretical blueprint dividing network tasks into 7 distinct boundaries. The **TCP/IP Model** is the practical architecture implemented in production operating system kernels, consolidating OSI Application, Presentation, and Session layers into a unified **Application Layer**, followed by **Transport, Internet, and Link** layers.

---

### 2. Multi-Layer Packet Encapsulation and Decapsulation Mechanics
As data traverses downward through the protocol stack, each layer encapsulates the higher-layer payload with metadata headers:
$$\text{Application Data} \to \text{TCP/UDP Segment (Port Headers)} \to \text{IP Packet (IP Headers)} \to \text{Link Frame (MAC Header + CRC)}$$
At the receiving endpoint, the hardware NIC and OS kernel parse and strip corresponding headers in reverse order (Decapsulation).

---

### 3. Layer 2 MAC Addresses vs. Layer 3 IP Addresses
- **MAC Address (Layer 2):** 48-bit physical identifier burned into hardware NICs, used for direct frame delivery within a single physical broadcast domain.
- **IP Address (Layer 3):** 32-bit (IPv4) or 128-bit (IPv6) logical, globally hierarchical address used by routers to deliver packets across disparate network boundaries.

---

### 4. Layer 1 Hubs vs. Layer 2 Switches vs. Layer 3 Routers
- **Hub (Layer 1):** Physical repeater that blindly replicates incoming electrical signals across all attached ports.
- **Switch (Layer 2):** Data link device inspecting MAC addresses to forward frames selectively to destination ports via internal MAC address tables (CAM tables).
- **Router (Layer 3):** Network gateway examining destination IP addresses to route packets across autonomous subnets using routing tables (OSPF, BGP).

---

### 5. Transport Layer Port Addressing
A **Port Number** is a 16-bit unsigned integer ($0\text{--}65535$) identifying specific application processes within an operating system. Ports $0\text{--}1023$ are reserved well-known ports (e.g. HTTP: 80, HTTPS: 443, SSH: 22, DNS: 53).

---

### 6. Network Socket Primitives
A **Socket** is an operating system file descriptor abstraction representing an open network connection, uniquely defined by the 5-tuple:
$$(\text{Source IP}, \, \text{Source Port}, \, \text{Destination IP}, \, \text{Destination Port}, \, \text{Transport Protocol})$$

---

### 7. Unicast, Broadcast, Multicast, and Anycast Topologies
- **Unicast:** One-to-one transmission directed to a single discrete destination IP.
- **Broadcast:** One-to-all transmission addressing every active host in the local Layer 2 subnet.
- **Multicast:** One-to-many transmission directed to a registered group of subscribed endpoints.
- **Anycast:** One-to-nearest routing where multiple distributed servers advertise identical IP addresses via BGP, routing traffic to the topologically closest host.

---

### 8. Maximum Transmission Unit (MTU) & IP Fragmentation
**MTU** defines the largest packet payload (typically 1500 bytes on Ethernet) transmissible across a physical link without subdivision. When a packet exceeds an intermediate router's link MTU without the "Don't Fragment" (DF) bit set, the router fragments the payload into smaller IP fragments reassembled at the destination host.

---

# 2. Transport Layer Mechanics & Congestion Control (Deep Dives 9-18)

### 9. Fundamental Tradeoffs: TCP vs. UDP
- **TCP:** Connection-oriented, reliable, strictly in-order byte stream with dynamic flow control and congestion avoidance at the cost of 1-RTT connection setup and packet retransmission latency.
- **UDP:** Connectionless, unordered datagram protocol with minimal 8-byte headers and zero retransmission overhead, maximizing throughput for real-time video, audio, and DNS.

---

### 10. TCP 3-Way Handshake Protocol Sequence
1. **Client sends SYN (seq = $x$):** Synchronizes initial client sequence number.
2. **Server responds with SYN-ACK (seq = $y$, ack = $x+1$):** Acknowledges client and synchronizes server sequence number.
3. **Client sends ACK (ack = $y+1$):** Acknowledges server sequence number. Sockets transition to the `ESTABLISHED` state.

---

### 11. Theoretical Requirement for a 3-Way Handshake
A 2-way exchange confirms only that the client can receive and the server can send; it provides **zero confirmation to the server that the client received the SYN-ACK**. Without the final ACK, delayed duplicate SYN packets could cause servers to allocate half-open state resources indefinitely.

---

### 12. TCP 4-Way Connection Teardown Mechanics
1. Active closer transmits **FIN** (closes write stream).
2. Passive receiver returns **ACK** (transitions to `CLOSE_WAIT`; can continue transmitting data).
3. Passive receiver transmits **FIN** when data stream ends.
4. Active closer returns **ACK** and enters `TIME_WAIT`.

---

### 13. Purpose of the TCP `TIME_WAIT` State ($2\text{MSL}$)
The active-closing endpoint remains in `TIME_WAIT` for twice the Maximum Segment Lifetime ($2\text{MSL}$, 60-120s) to guarantee delivery of the final ACK and ensure lingering delayed segments drain completely from intermediate network queues before the port pair is reused.

---

### 14. Flow Control vs. Congestion Control Invariants
- **Flow Control:** Protects the receiver's memory buffer via the advertised **Receive Window (`rwnd`)**.
- **Congestion Control:** Protects intermediate network routers via the calculated **Congestion Window (`cwnd`)**.

---

### 15. TCP Congestion Avoidance: Slow Start & AIMD Dynamics
- **Slow Start:** Exponential growth of `cwnd` ($2\times$ per RTT) until reaching `ssthresh`.
- **Congestion Avoidance:** Linear increase of `cwnd` ($+1\text{ MSS}$ per RTT).
- **AIMD (Additive Increase Multiplicative Decrease):** Halves `cwnd` on triple-duplicate ACKs; collapses `cwnd` to 1 MSS on timeout.

---

### 16. Head-of-Line (HoL) Blocking at the Transport Layer
Because TCP provides a strictly ordered byte stream, a single dropped packet forces all subsequent in-flight packets to sit in the OS receive buffer until the missing packet is retransmitted and acknowledged.

---

### 17. SYN Flood Attacks & Cryptographic SYN Cookie Mitigation
A SYN Flood exhausts server half-open connection tables using spoofed IP addresses. **SYN Cookies** eliminate backlog memory tables by encoding connection parameters into the 32-bit initial sequence number ($y$) of the SYN-ACK, allocating server memory only when the client returns a valid final ACK containing $y+1$.

---

### 18. TCP Keep-Alive vs. HTTP Keep-Alive
- **TCP Keep-Alive:** OS-level transport probe transmitting null packets on idle connections to detect peer crashes.
- **HTTP Keep-Alive:** Application-level header reusing a single persistent TCP connection across multiple sequential HTTP requests.

---

# 3. Network Layer, Addressing & Routing Protocols (Deep Dives 19-26)

### 19. IPv4 vs. IPv6 Architecture
- **IPv4:** 32-bit address space (~4.3 billion addresses), requiring NAT to mitigate exhaustion.
- **IPv6:** 128-bit address space ($3.4 \times 10^{38}$ addresses), featuring native IPSec support, simplified router headers, and elimination of NAT requirements.

---

### 20. CIDR Notation & Subnet Allocation
Classless Inter-Domain Routing (CIDR) uses `/N` prefix notation to designate network mask bits. A `/24` subnet reserves 24 bits for the network, providing $32 - 24 = 8$ host bits ($2^8 = 256$ total addresses, yielding 254 assignable host addresses after excluding network `.0` and broadcast `.255`).

---

### 21. Private Address Spaces (RFC 1918)
- Class A: `10.0.0.0/8` (`10.0.0.0` - `10.255.255.255`)
- Class B: `172.16.0.0/12` (`172.16.0.0` - `172.31.255.255`)
- Class C: `192.168.0.0/16` (`192.168.0.0` - `192.168.255.255`)

---

### 22. Network Address Translation (NAT) & Port Address Translation (PAT)
- **NAT:** Rewrites private source IP addresses to a public IP address during egress.
- **PAT (NAT Overload):** Maps thousands of private IP:Port flows to distinct temporary ephemeral ports on a single public IP address, multiplexing local networks over a shared public IP.

---

### 23. Dynamic Host Configuration Protocol (DHCP DORA Sequence)
1. **Discover:** Client broadcasts a `DHCPDISCOVER` packet searching for DHCP servers.
2. **Offer:** DHCP server broadcasts a `DHCPOFFER` with an available IP address.
3. **Request:** Client broadcasts a `DHCPREQUEST` accepting the offered lease.
4. **Acknowledge:** Server transmits a `DHCPACK` confirming IP lease, subnet mask, gateway, and DNS.

---

### 24. Address Resolution Protocol (ARP) & ARP Cache Poisoning
**ARP** broadcasts Layer 2 queries to resolve a known Layer 3 IP address to a physical MAC address on the local link. **ARP Cache Poisoning** transmits unsolicited spoofed ARP replies claiming ownership of the gateway IP, routing local subnet traffic through an intercepting host.

---

### 25. Traceroute Mechanics via ICMP TTL Expiry
`traceroute` transmits probe packets with incrementing Time-to-Live values ($\text{TTL} = 1, 2, 3\dots$). Each intermediate router decrements the TTL; when $\text{TTL} = 0$, the router drops the packet and emits an **ICMP Time Exceeded (Type 11)** message, exposing its hop IP address and round-trip time.

---

### 26. Border Gateway Protocol (BGP) Routing
**BGP** is an exterior Path-Vector routing protocol managing traffic exchange between independent **Autonomous Systems (AS)** across the global internet by advertising reachable IP prefix AS paths.

---

# 4. Application Layer Protocols & Security Handshakes (Deep Dives 27-36)

### 27. HTTP Verb Idempotency Semantics
An HTTP method is **Idempotent** if executing identical requests multiple times produces the exact same server state as a single invocation:
- **Idempotent:** `GET`, `PUT`, `DELETE`, `HEAD`, `OPTIONS`.
- **Non-Idempotent:** `POST`, `PATCH`.

---

### 28. Standard HTTP Status Code Hierarchies
- **2xx (Success):** `200 OK`, `201 Created`, `204 No Content`.
- **3xx (Redirection):** `301 Moved Permanently`, `302 Found`, `304 Not Modified`.
- **4xx (Client Error):** `400 Bad Request`, `401 Unauthorized`, `403 Forbidden`, `404 Not Found`, `429 Too Many Requests`.
- **5xx (Server Error):** `500 Internal Server Error`, `502 Bad Gateway`, `503 Service Unavailable`, `504 Gateway Timeout`.

---

### 29. Authentication (401) vs. Authorization (403) Boundaries
- **`401 Unauthorized`:** Indicates unauthenticated status; credentials are missing or invalid (client can retry with valid authentication).
- **`403 Forbidden`:** Identity is verified, but the principal lacks sufficient permission to execute the action on the resource.

---

### 30. HTTP/1.1 vs. HTTP/2 Binary Multiplexing
HTTP/1.1 transmits plaintext commands sequentially per TCP socket (causing application-level Head-of-Line blocking). **HTTP/2** introduces a **Binary Framing Layer**, multiplexing hundreds of independent streams concurrently over a single TCP connection, accompanied by HPACK header compression.

---

### 31. HTTP/3 & QUIC Transport over UDP
HTTP/3 runs over **QUIC**, implementing user-space stream multiplexing over UDP. Dropping a packet on Stream A stalls only Stream A, leaving all other concurrent streams uninterrupted. It also provides **0-RTT connection resumption** and **Connection ID migration** across mobile IP handoffs.

---

### 32. TLS 1.3 Cryptographic Handshake Mechanics
1. **ClientHello & ServerHello:** Negotiate TLS version, cipher suite, and exchange ephemeral public keys (ECDHE).
2. **Authentication:** Server presents X.509 certificate validated against trusted Certificate Authority (CA) root stores.
3. **Key Derivation:** Both parties compute a shared symmetric session secret.
4. **Data Transmission:** All subsequent payloads are encrypted using symmetric ciphers (AES-256-GCM, ChaCha20).

---

### 33. Symmetric vs. Asymmetric Cryptography
- **Symmetric Cryptography:** Employs a single secret key for encryption and decryption (high hardware throughput, used for payload streams).
- **Asymmetric Cryptography:** Employs mathematically linked public and private key pairs (computationally intensive, used for digital signatures and key exchange).

---

### 34. WebSockets vs. Server-Sent Events (SSE) vs. Long Polling
- **WebSockets:** Persistent, full-duplex, bidirectional TCP framing.
- **Server-Sent Events (SSE):** Unidirectional server-to-client streaming over standard HTTP text/event-stream.
- **Long Polling:** HTTP request held open by server until data is available, closing upon response.

---

### 35. gRPC over HTTP/2 vs. REST over JSON
- **REST:** Human-readable textual JSON serialization over HTTP/1.1 or HTTP/2.
- **gRPC:** High-speed binary Protocol Buffer serialization over HTTP/2 multiplexed streams with code-generated strongly typed contracts.

---

### 36. HTTP Strict Transport Security (HSTS)
An HTTP response header (`Strict-Transport-Security: max-age=31536000; includeSubDomains`) instructing user agents to interact with the host domain exclusively via HTTPS, neutralizing SSL-stripping Man-in-the-Middle attacks.

---

# 5. Domain Resolution, Edge Caching & Perimeter Defense (Deep Dives 37-50)

### 37. Complete Hierarchical DNS Resolution Pipeline
1. Check Browser Cache $\to$ OS Hosts Cache.
2. Query Recursive DNS Resolver (e.g. `8.8.8.8`).
3. Recursive Resolver queries **Root Server (`.`)** $\to$ returns TLD Server IP.
4. Recursive Resolver queries **TLD Server (`.com`)** $\to$ returns Authoritative Server IP.
5. Recursive Resolver queries **Authoritative Server** $\to$ returns definitive `A` record IP address.

---

### 38. Core DNS Record Archetypes
- **A Record:** Maps hostname to IPv4 address.
- **AAAA Record:** Maps hostname to IPv6 address.
- **CNAME Record:** Maps alias hostname to canonical domain name.
- **MX Record:** Specifies mail exchange servers for email routing.
- **TXT Record:** Arbitrary textual metadata used for SPF, DKIM, and domain verification.

---

### 39. DNS Time-to-Live (TTL) & Cache Invalidation
**TTL** dictates the maximum duration caching resolvers retain a DNS record before re-querying authoritative servers, governing the propagation window for DNS record updates worldwide.

---

### 40. Identity State Mechanics: Cookies vs. Sessions vs. JWTs
- **Cookie:** Client browser storage string transmitted in HTTP headers.
- **Session:** Stateful server-side record (stored in Redis) indexed by an opaque session ID cookie.
- **JWT:** Stateless, self-contained, cryptographically signed JSON payload carrying identity claims.

---

### 41. Secure Token Storage & Transmission Policies
Storing authentication tokens in **`HttpOnly`, `Secure`, `SameSite=Strict` Cookies** prevents JavaScript access (mitigating XSS theft), forces TLS transport encryption, and restricts cross-origin transmission (mitigating CSRF).

---

### 42. Cross-Origin Resource Sharing (CORS) & Preflight `OPTIONS`
CORS restricts cross-origin HTTP requests in browser environments. For non-simple requests (custom headers, mutating verbs), browsers dispatch an `OPTIONS` Preflight request to verify server authorization before executing the main mutation.

---

### 43. Content Delivery Networks (CDN) & Edge Caching
CDNs operate distributed edge reverse proxies to cache static assets geographically proximate to clients, minimizing latency (TTFB) and shielding origin servers from network bursts.

---

### 44. Layer 4 vs. Layer 7 Load Balancing Architecture
- **Layer 4 (NLB):** Routes raw packets based on IP and Port headers without application inspection.
- **Layer 7 (ALB):** Terminates TLS, parses HTTP headers, cookies, and URI paths for microservice routing.

---

### 45. Forward Proxies vs. Reverse Proxies
- **Forward Proxy:** Positioned in front of clients to enforce egress filtering, masking, and security.
- **Reverse Proxy:** Positioned in front of servers to manage load balancing, TLS offloading, and caching.

---

### 46. Virtual Private Networks (VPN)
Establishes an encrypted tunnel (WireGuard, OpenVPN, IPsec) encapsulating client packets to route traffic through a remote gateway, concealing the client's public IP.

---

### 47. Distributed Denial of Service (DDoS) & DNS Amplification
A **DDoS attack** saturates system bandwidth or CPU resources via distributed botnets. **DNS Amplification** exploits open UDP DNS resolvers with spoofed source IPs to reflect $50\times$ larger DNS response payloads onto victim endpoints.

---

### 48. SSL Pinning Architecture
Hardcoding expected server public keys or certificates directly inside client binaries, rejecting connections with mismatched certificates to prevent Man-in-the-Middle attacks via untrusted root CA stores.

---

### 49. In-Band vs. Out-of-Band Network Management
- **In-Band:** Device administration over the primary production network channel (e.g. SSH over LAN).
- **Out-of-Band (OOB):** Isolated physical management paths (serial console, IPMI/iLO) enabling recovery during primary network outages.

---

### 50. End-to-End Browser Request Pipeline
$$\text{URL Parse} \to \text{DNS Query} \to \text{TCP Handshake} \to \text{TLS Handshake} \to \text{HTTP Ingress} \to \text{Backend DB} \to \text{HTTP Egress} \to \text{DOM/CSSOM Render}$$
