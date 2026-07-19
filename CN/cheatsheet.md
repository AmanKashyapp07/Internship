# Computer Networks Interview Cheatsheet 🌐

A concise, high-yield reference for Computer Networks concepts, tailored for software engineering and backend developer interviews.

---

## 1. Network Models: OSI vs. TCP/IP 📑

The OSI model is a 7-layer theoretical standard; the TCP/IP model is a 4-layer practical suite.

| OSI Layer | Name | Data Unit | Primary Protocols / Devices | Core Purpose |
| :---: | :--- | :---: | :--- | :--- |
| **7** | Application | Data | HTTP, HTTPS, DNS, SMTP, FTP | Application-level interactions |
| **6** | Presentation | Data | SSL/TLS, ASCII, JPEG, MPEG | Encryption, translation, formatting |
| **5** | Session | Data | NetBIOS, RPC, Sockets | Inter-host session management |
| **4** | Transport | Segment | TCP, UDP | End-to-end reliability, flow/congestion |
| **3** | Network | Packet | IP, ICMP, ARP / Routers | Logical address routing across networks |
| **2** | Data Link | Frame | Ethernet, MAC, 802.11 / Switches | graphacent-node frame delivery |
| **1** | Physical | Bits | Cables, Fiber, RJ45 / Hubs, NICs | Physical transmission of raw bit streams |

*Note: In the **TCP/IP model**, OSI Layers 5–7 are collapsed into the **Application Layer**, and OSI Layers 1–2 are collapsed into the **Network Access (Link) Layer**.*

---

## 2. IP Addressing & Subnetting Reference 🗺️

### IPv4 vs. IPv6
* **IPv4:** 32-bit address space ($2^{32} \approx 4.3$B IPs). E.g., `192.168.1.1`. Variable header size (20–60 bytes).
* **IPv6:** 128-bit address space ($2^{128} \approx 3.4 \times 10^{38}$ IPs). E.g., `2001:db8::1`. Fixed header size (40 bytes), eliminating checksum computation overhead in routers.

### Subnetting Math Formulas
* **Total IPs in Subnet:** $2^{\text{Host Bits}}$
* **Usable IPs in Subnet:** $2^{\text{Host Bits}} - 2$ (removes Network ID and Broadcast IP)
* **Host Bits:** $32 - \text{Prefix Length (CIDR)}$
* **Number of generated subnets:** $2^{\text{Borrowed Bits}}$

### Common Subnet Reference Table
| CIDR | Subnet Mask | Total IPs | Usable IPs |
| :--- | :--- | :---: | :---: |
| `/30` | `255.255.255.252` | 4 | 2 (Point-to-point links) |
| `/28` | `255.255.255.240` | 16 | 14 |
| `/26` | `255.255.255.192` | 64 | 62 |
| `/24` | `255.255.255.0` | 256 | 254 |
| `/22` | `255.255.252.0` | 1024 | 1022 |
| `/16` | `255.255.0.0` | 65,536 | 65,534 |

---

## 3. Transport Layer: TCP vs. UDP ⚔️

| Feature | TCP (Transmission Control Protocol) | UDP (User Datagram Protocol) |
| :--- | :--- | :--- |
| **Connection Type** | Connection-oriented (Handshake required) | Connectionless |
| **Reliability** | Guaranteed delivery (Retransmissions) | Best-effort (No delivery checks) |
| **Data Order** | Guaranteed in-order arrival | No ordering guarantees |
| **Transmission Unit**| Byte stream (No message boundaries) | Datagram (Preserves message boundaries) |
| **Header Size** | 20 to 60 bytes | Exactly 8 bytes |
| **Flow & Congestion**| Active (`rwnd` & `cwnd` window buffers) | None |
| **Speed** | Slower (overhead of ACKs, sequencing) | Fast (minimal translation overhead) |
| **Use Cases** | HTTP, HTTPS, SSH, FTP, SMTP, DBs | DNS, DHCP, VoIP, Live Streaming, Gaming |

---

## 4. Connection State Machine: TCP Handshakes 🤝

### Connection Setup (Three-Way Handshake)
```
Client (SYN_SENT)                                         Server (LISTEN)
       │ ───────────── SYN (Seq=X) ────────────────────────────► │ [Enters SYN_RCVD]
       │ ◄──────────── SYN-ACK (Seq=Y, Ack=X+1) ──────────────── │
       │ ───────────── ACK (Seq=X+1, Ack=Y+1) ─────────────────► │ [Enters ESTABLISHED]
```

### Connection Close (Four-Way Handshake)
```
Client (ESTABLISHED)                                      Server (ESTABLISHED)
       │ ───────────── FIN ────────────────────────────────────► │ [Enters CLOSE_WAIT]
       │ ◄──────────── ACK ───────────────────────────────────── │ [Client enters FIN_WAIT_2]
       │ ◄──────────── FIN ───────────────────────────────────── │ [Enters LAST_ACK]
       │ ───────────── ACK ────────────────────────────────────► │ [Enters CLOSED]
```
* **`TIME_WAIT`:** The initiating closer stays in `TIME_WAIT` for **2MSL** (Maximum Segment Lifetime) before fully closing to:
  1. Ensure the final `ACK` reaches the receiver (retransmitting if lost).
  2. Allow lingering duplicate packets to die off, preventing data corruption in new connections.

---

## 5. Application Layer: HTTP Semantics 🎯

### HTTP Status Codes
* **1xx (Informational):** `101 Switching Protocols` (upgrading connection to WebSockets).
* **2xx (Success):** `200 OK`, `201 Created` (POST success), `204 No Content` (Empty body success).
* **3xx (Redirect):** `301 Moved Permanently` (browser cached), `302 Found` (temp), `304 Not Modified` (use browser cache).
* **4xx (Client Error):** `400 Bad Request`, `401 Unauthorized` (unauthenticated), `403 Forbidden` (unauthorized), `404 Not Found`, `429 Too Many Requests`.
* **5xx (Server Error):** `500 Internal Server Error`, `502 Bad Gateway` (proxy issue), `503 Service Unavailable`, `504 Gateway Timeout`.

### HTTP Methods Safe & Idempotent Matrix
* **Safe:** Does not modify resource state on the server.
* **Idempotent:** Executing identical requests multiple times leaves the server in the exact same state as a single execution.

| Method | Description | Safe? | Idempotent? |
| :--- | :--- | :---: | :---: |
| **GET** | Read resource data | ✅ Yes | ✅ Yes |
| **POST** | Create a new resource | ❌ No | ❌ No |
| **PUT** | Replace/Create resource at URI | ❌ No | ✅ Yes |
| **DELETE** | Remove resource | ❌ No | ✅ Yes |
| **PATCH** | Apply partial update to resource | ❌ No | ❌ No |

---

## 6. Authentication & Session Management 🔑

| Feature | Cookies | Sessions | JWT (JSON Web Tokens) |
| :--- | :--- | :--- | :--- |
| **Storage Location** | Client Browser | Server memory/database | Client Browser (`localStorage` or Cookie) |
| **Statefulness** | Stateless key-value holder | Stateful (Server records active sessions) | Stateless (Token contains claims) |
| **Scalability** | High | Low (Requires cluster session sync) | High (Server only verifies token signature) |
| **Security Risks** | XSS (if not `HttpOnly`), CSRF | Session ID theft | XSS theft of Token |
| **Payload Size** | Small (up to 4KB) | Minimal (only contains Session ID) | Medium/Large (contains encoded claims) |

---

## 7. DNS & Common DNS Record Types 🔍

DNS maps domain names to IP addresses. Below are the core record types:
* **A (Address Record):** Maps a hostname to an IPv4 address (e.g., `google.com` $\rightarrow$ `1.2.3.4`).
* **AAAA (IPv6 Address Record):** Maps a hostname to an IPv6 address.
* **CNAME (Canonical Name):** Maps an alias name to another canonical domain name (e.g., `www.example.com` $\rightarrow$ `example.com`).
* **MX (Mail Exchanger):** Specifies mail servers responsible for receiving email for the domain.
* **TXT (Text Record):** Stores arbitrary text data (commonly used for domain validation, SPF, and DKIM email security).
* **NS (Name Server):** Identifies the authoritative nameservers for the zone.

---

## 8. Common Networking Port Numbers 🔌

* **21:** FTP (File Transfer)
* **22:** SSH / SFTP (Secure Shell)
* **23:** Telnet (Unencrypted terminal)
* **25:** SMTP (Email sending)
* **53:** DNS (Domain Name System)
* **67/68:** DHCP (IP auto-assignment)
* **80:** HTTP (Web unencrypted)
* **110:** POP3 (Email retrieval)
* **143:** IMAP (Email retrieval)
* **443:** HTTPS (Web secure)
* **3306:** MySQL database
* **5432:** PostgreSQL database
* **6379:** Redis cache
