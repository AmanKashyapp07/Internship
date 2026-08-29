# Master Guide 03: Top 50 Computer Networks Spoken Flashcards & Trap Questions

> **Focus:** 50 High-Yield Spoken Flashcards for Computer Networks interviews at Google, Meta, Amazon, Microsoft, Uber, Cloudflare, and High-Frequency Trading (HFT) firms.
> 
> *The 15-minute complete verbal drill to read one day before any tech interview.*

---

# Table of Contents
1. [OSI & TCP/IP Layer Foundations (Cards 1–8)](#1-osi--tcpip-layer-foundations-cards-18)
2. [TCP, UDP & Transport Layer Internals (Cards 9–18)](#2-tcp-udp--transport-layer-internals-cards-918)
3. [IP Addressing, Routing & Network Layer (Cards 19–26)](#3-ip-addressing-routing--network-layer-cards-1926)
4. [Application Protocols: HTTP, HTTPS, WebSockets & TLS (Cards 27–36)](#4-application-protocols-http-https-websockets--tls-cards-2736)
5. [DNS, Security, Caching & Applied Networking (Cards 37–50)](#5-dns-security-caching--applied-networking-cards-3750)

---

# 1. OSI & TCP/IP Layer Foundations (Cards 1–8)

### Card 1: "What is the practical difference between the OSI model and the TCP/IP model?"
> **Spoken Answer:** The **OSI model** is a theoretical 7-layer conceptual framework. The **TCP/IP model** is the practical 4-layer architecture the internet actually uses, collapsing OSI Application, Presentation, and Session layers into a single **Application Layer**, followed by **Transport, Internet (Network), and Link (Network Access)** layers.

---

### Card 2: "What is Packet Encapsulation and Decapsulation across network layers?"
> **Spoken Answer:** As data moves down the stack, each layer wraps the payload from the layer above with its own header: **Application Data $\to$ Transport Segment (adds port header) $\to$ Network Packet (adds IP header) $\to$ Link Frame (adds MAC header + trailer)**. At the receiver, each layer strips its corresponding header in reverse order (Decapsulation).

---

### Card 3: "What is the difference between an IP Address and a MAC Address?"
> **Spoken Answer:** An **IP Address (Layer 3)** is a logical, hierarchical, and globally routable address used to route packets across disparate networks. A **MAC Address (Layer 2)** is a physical, 48-bit hardware identifier burned into the Network Interface Card (NIC) used to deliver frames locally between devices on the same physical link.

---

### Card 4: "What is the difference between a Hub, a Switch, and a Router?"
> **Spoken Answer:**
> - **Hub (Layer 1):** Dumb physical repeater that blindly broadcasts incoming electrical signals to all connected ports.
> - **Switch (Layer 2):** Intelligent device that inspects MAC addresses and forwards frames only to the specific destination port using an internal MAC address table.
> - **Router (Layer 3):** Network gateway that inspects destination IP addresses and routes packets across different networks using routing tables.

---

### Card 5: "What is a Port Number and why is it needed?"
> **Spoken Answer:** A **Port Number** is a 16-bit integer ($0\text{--}65535$) at the Transport Layer that directs network data to a **specific process or service running on a host**. Well-known ports ($0\text{--}1023$) include HTTP (80), HTTPS (443), SSH (22), and DNS (53).

---

### Card 6: "What is a Network Socket?"
> **Spoken Answer:** A **Socket** is an endpoint software abstraction for network I/O, uniquely identified by the 5-tuple: **`(Source IP, Source Port, Destination IP, Destination Port, Protocol)`**.

---

### Card 7: "What is the difference between Unicast, Broadcast, Multicast, and Anycast?"
> **Spoken Answer:**
> - **Unicast:** One-to-one transmission from a single sender to a single specific destination.
> - **Broadcast:** One-to-all transmission to every device on the local network segment.
> - **Multicast:** One-to-many transmission to an interested group of subscribed hosts.
> - **Anycast:** One-to-nearest transmission where routing infrastructure routes the packet to the geographically closest server sharing the same IP address (used by DNS root servers and Cloudflare CDNs).

---

### Card 8: "What is Maximum Transmission Unit (MTU) and IP Fragmentation?"
> **Spoken Answer:** **MTU** is the maximum size of a packet (typically 1500 bytes on standard Ethernet) that can be transmitted over a physical link without being broken up. If a packet exceeds the MTU of an intermediate router and the "Don't Fragment" (DF) bit is not set, the router performs **IP Fragmentation**, splitting the packet into smaller fragments reassembled at the final destination.

---

# 2. TCP, UDP & Transport Layer Internals (Cards 9–18)

### Card 9: "What are the core tradeoffs between TCP and UDP?"
> **Spoken Answer:** **TCP** is connection-oriented, reliable, and guarantees in-order byte stream delivery via sequence numbers, ACKs, flow control, and congestion control at the expense of latency. **UDP** is connectionless, lightweight (8-byte header vs. 20-60 bytes in TCP), and transmits unordered datagrams with zero delivery guarantees, maximizing speed for real-time video streaming, gaming, and DNS.

---

### Card 10: "Explain the TCP 3-Way Handshake in 15 seconds."
> **Spoken Answer:**
> 1. **Client sends SYN (seq = $x$):** Client requests connection.
> 2. **Server responds with SYN-ACK (seq = $y$, ack = $x+1$):** Server acknowledges client and requests reverse synchronization.
> 3. **Client sends ACK (ack = $y+1$):** Client acknowledges server. Connection is now **ESTABLISHED**.

---

### Card 11: "Why is a 2-way handshake insufficient for TCP?"
> **Spoken Answer:** A 2-way handshake allows the client to know the server can receive and send, but **the server has zero confirmation that the client received its SYN-ACK**. Without the final ACK, old delayed duplicate SYN packets could cause the server to allocate half-open connection resources for phantom clients.

---

### Card 12: "Explain the TCP 4-Way Connection Teardown."
> **Spoken Answer:**
> 1. Client sends **FIN** (no more data to send).
> 2. Server sends **ACK** (closes client-to-server direction; server can still send data).
> 3. Server sends **FIN** (server is done sending data).
> 4. Client sends **ACK** and enters `TIME_WAIT` state. Server closes immediately.

---

### Card 13: "Why does the TCP client enter the `TIME_WAIT` state for $2\times\text{MSL}$?"
> **Spoken Answer:** To ensure the final **ACK reaches the server** (resending it if lost so the server doesn't remain stuck in `LAST_ACK`), and to ensure all lingering duplicate packets from the connection drain from the network before a new connection reuses the same port.

---

### Card 14: "What is the difference between TCP Flow Control and Congestion Control?"
> **Spoken Answer:** **Flow Control** prevents the sender from overwhelming the *receiver's receive buffer* using a dynamic **Receive Window (`rwnd`)**. **Congestion Control** prevents the sender from overwhelming the *intermediate network routers* using algorithms that dynamically adjust the **Congestion Window (`cwnd`)**.

---

### Card 15: "How does TCP Congestion Control work (Slow Start & AIMD)?"
> **Spoken Answer:** TCP starts in **Slow Start**, doubling `cwnd` exponentially every Round-Trip Time (RTT) until reaching `ssthresh`. It then enters **Congestion Avoidance**, increasing `cwnd` linearly (+1 MSS per RTT). Upon packet loss, it cuts `cwnd` in half (**Additive Increase, Multiplicative Decrease / AIMD**) or resets to 1 on timeout.

---

### Card 16: "What is Head-of-Line (HoL) Blocking in TCP?"
> **Spoken Answer:** Because TCP guarantees strict in-order byte stream delivery, if a single packet is lost in transit, **all subsequent packets that arrived successfully must wait in the buffer** until the missing packet is retransmitted and acknowledged, stalling application processing.

---

### Card 17: "What is a SYN Flood attack and how do SYN Cookies defend against it?"
> **Spoken Answer:** A SYN Flood sends millions of spoofed SYN packets without completing the 3-way handshake, exhausting the server's connection backlog table. **SYN Cookies** eliminate the backlog table: the server encodes connection state cryptographically into the initial sequence number ($y$) of the SYN-ACK and allocates memory only when the valid final ACK containing $y+1$ returns.

---

### Card 18: "What is the difference between TCP Keep-Alive and HTTP Keep-Alive?"
> **Spoken Answer:** **TCP Keep-Alive** is an OS-level transport probe that sends empty packets on an idle connection to detect if the remote host crashed or lost connectivity. **HTTP Keep-Alive (Persistent Connection)** is an application-level header allowing multiple HTTP requests/responses to reuse a single open TCP connection, avoiding repeated 3-way handshake overhead.

---

# 3. IP Addressing, Routing & Network Layer (Cards 19–26)

### Card 19: "What is the difference between IPv4 and IPv6?"
> **Spoken Answer:** **IPv4** uses 32-bit addresses (~4.3 billion total, written in dotted decimal `192.168.1.1`) and requires NAT due to address exhaustion. **IPv6** uses 128-bit addresses ($3.4 \times 10^{38}$ total, written in hexadecimal `2001:db8::1`), eliminating the need for NAT, simplifying router headers, and integrating IPsec natively.

---

### Card 20: "What is CIDR notation and subnetting (e.g. `/24`)?"
> **Spoken Answer:** Classless Inter-Domain Routing (CIDR) uses a prefix `/N` to denote the number of fixed network bits, leaving $32 - N$ bits for host addresses. A `/24` subnet has $32 - 24 = 8$ host bits ($2^8 = 256$ total IP addresses), providing **254 usable host addresses** after reserving the Network ID (`.0`) and Broadcast address (`.255`).

---

### Card 21: "What are the Private IP Address ranges (RFC 1918)?"
> **Spoken Answer:** Non-routable private ranges reserved for local networks:
> - Class A: `10.0.0.0` to `10.255.255.255` (`10.0.0.0/8`)
> - Class B: `172.16.0.0` to `172.31.255.255` (`172.16.0.0/12`)
> - Class C: `192.168.0.0` to `192.168.255.255` (`192.168.0.0/16`)

---

### Card 22: "What is NAT (Network Address Translation) and PAT (Port Address Translation)?"
> **Spoken Answer:** **NAT** maps private local IP addresses to a single public IP address. **PAT (NAT Overload)** maps thousands of internal private IP + port combinations to distinct temporary source ports on a single public IP address, allowing an entire corporate office or home Wi-Fi network to share one public IP.

---

### Card 23: "How does DHCP work (The DORA Process)?"
> **Spoken Answer:**
> 1. **Discover:** Client broadcasts a DHCPDISCOVER packet searching for a DHCP server.
> 2. **Offer:** DHCP server broadcasts a DHCPOFFER with a reserved IP address.
> 3. **Request:** Client broadcasts a DHCPREQUEST accepting the offered IP lease.
> 4. **Acknowledge:** DHCP server sends a DHCPACK confirming the lease, subnet mask, default gateway, and DNS servers.

---

### Card 24: "What is ARP (Address Resolution Protocol) and ARP Poisoning?"
> **Spoken Answer:** **ARP** resolves a known Layer 3 IP address to a physical Layer 2 MAC address on the local network link by broadcasting an "ARP Request" and receiving an "ARP Reply". **ARP Poisoning (Spoofing)** is a Man-in-the-Middle attack where a malicious host sends fake unsolicited ARP replies claiming to own the Default Gateway's IP, redirecting local traffic through the attacker's machine.

---

### Card 25: "How does `traceroute` discover the network hops to a destination?"
> **Spoken Answer:** `traceroute` sends packets with incrementing **Time-To-Live (TTL)** values starting at $\text{TTL} = 1$. Each intermediate router decrements the TTL by 1. When $\text{TTL} = 0$, the router drops the packet and returns an **ICMP Time Exceeded** message, exposing the router's IP address and round-trip latency for that hop until the destination is reached.

---

### Card 26: "What is BGP (Border Gateway Protocol)?"
> **Spoken Answer:** **BGP** is the de facto Path-Vector routing protocol that glues the global internet together, routing traffic between independent **Autonomous Systems (AS)** (large ISPs, Google, Amazon) by advertising reachable IP prefix paths.

---

# 4. Application Protocols: HTTP, HTTPS, WebSockets & TLS (Cards 27–36)

### Card 27: "Which HTTP methods are Idempotent and what does idempotency mean?"
> **Spoken Answer:** An HTTP method is **Idempotent** if executing it multiple identical times produces the exact same server state as executing it once.
> - **Idempotent:** `GET`, `PUT` (replaces whole resource), `DELETE` (deleting already deleted row leaves state identical), `HEAD`, `OPTIONS`.
> - **Non-Idempotent:** `POST` (creates multiple duplicate resources), `PATCH` (incremental append mutations).

---

### Card 28: "What are the common HTTP status code ranges?"
> **Spoken Answer:**
> - **2xx (Success):** `200 OK`, `201 Created`, `204 No Content`.
> - **3xx (Redirection):** `301 Moved Permanently`, `302 Found`, `304 Not Modified` (cached).
> - **4xx (Client Error):** `400 Bad Request`, `401 Unauthorized` (unauthenticated), `403 Forbidden` (no permission), `404 Not Found`, `429 Too Many Requests`.
> - **5xx (Server Error):** `500 Internal Error`, `502 Bad Gateway` (upstream dead), `503 Unavailable`, `504 Gateway Timeout` (upstream slow).

---

### Card 29: "What is the difference between `401 Unauthorized` and `403 Forbidden`?"
> **Spoken Answer:** **`401 Unauthorized`** means the client is **unauthenticated** (missing or invalid credentials; client can retry with a valid token). **`403 Forbidden`** means the server knows who you are, but you **lack authorization/permissions** to access the resource (retrying with identical credentials will fail).

---

### Card 30: "What is the difference between HTTP/1.1 and HTTP/2?"
> **Spoken Answer:** HTTP/1.1 transfers plaintext and allows one request/response at a time per TCP connection (causing Head-of-Line blocking). **HTTP/2 uses binary framing and multiplexing**, allowing hundreds of concurrent bidirectional streams over a **single TCP connection**, and adds header compression (HPACK) and Server Push.

---

### Card 31: "Why does HTTP/3 use QUIC over UDP instead of TCP?"
> **Spoken Answer:** In HTTP/2 over TCP, a single lost packet blocks all multiplexed streams in the TCP buffer (TCP Head-of-Line blocking). **HTTP/3 runs over QUIC (UDP)**, isolating stream packet loss so losing a packet on Stream A does not stall Stream B, and enables **0-RTT connection resumption** and seamless connection migration across Wi-Fi/cellular IP changes.

---

### Card 32: "How does the TLS Handshake establish a secure HTTPS connection?"
> **Spoken Answer:**
> 1. **ClientHello & ServerHello:** Agree on TLS version and cipher suite.
> 2. **Certificate Verification (Asymmetric):** Server presents its digital certificate; client verifies authenticity against trusted root CAs using public-key cryptography.
> 3. **Key Exchange (Asymmetric):** Client and server use ECDHE (Elliptic Curve Diffie-Hellman) to compute a shared **Symmetric Session Key**.
> 4. **Encrypted Session (Symmetric):** All subsequent HTTP traffic is encrypted using fast symmetric ciphers (AES-GCM).

---

### Card 33: "What is the difference between Symmetric and Asymmetric Encryption?"
> **Spoken Answer:** **Symmetric Encryption** uses the exact same shared secret key for encryption and decryption (ultra-fast, used for bulk data transfer). **Asymmetric Encryption** uses a mathematically linked Public Key (encrypt/verify) and Private Key (decrypt/sign) pair (computationally expensive, used for initial authentication and key exchange).

---

### Card 34: "Compare WebSockets, Long Polling, and Server-Sent Events (SSE)."
> **Spoken Answer:**
> - **WebSockets:** Full-duplex, bidirectional, persistent TCP connection over a single socket (best for chat, multiplayer gaming).
> - **SSE (Server-Sent Events):** Unidirectional server-to-client streaming over standard HTTP (best for live stock tickers, news feeds).
> - **Long Polling:** Client opens HTTP request; server holds request open until data is ready, responds, and closes connection (inefficient legacy fallback).

---

### Card 35: "What is gRPC and how does it compare to REST?"
> **Spoken Answer:** **REST** uses HTTP/1.1 with human-readable JSON payloads, making it flexible but text-heavy. **gRPC** uses **HTTP/2 transport with Protocol Buffers (Protobuf)**, encoding payloads as ultra-compact binary streams with strict schema code generation, resulting in **$5\text{--}10\times$ faster serialization and lower network latency** for microservice communication.

---

### Card 36: "What is HSTS (HTTP Strict Transport Security)?"
> **Spoken Answer:** An HTTP response header (`Strict-Transport-Security: max-age=31536000; includeSubDomains`) that forces browsers to communicate with the domain **exclusively over HTTPS**, preventing SSL-stripping Man-in-the-Middle attacks on initial plaintext redirects.

---

# 5. DNS, Security, Caching & Applied Networking (Cards 37–50)

### Card 37: "Walk through the full DNS Resolution Hierarchy."
> **Spoken Answer:**
> 1. Check local **Browser Cache** $\to$ **OS Hosts Cache**.
> 2. Query **ISP Recursive Resolver** (`8.8.8.8`).
> 3. Resolver queries **Root Name Server (`.`)** $\to$ returns TLD Server IP.
> 4. Resolver queries **TLD Name Server (`.com`)** $\to$ returns Authoritative Server IP.
> 5. Resolver queries **Authoritative Name Server (`google.com`)** $\to$ returns exact A-record IP address (`142.250.x.x`).

---

### Card 38: "What are the common DNS Record Types (A, AAAA, CNAME, MX)?"
> **Spoken Answer:**
> - **A Record:** Maps a domain to an **IPv4 address** (`example.com -> 93.184.216.34`).
> - **AAAA Record:** Maps a domain to an **IPv6 address**.
> - **CNAME Record:** Maps an alias domain to a canonical domain (`www.example.com -> example.com`).
> - **MX Record:** Directs incoming domain emails to mail servers.
> - **TXT Record:** Stores arbitrary text, used for domain ownership verification and email security (SPF, DKIM).

---

### Card 39: "What is DNS TTL and what happens during a DNS propagation delay?"
> **Spoken Answer:** **TTL (Time to Live)** defines how long recursive DNS resolvers are permitted to cache a DNS record. When updating an A-record, resolvers worldwide continue serving the old cached IP until their local TTL countdown expires, causing DNS propagation delays.

---

### Card 40: "Compare Cookies, Server Sessions, and JWTs."
> **Spoken Answer:**
> - **Cookie:** Client browser storage (max 4KB) sent automatically in HTTP request headers.
> - **Session:** Stateful server-side session object (in Redis) referenced by a unique `session_id` cookie.
> - **JWT:** Stateless, client-side, cryptographically signed JSON token carrying user identity claims directly in its payload.

---

### Card 41: "How do you securely store and transmit JWTs to prevent XSS and CSRF?"
> **Spoken Answer:** Store the JWT inside an **`HttpOnly`, `Secure`, `SameSite=Strict` Cookie**. `HttpOnly` blocks JavaScript access (preventing XSS credential theft); `Secure` enforces HTTPS transmission; `SameSite=Strict` prevents the browser from sending the cookie in cross-site requests (mitigating CSRF).

---

### Card 42: "What is CORS (Cross-Origin Resource Sharing) and what is a Preflight request?"
> **Spoken Answer:** CORS is a browser security policy restricting web pages on Domain A from making AJAX requests to Domain B. For non-simple requests (e.g. `PUT`, `DELETE`, or custom headers), the browser automatically sends an **`OPTIONS` Preflight request** asking Domain B if the origin, method, and headers are allowed before sending the actual request.

---

### Card 43: "What is a Content Delivery Network (CDN) and what is Edge Caching?"
> **Spoken Answer:** A **CDN** is a globally distributed network of Point of Presence (PoP) edge proxy servers that cache static assets (images, video, JS/CSS) geographically close to end users, slashing network latency (TTFB) and shielding origin database servers from traffic spikes.

---

### Card 44: "What is the difference between Layer 4 and Layer 7 Load Balancing?"
> **Spoken Answer:** **Layer 4 Load Balancing (NLB)** routes raw packets based only on IP and Port without inspecting application data (ultra-fast, millions of connections). **Layer 7 Load Balancing (ALB)** parses full HTTP requests, enabling intelligent routing based on URL paths (`/api` vs `/static`), hostnames, HTTP headers, and cookies.

---

### Card 45: "What is the difference between a Forward Proxy and a Reverse Proxy?"
> **Spoken Answer:** A **Forward Proxy** acts on behalf of *clients* to access the internet (hiding client IP addresses or filtering corporate web access). A **Reverse Proxy** acts on behalf of *servers*, sitting in front of web applications to handle load balancing, TLS termination, caching, and rate limiting.

---

### Card 46: "What is a VPN (Virtual Private Network)?"
> **Spoken Answer:** A **VPN** creates an encrypted tunnel between a client device and a remote VPN server (using protocols like WireGuard or OpenVPN), encrypting all network traffic and masking the client's public IP address with the VPN server's IP.

---

### Card 47: "What is a DDoS attack and what is a DNS Amplification attack?"
> **Spoken Answer:** A **Distributed Denial of Service (DDoS)** floods a server with millions of requests from distributed botnets to exhaust CPU, RAM, or bandwidth. A **DNS Amplification attack** sends small spoofed DNS queries with the victim's source IP to open DNS resolvers, causing resolvers to flood the victim with $50\times$ larger DNS response payloads over UDP.

---

### Card 48: "What is SSL Pinning in mobile applications?"
> **Spoken Answer:** Hardcoding the server's specific SSL certificate or public key directly inside the client mobile app. The app rejects any connection whose certificate does not match the pinned key, preventing attackers from intercepting traffic using compromised user-installed root CA certificates.

---

### Card 49: "What is the difference between In-Band and Out-of-Band Network Management?"
> **Spoken Answer:** **In-Band** management administers network devices over the primary production network channel (e.g. SSH over production LAN). **Out-of-Band (OOB)** uses an isolated, dedicated secondary network connection (e.g. serial console or IPMI/iLO), allowing engineers to recover crashed network hardware even when the main network is down.

---

### Card 50: "What happens when you type a URL into a browser and press Enter (The Canonical 60-Second Master Answer)?"
> **Spoken Answer:**
> 1. **DNS Lookup:** Browser checks caches, then queries DNS resolvers to translate the domain into an IP address.
> 2. **TCP Handshake:** Client sends `SYN`, receives `SYN-ACK`, returns `ACK` to open a TCP connection on port 443.
> 3. **TLS Handshake:** Client verifies the server's SSL certificate and negotiates a symmetric session key.
> 4. **HTTP Request:** Client transmits an encrypted `GET /` request over the TLS tunnel.
> 5. **Load Balancer & Server Processing:** Reverse proxy routes request to application server; server queries database and renders HTML.
> 6. **HTTP Response:** Server returns `200 OK` with response headers and payload.
> 7. **Browser Rendering:** Browser parses HTML into DOM tree, parses CSS into CSSOM tree, computes Layout, and paints pixels onto the screen.
