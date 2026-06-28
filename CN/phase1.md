# Phase 1: Networking Fundamentals 🌐

This report covers basic networking concepts, the models governing communications, data translation layers, and hardware infrastructure.

---

## 1. The OSI Model (Open Systems Interconnection)

The OSI model is a conceptual 7-layer framework developed by the ISO to standardize network communication.

| Layer | Name | Data Unit | Primary Function | Key Protocols / Hardware |
| :--- | :--- | :--- | :--- | :--- |
| **7** | **Application** | Data | User-interface & network application interaction | HTTP, HTTPS, DNS, FTP, SMTP |
| **6** | **Presentation**| Data | Data translation, formatting, encryption, & compression | SSL/TLS, ASCII, JPEG, MPEG |
| **5** | **Session**      | Data | Establishes, manages, & terminates communication sessions | NetBIOS, RPC, Sockets |
| **4** | **Transport**    | Segment | End-to-end reliability, flow control, & error recovery | TCP, UDP |
| **3** | **Network**      | Packet | Logical routing of packets across networks | IP (IPv4/IPv6), ICMP, Routers |
| **2** | **Data Link**    | Frame | Physical transfer of frames between adjacent nodes | Ethernet, MAC, ARP, Switches |
| **1** | **Physical**     | Bits | Physical transmission of raw bits over medium | Cables, Hubs, Repeaters, NICs |

---

## 2. The TCP/IP Model

The TCP/IP model is a practical, 4-layer architecture used in modern operating systems and the internet. It maps to the OSI model as follows:

| TCP/IP Layer | Corresponding OSI Layers | Focus |
| :--- | :--- | :--- |
| **Application** | Application (7), Presentation (6), Session (5) | App-level data formatting & service delivery |
| **Transport** | Transport (4) | End-to-end port-based delivery & reliability |
| **Internet** | Network (3) | Host-to-host IP routing across networks |
| **Network Access (Link)** | Data Link (2), Physical (1) | Physical wire-level frame delivery |

---

## 3. Encapsulation & Decapsulation

This is the process of wrapping data with protocol headers (and trailers) as it moves down the network stack, and stripping them as it moves up.

```
[ Sender Side: Encapsulation (Down) ]      [ Receiver Side: Decapsulation (Up) ]
      Application Data                                  Application Data
             │                                                 ▲
             ▼                                                 │
      [TCP Header] + Data                              [TCP Header] + Data    (Segment)
             │                                                 ▲
             ▼                                                 │
[IP Header] + [TCP] + Data                       [IP Header] + [TCP] + Data   (Packet)
             │                                                 ▲
             ▼                                                 │
[Eth Header] + [IP] + [TCP] + Data + [Trailer]   [Eth Header] + [IP] + [TCP]  (Frame)
             │                                                 ▲
             ▼                                                 │
      1010001110101010100101                           1010001110101010100101 (Bits)
```

1. **Encapsulation (Sender):** Application data passes down. The transport layer appends a TCP/UDP header (**Segment**). The network layer appends an IP header (**Packet**). The data link layer appends an Ethernet header/trailer (**Frame**). The physical layer converts this into electrical/optical signals (**Bits**).
2. **Decapsulation (Receiver):** Raw signals are received as bits. The link layer validates the MAC header and strips it. The network layer checks the IP header for destination matching and strips it. The transport layer checks the port and forwards the raw data to the target socket.

---

## 4. Network Devices

* **Hub (Layer 1):** A passive device that connects nodes on a local network. It does not inspect headers; it simply copies any incoming bit stream and repeats it to **all** physical ports.
  * *Result:* Creates a single collision domain and broadcast domain, leading to high collision rates.
* **Switch (Layer 2):** An active device that inspects Layer 2 frames. It maintains a **MAC Address Table (CAM Table)** by reading the source MAC addresses of incoming frames. When forwarding, it sends the frame strictly to the port matching the destination MAC.
  * *Result:* Each switch port is an isolated collision domain, but all ports share a single broadcast domain.
* **Router (Layer 3):** A device that connects distinct subnets. It reads Layer 3 IP headers and uses routing tables (static or dynamic) to determine the next hop for a packet.
  * *Result:* Routers block broadcast frames, dividing networks into separate broadcast domains.
* **Gateway (Cross-Layer):** A network node that acts as a translator between completely incompatible network architectures or protocols (e.g., connecting an enterprise IP network to a legacy telecom SS7 system).

---

## 5. MAC Address vs. IP Address

| Feature | MAC Address (Media Access Control) | IP Address (Internet Protocol) |
| :--- | :--- | :--- |
| **Layer** | Data Link Layer (Layer 2) | Network Layer (Layer 3) |
| **Type** | Physical / Hardware address | Logical / Software address |
| **Uniqueness**| Hardcoded in NIC at factory (globally unique) | Assigned dynamically by router or administrator |
| **Format** | 48-bit hex (e.g., `00:1A:2B:3C:4D:5E`) | 32-bit (IPv4) or 128-bit (IPv6) |
| **Purpose** | Hop-to-hop communication on the local wire | End-to-end routing across global networks |
