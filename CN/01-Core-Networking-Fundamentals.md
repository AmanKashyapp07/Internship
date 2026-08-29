# Master Guide 01: Core Networking Fundamentals

> **Focus:** OSI vs. TCP/IP Models, TCP vs. UDP Decision Matrix, TCP 3-Way Handshake & 4-Way Teardown, HTTP vs. HTTPS (TLS Key Exchange), and DNS Resolution Workflow.
> 
> *Targeted for Top-Tier Tech Internship & Software Engineering Interviews.*

---

# Table of Contents
1. [OSI vs. TCP/IP Model: The Practical Mapping](#1-osi-vs-tcpip-model-the-practical-mapping)
2. [TCP vs. UDP: The Master Decision Matrix](#2-tcp-vs-udp-the-master-decision-matrix)
3. [TCP Connection Lifecycle: 3-Way Handshake & 4-Way Teardown](#3-tcp-connection-lifecycle-3-way-handshake--4-way-teardown)
4. [HTTP vs. HTTPS & TLS Handshake Mechanics](#4-http-vs-https--tls-handshake-mechanics)
5. [DNS Resolution: From Domain Name to IP Address](#5-dns-resolution-from-domain-name-to-ip-address)
6. [High-Frequency Interview Drill & Verbal Q&A](#6-high-frequency-interview-drill--verbal-qa)

---

# 1. OSI vs. TCP/IP Model: The Practical Mapping

```
+--------------------------+--------------------------+------------------------------------------------+
| OSI 7-LAYER MODEL        | TCP/IP 4-LAYER MODEL     | COMMON PROTOCOLS & DATA UNITS                  |
+--------------------------+--------------------------+------------------------------------------------+
| 7. Application Layer     |                          | HTTP, HTTPS, DNS, SSH, FTP, SMTP, WebSockets   |
| 6. Presentation Layer    | Application Layer        | (Data / Message Payload)                       |
| 5. Session Layer         |                          |                                                |
+--------------------------+--------------------------+------------------------------------------------+
| 4. Transport Layer       | Transport Layer          | TCP (Segments), UDP (Datagrams), Ports         |
+--------------------------+--------------------------+------------------------------------------------+
| 3. Network Layer         | Internet / Network Layer | IP (IPv4, IPv6), ICMP, Routers (Packets)       |
+--------------------------+--------------------------+------------------------------------------------+
| 2. Data Link Layer       | Link / Network Interface | Ethernet, Wi-Fi (MAC Addresses, Frames)        |
| 1. Physical Layer        | Layer                    | Cables, Fiber, NIC Transceivers (Bits)         |
+--------------------------+--------------------------+------------------------------------------------+
```

- **One-Line Intuition:** OSI is the theoretical 7-layer textbook blueprint; TCP/IP is the practical 4-layer architecture the actual internet runs on.
- **The Interview Trap:** Trying to memorize obscure details of Presentation and Session layers. In real production software, layers 5, 6, and 7 are all collapsed into the user-space **Application Layer**.
- **30-Second Verbal Answer:** **"The TCP/IP model has 4 layers: Application for user-level protocols like HTTP and DNS, Transport for end-to-end process communication via TCP or UDP, Network for host-to-host routing via IP, and Link for physical frame transmission over Ethernet or Wi-Fi."**
- **Follow-up:** *What layer does a router operate on vs. a switch?* **A standard network switch operates on Layer 2 (Data Link / MAC addresses), while a router operates on Layer 3 (Network / IP addresses).**

---

# 2. TCP vs. UDP: The Master Decision Matrix

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | TCP (Transmission Control Protocol)   | UDP (User Datagram Protocol)       |
+---------------------------------------------------------------------------------------------------+
| Connection Type      | Connection-Oriented (3-way handshake) | Connectionless (No setup overhead) |
| Reliability          | 100% Reliable (ACKs + Retransmission) | Best-Effort (Packets can be lost)  |
| Ordering             | Guaranteed In-Order (Sequence numbers)| Unordered (Packets arrive randomly)|
| Flow/Congestion Ctrl | YES (Sliding Window, Backoff, AIMD)   | NO (Sends at application's rate)   |
| Header Overhead      | 20–60 Bytes                           | 8 Bytes (Ultra-lightweight)        |
| Speed / Latency      | Slower (Handshake + ACK roundtrips)   | Fastest (Zero handshake latency)   |
| Primary Use Cases    | Web (HTTP/HTTPS), File transfers      | Video Streaming (Zoom), Gaming,    |
|                      | (FTP), Emails (SMTP), SSH, Databases  | DNS Lookups, VoIP, Live Broadcasts |
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** TCP is a certified registered letter requiring a signature confirmation; UDP is shouting across a crowded room—fast, but some words might get lost.
- **The Interview Trap:** Saying "UDP is bad because it is unreliable." UDP is intentionally designed without retransmission overhead because in live video or online gaming, an old delayed frame is completely useless; **freshness and low latency beat guaranteed arrival**.
- **30-Second Verbal Answer:** **"TCP is a connection-oriented, reliable protocol that guarantees in-order byte stream delivery using sequence numbers, acknowledgements, and congestion control at the cost of latency. UDP is connectionless and lightweight, sending datagrams with zero setup overhead and no delivery guarantees, making it ideal for real-time video, gaming, and DNS."**

---

# 3. TCP Connection Lifecycle: 3-Way Handshake & 4-Way Teardown

```
TCP 3-Way Handshake (Establishment):
[ Client ]                                                        [ Server ]
    | --- 1. SYN (seq=x) --------------------------------------------> | (Client: "Can you hear me?")
    | <--- 2. SYN-ACK (seq=y, ack=x+1) ------------------------------- | (Server: "Yes, can you hear me?")
    | --- 3. ACK (ack=y+1) ------------------------------------------> | (Client: "Yes, connection open!")
    |                                                                  |
    [ ESTABLISHED ]                                                    [ ESTABLISHED ]

TCP 4-Way Handshake (Termination):
[ Client ]                                                        [ Server ]
    | --- 1. FIN (seq=u) --------------------------------------------> | (Client: "I have no more data to send")
    | <--- 2. ACK (ack=u+1) ------------------------------------------ | (Server: "ACK, closing my read end")
    | <--- 3. FIN (seq=w) -------------------------------------------- | (Server: "I am also done sending data")
    | --- 4. ACK (ack=w+1) ------------------------------------------> | (Client: "ACK, fully closed")
    | [ TIME_WAIT (2*MSL) ]                                            [ CLOSED ]
```

- **One-Line Intuition:** The 3-way handshake confirms that both sides can independently **send AND receive** before transmitting payload data.
- **The Interview Trap:** Thinking 2 packets are enough. With only 2 packets (SYN $\to$ SYN-ACK), the client knows the server can send and receive, but the **server has no confirmation that the client received its SYN-ACK**.
- **Follow-up:** *What is the purpose of `TIME_WAIT` state on the client?* **To ensure the final ACK was delivered to the server (resending it if lost) and to prevent old delayed duplicate packets from a previous connection interfering with a newly opened connection on the same port.**

---

# 4. HTTP vs. HTTPS & TLS Handshake Mechanics

```
Plain HTTP (Port 80):     [ Plaintext Payload: "password=123" ] (Vulnerable to eavesdropping & MITM)

HTTPS (Port 443):         [ Encrypted TLS Tunnel ] ---> [ Ciphertext: "a9#k!z98" ]
```

### The TLS 1.2 / 1.3 Handshake in 3 Simple Steps:
1. **Certificate Verification (Asymmetric Crypto):** Server presents its SSL/TLS Certificate signed by a trusted Certificate Authority (CA). Client verifies the server's identity using the CA's public key.
2. **Key Exchange (Asymmetric Crypto):** Client and server use asymmetric cryptography (RSA or Diffie-Hellman / ECDHE) to securely agree on a shared random **Symmetric Session Key**.
3. **Data Transfer (Symmetric Crypto):** All subsequent HTTP request/response payloads are encrypted using the fast **Symmetric Session Key** (e.g. AES-GCM).

- **One-Line Intuition:** Asymmetric crypto is the slow armored truck used once to exchange a secret handshake; symmetric crypto is using that secret handshake to talk at lightning speed for the rest of the conversation.
- **30-Second Verbal Answer:** **"HTTPS is HTTP encrypted over TLS on port 443. It provides confidentiality, integrity, and server authentication. It uses asymmetric cryptography during the initial handshake to authenticate certificates and negotiate a shared symmetric key, then uses fast symmetric encryption (like AES) for all actual payload data."**

---

# 5. DNS Resolution: From Domain Name to IP Address

```
Client types "google.com":
[ Browser Cache ] ---> [ OS Local Cache ] ---> [ ISP Recursive Resolver ]
                                                           |
               +-------------------------------------------+-------------------------------------------+
               |                                           |                                           |
               v                                           v                                           v
    1. Root DNS Server (.)                    2. TLD DNS Server (.com)             3. Authoritative DNS Server
    (Returns .com TLD Server IP)              (Returns google.com Auth Server IP)   (Returns exact Host IP 142.250.x.x)
```

### Resolution Flow (Cache Miss Hierarchy):
1. **Browser Cache & OS Hosts Cache:** Immediate local memory check.
2. **Recursive DNS Resolver (ISP / `8.8.8.8`):** Queries on client's behalf.
3. **Root Name Server (`.`):** Directs resolver to Top-Level Domain (TLD) server.
4. **TLD Name Server (`.com`, `.org`):** Directs resolver to Authoritative Name Server.
5. **Authoritative Name Server:** Holds the definitive DNS record (A record) and returns the actual IP address.

- **One-Line Intuition:** DNS is the phonebook of the internet, translating human-friendly domain names (`google.com`) into computer-routable IP addresses (`142.250.190.46`).
- **Follow-up:** *What is DNS TTL (Time-To-Live)?* **The duration in seconds that intermediate resolvers and clients are allowed to cache a DNS record before re-querying the authoritative server.**

---

# 6. High-Frequency Interview Drill & Verbal Q&A

### Q1: What is the difference between TCP Flow Control and Congestion Control?
> **Answer:** **Flow Control** prevents the sender from overwhelming the *receiver's buffer* using a dynamic **Sliding Window (`rwnd`)**. **Congestion Control** prevents the sender from overwhelming the *underlying network routers* using algorithms like Slow Start and Congestion Avoidance (`cwnd`).

### Q2: What is the difference between an IP address and a MAC address?
> **Answer:** An **IP address (Layer 3)** is a logical, globally routable address used to deliver packets across different networks. A **MAC address (Layer 2)** is a physical, hardware-burned identifier used to deliver frames locally between devices on the same physical network link.

### Q3: What is the difference between HTTP/1.1, HTTP/2, and HTTP/3?
> **Answer:** **HTTP/1.1** allows persistent connections but suffers from Head-of-Line (HoL) blocking on single requests. **HTTP/2** introduces binary framing and **multiplexing** over a single TCP connection. **HTTP/3** replaces TCP with **QUIC (over UDP)**, eliminating TCP-level Head-of-Line blocking and enabling near-instant 0-RTT handshakes.

### Q4: Why does DNS primarily use UDP instead of TCP?
> **Answer:** Because DNS requests are small single-packet queries where the **3-way handshake overhead of TCP would double lookup latency**. DNS falls back to TCP only for zone transfers or when response payloads exceed 512 bytes (e.g. DNSSEC).

### Q5: What is a Port number in networking?
> **Answer:** A 16-bit integer ($0\text{--}65535$) used at the Transport Layer to identify a **specific process or service running on a host** (e.g. Port 80 for HTTP, Port 443 for HTTPS, Port 22 for SSH).
