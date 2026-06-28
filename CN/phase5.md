# Phase 5: Web Communication & Security 🌐

This report covers modern web communication protocols, encryption, caching, and common interview questions.

---

## 1. REST APIs (Representational State Transfer)

REST is an architectural style for designing networked applications using HTTP. It relies on a client-server architecture and stateless communication.

### Core Constraints
1. **Client-Server:** Decouples user-interface concerns from data storage concerns.
2. **Stateless:** Each request from a client must contain all information needed to process it. The server stores no session context.
3. **Cacheable:** Server responses must define themselves as cacheable or non-cacheable to reduce load times.
4. **Uniform Interface:** Simplifies architecture through standardized resource URIs (e.g., `/api/v1/users`), standard HTTP methods, and representation formatting (typically JSON).

---

## 2. WebSockets

WebSockets provide a full-duplex, persistent communication channel over a single TCP connection.

```
Client                                                   Server
  │                                                        │
  │ ──► HTTP GET (Upgrade: websocket) ───────────────────► │ (WebSocket Handshake)
  │ ◄── HTTP 101 Switching Protocols ────────────────────  │
  │                                                        │
  │ ◄================= Bidirectional ====================► │ (Persistent TCP Frame Stream)
  │                                                        │
```

* **Handshake:** The connection begins as a standard HTTP/1.1 request containing headers requesting an upgrade: `Connection: Upgrade` and `Upgrade: websocket`. The server responds with `HTTP 101 Switching Protocols` to switch protocols.
* **Frames:** Once established, communication shifts to low-overhead WebSocket frames, allowing real-time, low-latency streaming without HTTP request/response headers.
* **Use Cases:** Chat applications, real-time dashboards, multiplayer gaming, financial ticker updates.

---

## 3. TLS/SSL Basics

TLS (Transport Layer Security) secures communications by combining asymmetric and symmetric cryptography.

```
TLS 1.2 Handshake (Simplifed)
1. Client Sends Hello (Supported cipher suites, random bytes)
2. Server Sends Hello + Certificate (Contains server public key)
3. Client verifies certificate with trusted CA root keys
4. Client generates pre-master secret key, encrypts it with server public key, sends it
5. Server decrypts it with its private key
6. Both derive identical session keys (Symmetric encryption begins)
```

* **Asymmetric Key Exchange:** The client uses the server's public key (retrieved from the SSL Certificate) to negotiate a shared session key.
* **Symmetric Encryption:** Once the session key is agreed upon, all subsequent message exchanges are encrypted using this key (e.g., AES).
* **CA (Certificate Authority):** A trusted third party (e.g., Let's Encrypt) that digitally signs server public keys, preventing man-in-the-middle impersonations.

---

## 4. CDN Basics (Content Delivery Network)

A CDN is a geographically distributed network of proxy servers designed to cache static assets (images, video, JavaScript) closer to users.

* **Caching at the Edge:** When a user requests an asset, DNS routing directs the query to the topologically nearest edge server. If the asset is cached locally (cache hit), it is returned immediately, reducing latency.
* **Origin Server Protection:** CDNs reduce traffic hitting the primary backend servers (origin servers), protecting them from load spikes and DDoS attacks.

---

## 5. Common Networking Interview Scenarios

### Q1. What is the difference between latency, bandwidth, and throughput?
* **Latency:** The time delay for a single data packet to travel from source to destination (measured in milliseconds).
* **Bandwidth:** The maximum theoretical capacity of a link (e.g., a 100 Mbps fiber line).
* **Throughput:** The actual rate of data successfully delivered over the network (e.g., due to congestion, packet loss, or protocol overhead, a 100 Mbps link may yield only 70 Mbps throughput).

### Q2. What is the difference between TCP Keep-Alive and HTTP Keep-Alive?
* **HTTP Keep-Alive:** An application-layer directive (`Connection: keep-alive`) telling the server to keep the TCP connection open for subsequent requests, avoiding handshake delays for website assets.
* **TCP Keep-Alive:** A transport-layer socket feature. If a connection is idle, the OS sends small empty probe packets to verify the remote host is still active and hasn't disconnected silently.

### Q3. Explain the difference between CORS (Cross-Origin Resource Sharing) and Same-Origin Policy (SOP).
* **Same-Origin Policy:** A browser security mechanism that restricts documents or scripts loaded from one origin (protocol + domain + port) from interacting with resources from another origin.
* **CORS:** A system of HTTP headers that relaxes SOP, allowing servers to declare which origins are authorized to fetch their data (e.g., `Access-Control-Allow-Origin: https://client.example.com`).

### Q4. What is a SYN Flood attack? How do SYN Cookies mitigate it?
* **Attack:** A DoS attack where the client floods the server with `SYN` requests but never responds to the `SYN-ACK` responses. This leaves the server's queue holding half-open connections, exhausting connection resources.
* **Mitigation (SYN Cookies):** The server does not allocate buffer queue memory for the connection immediately. Instead, it encodes connection state into the `SYN-ACK` sequence number (a cookie). When the client returns the final `ACK`, the server validates the sequence number and allocates memory for the connection only then.
