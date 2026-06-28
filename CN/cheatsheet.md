# Computer Networks Interview Cheatsheet 🌐

A concise, high-yield reference for Computer Networks basics, tailored for software engineering and backend interviews.

---

## 1. OSI vs. TCP/IP Model 📑

The OSI model is a conceptual 7-layer framework, while the TCP/IP model is a practical 4-layer suite used in real-world networking.

| OSI Layer | Name | Unit | Primary Protocols / Hardware | Description |
| :--- | :--- | :--- | :--- | :--- |
| **7** | Application | Data | HTTP, HTTPS, DNS, SMTP, FTP | User-interface interaction |
| **6** | Presentation | Data | SSL, TLS, JPEG, ASCII | Encryption, compression, formatting |
| **5** | Session | Data | NetBIOS, RPC, Sockets | Establishes and manages connections |
| **4** | Transport | Segment / Datagram | TCP, UDP | End-to-end reliability & flow control |
| **3** | Network | Packet | IP (IPv4/IPv6), ICMP, Routers | Routing packets between networks |
| **2** | Data Link | Frame | Ethernet, MAC, Switches, ARP | Transferring data between adjacent nodes |
| **1** | Physical | Bits | Cables, Hubs, Repeaters | Physical transmission of raw bits |

*Note: In the **TCP/IP model**, OSI Layers 5–7 are collapsed into the **Application Layer**, and OSI Layers 1–2 are collapsed into the **Network Access (Link) Layer**.*

---

## 2. TCP vs. UDP ⚔️

| Feature | TCP (Transmission Control Protocol) | UDP (User Datagram Protocol) |
| :--- | :--- | :--- |
| **Connection** | Connection-oriented (Handshake required) | Connectionless |
| **Reliability** | Guaranteed delivery (Retransmission) | Best-effort (No guarantee) |
| **Ordering** | Guarantees ordered arrival of packets | No ordering guarantee |
| **Header Size** | 20–60 bytes | 8 bytes |
| **Flow/Congestion** | Yes (Window sliding, Congestion window) | No |
| **Speed** | Slower (due to acknowledgment overhead) | Faster |
| **Use Cases** | HTTP, HTTPS, FTP, SMTP, SSH | Video streaming, DNS, VoIP, Online games |

---

## 3. TCP Three-Way Handshake 🤝

Establishes a reliable connection between Client and Server.

```
   Client                                  Server
     |                                       |
     | ------------ SYN (Seq=X) -----------> |  [Client is SYN_SENT]
     |                                       |  [Server enters SYN_RCVD]
     | <--- SYN-ACK (Seq=Y, Ack=X+1) ------- |
     |                                       |  [Client enters ESTABLISHED]
     | ---------- ACK (Ack=Y+1) -----------> |
     |                                       |  [Server enters ESTABLISHED]
```

### Connection Termination (Four-Way Handshake)
1. **Client $\rightarrow$ Server:** FIN
2. **Server $\rightarrow$ Client:** ACK (Server enters `CLOSE_WAIT`, Client enters `FIN_WAIT_2`)
3. **Server $\rightarrow$ Client:** FIN (Server enters `LAST_ACK`)
4. **Client $\rightarrow$ Server:** ACK (Client enters `TIME_WAIT` for 2MSL to ensure ACK is received; connection closed)

---

## 4. HTTP Status Codes Quick Reference 🎯

| Range | Class | Common Examples |
| :--- | :--- | :--- |
| **1xx** | Informational | `101 Switching Protocols` (WebSockets upgrade) |
| **2xx** | Success | `200 OK`, `201 Created`, `204 No Content` |
| **3xx** | Redirection | `301 Moved Permanently` (browser caches this), `302 Found` (temp), `304 Not Modified` |
| **4xx** | Client Error | `400 Bad Request`, `401 Unauthorized`, `403 Forbidden`, `404 Not Found`, `429 Too Many Requests` |
| **5xx** | Server Error | `500 Internal Server Error`, `502 Bad Gateway`, `503 Service Unavailable`, `504 Gateway Timeout` |

---

## 5. Common Port Numbers 🔌

* **22**: SSH (Secure Shell)
* **53**: DNS (Domain Name System)
* **80**: HTTP
* **443**: HTTPS
* **5432**: PostgreSQL (Database standard)
* **3306**: MySQL
* **6379**: Redis
