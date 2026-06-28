# Key Networking Protocols & Flow Controls 🌐

A high-yield guide to the core networking protocols and flow control mechanisms tested in software engineering interviews.

---

## 1. Application Layer: HTTP Evolution 🚀

| Feature | HTTP/1.1 (1997) | HTTP/2 (2015) | HTTP/3 (2020) |
| :--- | :--- | :--- | :--- |
| **Transport Protocol** | TCP | TCP | UDP (via QUIC) |
| **Connection Model** | Persistent connections; Head-of-Line (HoL) blocking on request level | Multiplexing over single TCP connection; HoL blocking on TCP packet level | Multiplexing over UDP; Zero HoL blocking (independent streams) |
| **Format** | Text-based (human readable) | Binary frames (faster parsing) | Binary frames |
| **Header Compression** | ❌ None | HPACK (static/dynamic tables) | QPACK (handles out-of-order streams) |
| **Server Push** | ❌ No | ✅ Yes (server pushes assets ahead) | ✅ Yes |

### Head-of-Line (HoL) Blocking Explained
* **HTTP/1.1 HoL:** A client can only send one request at a time per TCP connection. If the first request is slow, all subsequent requests are blocked. (Mitigated by browsers opening up to 6 parallel connections).
* **HTTP/2 HoL:** Solves HTTP/1.1 HoL by multiplexing multiple streams over one TCP connection. However, if a single TCP packet is lost, the entire connection halts until the packet is retransmitted (TCP-level HoL blocking).
* **HTTP/3 (QUIC) HoL:** Replaces TCP with UDP. Each stream is handled independently at the transport layer. If a packet in stream A is lost, stream B continues uninterrupted.

---

## 2. HTTPS & TLS Handshake (TLS 1.2 vs 1.3) 🔒

HTTPS is HTTP encrypted inside **TLS (Transport Layer Security)**.

```
TLS 1.2 Handshake (2 RTT - Round Trip Times)
Client                                               Server
  | ----------------- ClientHello -------------------> |
  | <---- ServerHello + Certificate + KeyExchange ---- | (RTT 1)
  | ----------------- KeyExchange + Finished --------> |
  | <---------------- Finished ----------------------- | (RTT 2)
  | <================ Encrypted Data ================> |
```

```
TLS 1.3 Handshake (1 RTT)
Client                                               Server
  | ----- ClientHello (with Key Share Guess) ---------> |
  | <---- ServerHello + Certificate + Finished ------- | (RTT 1)
  | <================ Encrypted Data ================> |
```

### TLS 1.3 Optimization: Zero-RTT (0-RTT)
For returning clients, the client uses cached session keys to encrypt and send data directly in the first message (`ClientHello`), resulting in **0 network delay** for connection setup.

---

## 3. DNS (Domain Name System) Resolution Flow 🔍

DNS translates human-readable hostnames (e.g., `google.com`) into machine-readable IP addresses.

```
Client ──► 1. Recursive Resolver ──► 2. Root Nameserver (.)
                                 ◄── Returns TLD Server (.com)
       ──► 3. TLD Nameserver     ──► Returns Authoritative Server
       ──► 4. Authoritative Nameserver (google.com) ──► Returns IP (1.2.3.4)
```

### Recursive vs. Iterative Queries
* **Recursive Query:** The client asks the resolver (e.g., Google DNS 8.8.8.8) to do all the work and return the final IP or an error.
* **Iterative Query:** The resolver queries nameservers sequentially. The nameserver doesn't resolve the query itself, but returns the address of the next-level nameserver to query.

---

## 4. Transport Layer: TCP Flow & Congestion Control ⚙️

### A. Flow Control (Sliding Window)
Prevents the sender from overwhelming the receiver's memory buffer.
* The receiver advertises its available buffer space using the **Receiver Window (`rwnd`)** header field in ACK packets.
* The sender guarantees that the size of unacknowledged data never exceeds `rwnd`.

---

### B. Congestion Control
Prevents the sender from overwhelming the network infrastructure (routers/switches). The sender maintains a **Congestion Window (`cwnd`)**.

The state machine operates in 4 phases:

```
  Window Size (cwnd)
     ^
     |         / \
     |        /   \  <-- Congestion Event (Packet loss/Timeout)
     |       /     \
     |  __  /       \_____ (Congestion Avoidance - linear growth)
     | /  \/
     |/ (Slow Start - exponential growth)
     +-----------------------------------> Time
```

1. **Slow Start:** Starts with `cwnd = 1` MSS (Maximum Segment Size). Double the `cwnd` every RTT (exponential growth) until it reaches the Slow Start Threshold (`ssthresh`).
2. **Congestion Avoidance:** Once `cwnd >= ssthresh`, grow `cwnd` by exactly 1 MSS per RTT (linear growth) to probe network limits safely.
3. **Fast Retransmit:** If the sender receives **3 duplicate ACKs** for a packet, it immediately assumes the packet was lost and retransmits it without waiting for the retransmission timeout (RTO).
4. **Fast Recovery:** Instead of resetting `cwnd` to 1 upon 3 duplicate ACKs, halve `cwnd` (`cwnd = cwnd / 2`), set `ssthresh = cwnd`, and resume Congestion Avoidance (linear growth).
