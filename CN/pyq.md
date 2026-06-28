# Computer Networks Interview Q&A (Top 75) 🚀

A compilation of the 75 most-asked Computer Networks interview questions for software engineering and backend developer roles.

---

### Q1. What happens step-by-step when you type a URL (e.g., `https://google.com`) into your browser and press Enter?
* **Answer:**
  1. **Browser Parses URL:** Separates protocol (`https`), hostname (`google.com`), and resource path.
  2. **DNS Lookup:** Resolves hostname to an IP address. Checks local browser cache $\rightarrow$ OS hosts file $\rightarrow$ Router Cache $\rightarrow$ ISP DNS server (recursive query).
  3. **ARP (Address Resolution Protocol):** Finds the MAC address of the default gateway router matching its IP.
  4. **TCP Handshake:** Establishes a TCP connection with the target server IP on port 443 (three-way handshake).
  5. **TLS Handshake:** Negotiates encryption keys and verifies certificates for HTTPS.
  6. **Send HTTP Request:** Browser sends an `HTTP GET` request for the page.
  7. **Server Process:** Server handles the request and returns an HTTP response containing HTML/assets.
  8. **Render Page:** Browser parses HTML, fetches external assets, executes Javascript, and renders the page.

---

### Q2. Compare MAC Address vs. IP Address. Why do we need both?
* **Answer:**
  * **MAC Address (Physical/Hardware):** 48-bit address burnt into the Network Interface Card (NIC) at factory level. Used to identify devices on the **local network segment** (Layer 2).
  * **IP Address (Logical/Software):** 32-bit (IPv4) or 128-bit (IPv6) address assigned dynamically by routers/DHCP. Used to route packets across **interconnected networks** (Layer 3).
  * **Why both:** IP addresses handle routing from country A to country B (global pathfinding). MAC addresses handle hop-by-hop transmission between adjacent switches/routers on the same wire (local delivery).

---

### Q3. What is ARP (Address Resolution Protocol)? How does it work?
* **Answer:**
  * **ARP** maps a known Layer 3 logical address (IP) to a Layer 2 physical address (MAC).
  * **How it works:**
    1. A device broadcasts an **ARP Request** containing the target IP: *"Who has IP 192.168.1.5? Tell MAC 00:1A:2B..."*
    2. All local devices receive it, but only the owner of 192.168.1.5 unicasts an **ARP Reply**: *"I have that IP. My MAC is 00:1A:2B:3C..."*
    3. The sender caches this in its local ARP table to avoid repeat broadcasts.

---

### Q4. What is CIDR (Classless Inter-Domain Routing)? How does subnet masking work?
* **Answer:**
  * **CIDR** replaced old Classful routing (A, B, C) with a flexible system allowing arbitrary-length network prefixes.
  * **Subnet Masking:** A 32-bit mask dividing an IP into the **Network Portion** (represented by 1s) and the **Host Portion** (represented by 0s).
  * **Example:** `192.168.1.0/24` means the first 24 bits are the network address. The remaining $32 - 24 = 8$ bits are for hosts, allowing $2^8 - 2 = 254$ unique host IPs. (We subtract 2 because the first IP is the network identifier, and the last is the broadcast IP).

---

### Q5. What is the difference between a Hub, a Switch, and a Router?
* **Answer:**
  * **Hub (Layer 1):** Connects devices. Operates by repeating incoming electrical signals to **all** ports. Creates high network collision rates.
  * **Switch (Layer 2):** Connects local devices. Inspects destination MAC addresses in frames and routes them strictly to the destination port, reducing collisions.
  * **Router (Layer 3):** Connects distinct networks. Reads IP packets to forward data packets between different subnets.

---

### Q6. What is the TCP `TIME_WAIT` state? Why does it exist?
* **Answer:**
  * When a connection terminates, the side that initiates the active close enters the `TIME_WAIT` state after sending the final ACK. It remains in this state for twice the Maximum Segment Lifetime (2MSL), typically 1–4 minutes.
  * **Why it exists:**
    1. **Guarantees FIN Delivery:** If the final ACK is lost in transit, the server will retransmit its FIN. The client must stay open to receive it and resend the ACK.
    2. **Prevents Data Corruption:** Ensures old packets matching the same socket connection die off on the network, preventing them from being mistakenly read by new connections reusing the same ports.

---

### Q7. What is NAT (Network Address Translation)? Why is it used?
* **Answer:**
  * **NAT** maps multiple private IPs on a local network to a single public IP address at the gateway router.
  * **Why used:** It slowed down the exhaustion of the IPv4 address space by allowing entire home and corporate networks to share a single public IP. It also provides basic security by keeping internal IPs hidden from the public internet.

---

### Q8. What is the DHCP (Dynamic Host Configuration Protocol)? Explain the DORA process.
* **Answer:**
  * **DHCP** dynamically assigns IP addresses and network configurations (subnet mask, DNS, gateway) to devices joining a network.
  * **DORA Process (UDP ports 67/68):**
    1. **Discover:** Client broadcasts a packet to locate active DHCP servers.
    2. **Offer:** Server broadcasts/unicasts an offer containing an available IP.
    3. **Request:** Client broadcasts a request to accept that specific IP offer.
    4. **Acknowledge:** Server commits the lease and acknowledges back to the client.

---

### Q9. Compare Symmetric vs. Asymmetric Encryption in TLS/HTTPS.
* **Answer:**
  * **Asymmetric Encryption (Public/Private Keys):** Extremely secure, but slow and computationally expensive.
    * *Use case:* Used only during the initial TLS handshake to verify the server identity and securely agree on a shared key.
  * **Symmetric Encryption (Shared Key):** Fast and lightweight, but sharing the key securely is difficult.
    * *Use case:* Once the shared session key is agreed upon during the handshake, all subsequent HTTP request/response payloads are encrypted using symmetric encryption.

---

### Q10. What are WebSockets? How do they differ from HTTP Long-Polling?
* **Answer:**
  * **WebSockets (RFC 6455):** A protocol providing full-duplex, persistent communication channels over a single TCP connection, upgrade-initiated via HTTP.
  * **Difference:** In HTTP Long-Polling, the client continually opens a request, the server hangs on until new data is available, responds, and closes the connection, repeating the loop. WebSockets keep a single TCP channel open, sending messages instantly back and forth with zero connection setup overhead.

---

### Q11. How do `ping` and `traceroute` work?
* **Answer:**
  * Both rely on **ICMP (Internet Control Message Protocol)**.
  * **`ping`:** Sends an `ICMP Echo Request` to an IP. If reachable, the target returns an `ICMP Echo Reply`, measuring round-trip latency.
  * **`traceroute`:** Sends packets with incrementing **TTL (Time to Live)** fields starting at 1. The first router decrements TTL to 0, drops the packet, and returns an `ICMP Time Exceeded` error. We record its IP, increment TTL to 2, and repeat until we reach the destination, mapping the path hop-by-hop.

---

### Q12. What is a proxy server? Compare Forward Proxy vs. Reverse Proxy.
* **Answer:**
  * **Forward Proxy:** Acts on behalf of the **client**. Internal clients route queries through the proxy to access the public internet. Used for content filtering and hiding client IPs.
  * **Reverse Proxy:** Acts on behalf of the **server**. Incoming requests from the public internet hit the proxy, which forwards them to backend servers. Used for load balancing, SSL termination, and caching.

---

### Q13. Explain the difference between CORS (Cross-Origin Resource Sharing) and Same-Origin Policy (SOP).
* **Answer:**
  * **Same-Origin Policy:** A critical browser security model preventing a script on page A from reading data on page B unless they share the same Origin (Protocol + Domain + Port).
  * **CORS:** A mechanism that uses additional HTTP headers to tell browsers to allow scripts on origin A to access resources from origin B. The target server must explicitly send `Access-Control-Allow-Origin: origin_A` in response.

---

### Q14. What is a SYN Flood attack? How does a server defend against it?
* **Answer:**
  * **SYN Flood:** A Denial of Service (DoS) attack where the attacker floods the server with `SYN` requests but never sends the final `ACK` to complete the handshake. This leaves the server holding thousands of half-open TCP connections in its memory buffer, exhausting resources.
  * **Defense (SYN Cookies):** The server does not allocate memory for the connection upon receiving a `SYN`. Instead, it encodes connection details into the `SYN-ACK` sequence number (a cryptographic cookie). When the client returns the final `ACK`, the server validates the sequence number and instantiates the connection only then.

---

### Q15. Compare Cookie vs. Session vs. LocalStorage.
* **Answer:**
  * **Cookie:** Small data strings (up to 4KB) stored in the browser. Sent automatically with every HTTP request. Used for session identifiers.
  * **LocalStorage:** HTML5 storage holding up to 5–10MB. Stays in the browser indefinitely until cleared via Javascript. Never sent automatically to the server.
  * **Session:** Server-side storage holding state about a user. The browser only stores a `session_id` cookie, which the server reads to look up the session object in its database/cache.

---

### Q16. What is TCP Keep-Alive vs. HTTP Keep-Alive?
* **Answer:**
  * **HTTP Keep-Alive:** An application-layer directive (`Connection: keep-alive`) telling the server and browser to reuse the same TCP connection for multiple HTTP requests, avoiding handshake delays for images/CSS.
  * **TCP Keep-Alive:** A transport-layer socket feature. If a connection is idle, the OS sends small probe packets to verify the other side is still active and hasn't crashed or disconnected silently.

---

### Q17. Explain Symmetric Routing vs. Asymmetric Routing.
* **Answer:**
  * **Symmetric Routing:** Packets traveling from source A to destination B take the exact same sequence of routers/paths on the return trip from B to A.
  * **Asymmetric Routing:** Packets returning from B to A take a completely different physical path/sequence of routers than the outbound path from A to B. Common on the internet due to dynamic BGP routing changes.

---

### Q18. Compare Unicast, Broadcast, Multicast, and Anycast.
* **Answer:**
  * **Unicast:** One-to-one communication (packet sent to one destination IP).
  * **Broadcast:** One-to-all communication (packet sent to all hosts on the subnet, e.g., ARP).
  * **Multicast:** One-to-many communication (packet sent to a specific group of subscribed hosts).
  * **Anycast:** One-to-nearest communication (packet sent to a single IP address shared by multiple physical servers; routing protocols direct it to the topologically nearest node, common in CDNs).

---

### Q19. What is a Content Delivery Network (CDN)? How does it work?
* **Answer:**
  * **CDN:** A distributed network of proxy servers (Edge servers) located close to users.
  * **How it works:** Static assets (images, video, JS) are cached on Edge servers. When a user requests an asset, DNS routing redirects the query to the topologically nearest CDN edge server, reducing latency and backend database traffic.

---

### Q20. What is MTU (Maximum Transmission Unit)? What happens if a packet exceeds it?
* **Answer:**
  * **MTU** is the maximum size (in bytes) of a packet that can be sent over a physical network medium (standard Ethernet MTU is 1500 bytes).
  * **Fragmentation:** If a packet exceeds the MTU of a router along its path:
    * If the `DF (Don't Fragment)` flag is 0 in the IP header, the router splits the packet into smaller fragments, transmitting them independently. The destination host reassembles them.
    * If the `DF` flag is 1, the router drops the packet and returns an `ICMP Destination Unreachable` error.

---

### Q21. Explain the difference between Latency and Throughput.
* **Answer:**
  * **Latency:** The time delay it takes for a single packet to travel from source to destination (measured in milliseconds).
  * **Throughput:** The volume of data successfully transmitted over the network link per unit of time (measured in Mbps or Gbps).

---

### Q22. Explain Session Hijacking and how to protect against it.
* **Answer:**
  * **Session Hijacking:** An attacker steals a user's session identifier (usually a cookie) and uses it to impersonate the user to the web server.
  * **Protection:**
    * Mark cookies with the **`Secure`** flag (only sent over HTTPS).
    * Mark cookies with the **`HttpOnly`** flag (makes cookie inaccessible to malicious Javascript scripts, preventing XSS thefts).
    * Use **`SameSite=Strict/Lax`** to mitigate CSRF cross-origin cookie attachment.

---

### Q23. What is DNS Cache Poisoning?
* **Answer:**
  * An exploit where an attacker introduces false DNS records into a recursive resolver's cache.
  * **Impact:** Subsequent users attempting to visit a legitimate site (e.g., `bank.com`) are redirected to an IP controlled by the attacker (malicious clone site) to harvest credentials.

---

### Q24. What is the role of the TTL (Time to Live) field in IP packet headers?
* **Answer:**
  * **TTL** is an 8-bit counter in the IP header set by the sender. Every router that forwards the packet decrements the TTL by 1.
  * **Why needed:** If a routing loop occurs (packet bounces indefinitely between two routers), the TTL will eventually hit 0. The router holding the packet drops it and sends an ICMP error, preventing orphaned packets from circulating forever and crashing the network.

---

### Q25. What is the difference between IPv4 and IPv6?
* **Answer:**
  * **Address Space:** IPv4 uses 32-bit addresses ($2^{32} \approx 4.3 \text{ Billion}$ IPs). IPv6 uses 128-bit addresses ($2^{128} \approx 3.4 \times 10^{38}$ IPs).
  * **Header Format:** IPv6 has a fixed, simplified 40-byte header, making packet processing faster in routers.
  * **IPsec:** Built into the IPv6 standard, whereas it's optional in IPv4.

---

### Q26. Explain HTTP Basic Authentication. How is it secured?
* **Answer:**
  * **Mechanism:** The client sends credentials encoded as a Base64 string (`username:password`) in the `Authorization: Basic <base64>` header.
  * **Security:** Base64 is **not encryption**; it is easily decoded. Basic auth is only secure if wrapped inside HTTPS/TLS, which encrypts the header block in transit.

---

### Q27. Compare Flow Control vs. Congestion Control.
* **Answer:**
  * **Flow Control:** Solves speed mismatches between the **Sender and the Receiver** (prevents sender from overflowing receiver's buffer).
  * **Congestion Control:** Solves speed mismatches between the **Sender and the Network infrastructure** (prevents sender from overloading intermediate routers/switches).

---

### Q28. What is the loopback address? Why is it 127.0.0.1?
* **Answer:**
  * The **Loopback Address** is a special IP address reserved for a host to send packets to itself. Used for local testing of server software.
  * Packets sent to 127.0.0.1 never leave the local machine's network stack; they are routed internally by the OS loopback driver directly back to the receiver queue.

---

### Q29. What is a socket? Name the system calls used in a TCP server lifecycle.
* **Answer:**
  * A **Socket** is a software abstraction representing an endpoint for sending and receiving data over the network.
  * **TCP Server socket lifecycle calls:**
    1. `socket()`: Creates a new endpoint socket descriptor.
    2. `bind()`: Binds the socket to a specific local IP and Port number.
    3. `listen()`: Puts the socket in listening mode to accept incoming client connections.
    4. `accept()`: Blocks execution until a client requests connection; returns a new socket dedicated to that client.
    5. `read()` / `write()`: Exchanging data.
    6. `close()`: Closes connection.

---

### Q30. What is BGP (Border Gateway Protocol)?
* **Answer:**
  * **BGP** is the routing protocol of the global Internet. It is used to exchange routing path information between different **Autonomous Systems (AS)** (large network networks managed by ISPs, universities, and tech companies).
  * It determines the most efficient path for packets to travel across the mesh of internet network links.

---

### Q31. What is the difference between Symmetric and Asymmetric Cryptography? How are they combined in TLS?
* **Answer:**
  * **Symmetric Cryptography:** Uses the same single key for both encryption and decryption (e.g., AES, DES). Very fast, but key exchange is insecure.
  * **Asymmetric Cryptography:** Uses a key pair: a public key for encryption (shared openly) and a private key for decryption (kept secret) (e.g., RSA, ECC). High security, but computationally expensive/slow.
  * **Combination in TLS:** The handshake uses asymmetric cryptography to securely authenticate the server and negotiate a temporary shared session key. Once established, all data transmitted is encrypted using symmetric cryptography (fast) using that session key.

---

### Q32. In the TCP Three-Way Handshake, what happens if the third ACK from the client is lost?
* **Answer:**
  * The server is in the `SYN_RCVD` state and is waiting for the `ACK`. The client is already in the `ESTABLISHED` state (since it sent the `ACK` and assumed the handshake completed).
  * **If client starts sending data:** The data packet contains the acknowledgement number. When the server receives the data packet, it reads the ACK field, implicitly completes the handshake, moves to `ESTABLISHED`, and processes the data.
  * **If client does not send data immediately:** The server's retransmission timer will expire. The server will retransmit the `SYN-ACK` packet. The client, receiving the duplicate `SYN-ACK`, will re-send the `ACK` packet. If multiple retransmissions fail, the server will close the half-open connection (resource cleanup).

---

### Q33. What is a TCP Half-Open Connection?
* **Answer:**
  * A connection is half-open if one side has closed or crashed without notifying the other, or if the three-way handshake was not completed.
  * **Detection:** The active side will only discover the connection is dead when it tries to send a packet and receives a `RST` (Reset) packet in response, or if the socket's TCP Keep-Alive probe timer expires without receiving an ACK.

---

### Q34. What is a DHCP Starvation Attack? How is it mitigated?
* **Answer:**
  * **Attack:** An attacker broadcasts thousands of DHCP requests with spoofed MAC addresses, exhausting the DHCP server's pool of IP addresses. Real clients joining the network are unable to get an IP address.
  * **Mitigation (DHCP Snooping):** A Layer 2 switch security feature. It inspects DHCP packets on untrusted ports, mapping MAC addresses to assigned IPs. It blocks incoming DHCP Release or Request messages if the MAC address does not match the switch's binding database.

---

### Q35. What is the difference between a Collision Domain and a Broadcast Domain?
* **Answer:**
  * **Collision Domain:** A network segment where packets can collide with each other during transmission (devices share physical media).
    * *Boundaries:* Switches and Routers break collision domains. Every port on a switch is a separate collision domain.
  * **Broadcast Domain:** A network segment where any device broadcasting a packet will have that packet received by all other devices in the same segment.
    * *Boundaries:* Only Routers break broadcast domains (Layer 3). Switches forward broadcast frames (FF:FF:FF:FF:FF:FF) to all ports, keeping them in the same broadcast domain.

---

### Q36. Compare Link-State (OSPF) vs. Distance-Vector (RIP) Routing Protocols.
* **Answer:**
  * **Distance-Vector (e.g., RIP):**
    * Nodes only know the distance (hop count) and direction (next hop) to destinations.
    * Periodically advertise their entire routing tables to immediate neighbors.
    * Slow convergence; prone to routing loops (count-to-infinity problem).
  * **Link-State (e.g., OSPF):**
    * Every node maintains a complete map (topology graph) of the entire network.
    * Nodes flood Link-State Advertisements (LSAs) detailing only changed link statuses.
    * Fast convergence using Dijkstra's algorithm; avoids routing loops.

---

### Q37. What are Ephemeral Ports? How are they used?
* **Answer:**
  * **Ephemeral Ports:** Temporary, short-lived transport ports allocated dynamically by the client OS when initiating a connection to a server.
  * **Range:** Typically $49152$ to $65535$ (IANA standard), though Linux often uses $32768$ to $60999$.
  * **Usage:** When a client opens a connection to `google.com:443`, the OS binds a free ephemeral port (e.g., `51024`) as the source port. This ensures the return packets from the server are routed back to the correct client process socket.

---

### Q38. What is DNSSEC (Domain Name System Security Extensions)?
* **Answer:**
  * **DNSSEC:** A suite of extensions adding cryptographic signatures to DNS records to protect clients from DNS spoofing and cache poisoning.
  * **Mechanism:** Nameservers cryptographically sign DNS records (e.g., A, AAAA, MX) using public-key cryptography. Resolvers validate these signatures using public keys distributed through the DNS hierarchy, guaranteeing data integrity and origin authenticity. It does **not** encrypt DNS queries (queries remain public text).

---

### Q39. Explain the difference between ALPN (Application-Layer Protocol Negotiation) and SNI (Server Name Indication) in TLS.
* **Answer:**
  * **SNI (Server Name Indication):** The client indicates the hostname it wants to connect to (e.g., `api.example.com`) during the initial `ClientHello` message. This allows a single web server with one IP address to host multiple secure domains and present the correct SSL certificate.
  * **ALPN (Application-Layer Protocol Negotiation):** The client lists its supported application protocols (e.g., `h2`, `http/1.1`) in the `ClientHello`. The server picks the preferred protocol and returns it in the `ServerHello`. This avoids extra network round-trips to negotiate the protocol after connection setup.

---

### Q40. Compare Tunnel Mode vs. Transport Mode in IPsec.
* **Answer:**
  * **Transport Mode:** Only the payload of the IP packet is encrypted/authenticated. The original IP header is left intact.
    * *Use Case:* End-to-end communication between two hosts directly.
  * **Tunnel Mode:** The entire original IP packet (including header and payload) is encrypted and nested inside a brand-new IP packet with a new header.
    * *Use Case:* Gateway-to-gateway (VPN tunnels) connecting two private subnets across the public internet.

---

### Q41. What is Nagle's Algorithm? When should it be disabled?
* **Answer:**
  * **Nagle's Algorithm:** A TCP optimization designed to prevent network congestion by reducing the number of small packets sent. It delays sending small data blocks, buffering them until they can be sent as a single maximum-sized segment (MSS) or until all outstanding data is acknowledged (ACKed).
  * **When to disable (`TCP_NODELAY`):** Real-time interactive applications (e.g., online gaming, SSH terminals, financial trading, mouse movements) where latency is critical and buffering introduces unacceptable delays.

---

### Q42. What is Delayed ACK? How does it interact with Nagle's Algorithm?
* **Answer:**
  * **Delayed ACK:** A TCP optimization where the receiver delays sending an ACK packet (up to 200ms or until 2 packets are received) hoping to piggyback the ACK on returning application data.
  * **Nagle + Delayed ACK Conflict:** If the sender uses Nagle (waits for ACK before sending next segment) and the receiver uses Delayed ACK (waits for more packets before sending ACK), they block each other, causing a latency lock of 200ms for small messages. Disabling Nagle via `TCP_NODELAY` resolves this.

---

### Q43. What is Silly Window Syndrome (SWS)? How is it resolved?
* **Answer:**
  * **SWS:** A performance degradation that occurs when the receiver window buffer is full, and it advertises tiny window sizes (e.g., 1 byte) to the sender. The sender immediately transmits a 1-byte payload wrapped in a heavy 40-byte TCP/IP header, wasting bandwidth.
  * **Resolution:**
    * **Clark's Solution (Receiver-side):** The receiver prevents advertising small window expansions. It advertises a window size of 0 until its buffer has space for a full MSS or at least half its total buffer capacity.
    * **Nagle's (Sender-side):** Sender buffers data until it has enough to fill a full MSS segment before sending.

---

### Q44. Compare a Stateful Firewall vs. a Stateless Firewall.
* **Answer:**
  * **Stateless Firewall:** Inspects each packet independently based on static criteria (source/destination IP, port, protocol) without memory of previous packets. (Fast, but easily bypassed).
  * **Stateful Firewall:** Tracks the state of active network connections (maintains a state table of TCP handshakes). It automatically allows return packets for established connections, blocking unsolicited incoming packets even if they match basic rules. (Highly secure).

---

### Q45. Explain the purpose of Subnetting.
* **Answer:**
  * **Subnetting** is the practice of dividing a single large network into smaller, logically isolated sub-networks.
  * **Benefits:**
    * **Security:** Isolates different departments or sensitive server clusters from general traffic.
    * **Performance:** Limits the range of broadcast traffic (reduces broadcast storm impacts).
    * **Management:** Simplifies host allocation and tracking.

---

### Q46. What is a Virtual Private Network (VPN)? How does tunneling work?
* **Answer:**
  * **VPN:** Creates a secure, encrypted connection (tunnel) over a public network (the Internet), allowing remote clients to access a private corporate network as if they were physically connected locally.
  * **Tunneling:** The client encrypts its local packets, wraps them inside standard public routing headers (using protocols like IPsec, OpenVPN, or WireGuard), and sends them to the VPN gateway, which decrypts and forwards them into the private subnet.

---

### Q47. What is Path MTU Discovery (PMTUD)?
* **Answer:**
  * **PMTUD** is a technique used to determine the maximum transmission unit (MTU) size supported along the entire path between a sender and receiver without fragmentation.
  * **Mechanism:** The sender transmits packets with the `DF (Don't Fragment)` flag set to 1. If a router along the path cannot forward the packet due to a smaller MTU, it drops it and returns an `ICMP Type 3 Code 4 (Destination Unreachable - Fragmentation Needed)` message containing its MTU. The sender reduces its packet size accordingly and repeats the process.

---

### Q48. What is a Head-of-Line (HoL) blocking problem in networks?
* **Answer:**
  * **HoL Blocking:** A performance queueing delay that occurs when a single blocked or slow packet at the front of a queue holds up all subsequent packets behind it, even if those packets are destined for empty queues or ready targets.
  * *Transport Level:* Occurs in TCP streams where packet loss halts delivery of all subsequent packets until the lost packet is retransmitted.
  * *Application Level:* Occurs in HTTP/1.1 where a slow request blocks the reuse of the connection for other requests.

---

### Q49. What is Anycast Routing? Give a practical use case.
* **Answer:**
  * **Anycast:** A routing methodology where multiple physical servers share the exact same IP address. Routers route packets to the topologically closest server hosting that IP using standard BGP path cost metrics.
  * **Use Cases:**
    * **CDNs (Content Delivery Networks):** To serve static assets from the nearest edge location.
    * **DNS Providers (e.g., Cloudflare 1.1.1.1, Google 8.8.8.8):** To distribute the DNS query load globally and mitigate Distributed Denial of Service (DDoS) attacks.

---

### Q50. Explain BGP Hijacking.
* **Answer:**
  * **BGP Hijacking:** An attack where a malicious Autonomous System (AS) falsely advertises IP prefix blocks that it does not own.
  * **Impact:** Global routers accept the route announcement and update their routing tables. Traffic meant for the legitimate IP address ranges is redirected to the attacker's AS, enabling them to eavesdrop, drop traffic, or serve spoofed clone sites.

---

### Q51. What is the difference between a Gateway and a Router?
* **Answer:**
  * **Router (Layer 3):** Connects two or more logically distinct subnets using the same protocol stack (e.g., routing traffic between local IP subnet `192.168.1.0/24` and WAN IP `203.0.113.1`).
  * **Gateway (Cross-Layer):** Acts as a protocol translator. It connects systems that use completely incompatible protocol stacks or data formats (e.g., converting legacy cellular voice protocols to VoIP packets, or translating data link protocols between Ethernet and Token Ring).

---

### Q52. Explain standard MTU size. What is a Jumbo Frame? When is it used?
* **Answer:**
  * **Standard MTU (Maximum Transmission Unit):** The default limit on Ethernet networks is **1500 bytes**.
  * **Jumbo Frame:** Any Ethernet frame containing a payload larger than 1500 bytes, typically scaled up to **9000 bytes**.
  * **When used:** Used inside closed high-speed local data center networks (like SAN storage networks or database replication clusters).
  * **Benefit:** Sending 9000-byte packets instead of 1500-byte packets reduces the packet count by 6x for the same volume of data, significantly lowering the CPU interrupt overhead required to parse headers on networking cards.

---

### Q53. Explain the TCP Window Scaling option. Why is it necessary for Long Fat Networks (LFNs)?
* **Answer:**
  * **Limit:** The standard TCP header allocates only 16 bits for the window size, limiting the maximum receiver window to $2^{16} - 1 = 65,535$ bytes (64 KB).
  * **Long Fat Networks (LFNs):** Networks with high bandwidth and high latency (e.g., transatlantic fiber or satellite links). To maximize throughput, the network path's **Bandwidth-Delay Product (BDP)** ($BDP = Bandwidth \times RTT$) must be filled with packets. If the BDP is 5 MB, but the TCP window limit is only 64 KB, the sender stops transmitting, waiting for ACKs, wasting $98\%$ of the link capacity.
  * **Window Scaling (WS):** An option negotiated during the 3-way handshake that shifts the 16-bit window field left by up to 14 bits (a scale factor of up to $2^{14} = 16,384$), allowing maximum windows of up to $1\text{ GB}$.

---

### Q54. What is the difference between Go-Back-N (GBN) and Selective Repeat (SR) sliding window protocols?
* **Answer:**
  * **Go-Back-N:**
    * The receiver maintains a window of size 1. It only accepts packets in strict sequential order.
    * If packet 3 is lost, but packets 4 and 5 arrive, the receiver discards 4 and 5.
    * The sender's timer for packet 3 expires, forcing the sender to retransmit packet 3 and **all subsequent packets** (Go Back N).
  * **Selective Repeat:**
    * The receiver maintains a buffer window larger than 1. It accepts and buffers out-of-order packets (e.g., storing 4 and 5 while waiting for 3).
    * The sender retransmits **only** the specific packet that was lost (packet 3) once its timer expires or a NACK is received.
    * More memory efficient on the wire, but requires receiver-side buffer memory.

---

### Q55. Explain the "Count to Infinity" problem in Distance-Vector routing. How does Split Horizon resolve it?
* **Answer:**
  * **Count to Infinity:** Occurs when a link fails, and two adjacent routers exchange updates based on stale routing information. Router A thinks Router B has a route, and Router B thinks Router A has a route, creating a loop where the metric (hop count) increments indefinitely (up to RIP's infinity limit of 16).
  * **Split Horizon:** A routing rule stating that a router must **not** advertise a route back out of the same interface from which it learned that route.
    * *Example:* If Router A learns a route to subnet X from Router B, Split Horizon prevents A from advertising route X back to B, stopping routing loops.

---

### Q56. Explain core BGP Path Attributes and how they determine route selection.
* **Answer:**
  * BGP uses attributes to select the best path across Autonomous Systems (AS):
    1. **AS-Path:** A list of all AS numbers the route has traversed. BGP prefers the route with the shortest AS-Path.
    2. **Next-Hop:** Specifies the IP address of the border router to reach the next AS.
    3. **Local Preference:** Used to configure outbound routing policies within an AS (higher values preferred).
    4. **MED (Multi-Exit Discriminator):** Used to suggest preferred ingress points to external neighbors (lower values preferred).

---

### Q57. What is the difference between Recursive and Iterative DNS queries?
* **Answer:**
  * **Recursive Query:** The client asks a DNS server (typically a recursive resolver) to return the final resolved IP address. The resolver takes complete responsibility, querying other nameservers on behalf of the client.
  * **Iterative Query:** The DNS client queries a nameserver, and the nameserver returns the address of the next nameserver in the hierarchy (e.g., "I don't know, but ask the TLD nameserver at IP X") instead of resolving it itself.

---

### Q58. What is DNS Caching? At what different levels does DNS caching occur?
* **Answer:**
  * **DNS Caching:** Storing hostname-to-IP mappings temporarily to bypass DNS resolution delays.
  * **Levels of DNS Caching:**
    1. **Browser Cache:** Browsers cache records locally (e.g., Chrome cache).
    2. **OS/Hosts Cache:** Operating system DNS client cache.
    3. **Router Cache:** Home/office gateway routers cache queries.
    4. **Recursive Resolver Cache:** ISP or public resolver (e.g., `8.8.8.8`) caches queries to avoid traversing root/TLD servers.

---

### Q59. What state is a TCP socket in when it receives a SYN packet? Explain the transition states.
* **Answer:**
  * **Initial State:** Server socket is in the `LISTEN` state.
  * **SYN Received:** When the server receives a `SYN` packet from a client, it transitions to the **`SYN_RCVD`** (SYN Received) state.
  * **Transition:** The server allocates resource metadata (or uses SYN cookies), transmits a `SYN-ACK`, and waits for the client's final `ACK`. Once the `ACK` arrives, the socket transitions to **`ESTABLISHED`**.

---

### Q60. How does a SYN Cookie cryptographically encode connection parameters?
* **Answer:**
  * To prevent SYN floods, the server does not store connection state in memory when it receives a `SYN`. Instead, it generates a cryptographic cookie and uses it as the initial sequence number ($ISN$) in the `SYN-ACK`:
    $$ISN = \text{Hash}(SrcIP, DstIP, SrcPort, DstPort, SecretKey) + L$$
    Where $L$ is a timestamp counter protecting against replay attacks.
  * **Verification:** When the client returns the final `ACK` (with `Acknowledge Number = ISN + 1`), the server subtracts 1, regenerates the hash using the packet's IP/port details, and verifies it. If it matches, the connection is instantiated.

---

### Q61. What are the differences between IPv4 and IPv6 header structures? Name 3 fields removed in IPv6.
* **Answer:**
  * IPv6 headers have a fixed size of 40 bytes to simplify hardware parsing.
  * **Removed Fields:**
    1. **Header Length (IHL):** No longer needed since the IPv6 header is fixed at 40 bytes.
    2. **Identification / Flags / Fragment Offset:** Moved to optional Extension Headers. IPv6 routers do not fragment packets; source hosts must perform fragmentation.
    3. **Header Checksum:** Removed to speed up routing. Relies on Layer 2 and Layer 4 integrity checks instead.

---

### Q62. Explain the TLS 1.3 handshake optimization (1-RTT) and how it compares to TLS 1.2 (2-RTT).
* **Answer:**
  * **TLS 1.2 Handshake (2-RTT):** Requires two network round-trips:
    * *RTT 1:* Negotiate cipher suites, exchange random bytes, share public key parameters.
    * *RTT 2:* Exchange key shares, verify certificates, and activate symmetric encryption.
  * **TLS 1.3 Handshake (1-RTT):** The client guesses the key exchange algorithm and sends its public key share directly in the first `ClientHello` message. The server responds with its key share, certificate, and a Finished flag in `ServerHello`, completing key setup in a single RTT.

---

### Q63. What is QUIC (Quick UDP Internet Connections)? Why does it run over UDP instead of TCP?
* **Answer:**
  * **QUIC:** A multiplexed transport protocol designed by Google that forms the foundation of HTTP/3.
  * **Why UDP:** Modifying TCP requires updating OS kernels worldwide, which is slow. UDP is supported by all middleboxes (firewalls, routers). By running over UDP, QUIC can implement custom transport-layer features entirely in application space.
  * **Key Features:**
    * **Connection Migration:** Connections are identified by a 64-bit Connection ID, not the 4-tuple IP/Port. If a client switches from Wi-Fi to Cellular, the connection stays open.
    * **No Head-of-Line Blocking:** Multiplexed streams are independent; packet loss in stream A does not stall stream B.

---

### Q64. What is the difference between REST and gRPC in modern API communication?
* **Answer:**
  * **REST (Representational State Transfer):**
    * Uses HTTP/1.1 or HTTP/2.
    * Payload format is text-based (JSON/XML).
    * Loose coupling; easy to test but higher bandwidth overhead.
  * **gRPC (Google Remote Procedure Call):**
    * Runs strictly over HTTP/2.
    * Payload format is serialized binary (**Protocol Buffers**).
    * Strong contract definition (compiled `.proto` files). High performance, low bandwidth, and supports bidirectional streaming.

---

### Q65. What is a CORS (Cross-Origin Resource Sharing) Preflight Request? What HTTP method does it use?
* **Answer:**
  * **Preflight Request:** A safety mechanism where the browser sends an initial request before sending the actual cross-origin request.
  * **Why:** To check if the target server understands and permits the cross-origin call before sending potentially modifying data.
  * **HTTP Method:** Uses the **`OPTIONS`** method.
  * **Response:** The server returns headers like `Access-Control-Allow-Methods` and `Access-Control-Allow-Origin`. If approved, the browser sends the actual request (e.g., `POST` or `PUT`).

---

### Q66. Explain the difference between Symmetric and Asymmetric Routing in BGP.
* **Answer:**
  * **Symmetric Routing:** Data packets traveling from Host A to Host B, and return packets from Host B to Host A, follow the exact same path/routers.
  * **Asymmetric Routing:** Outbound packets (A $\rightarrow$ B) and inbound return packets (B $\rightarrow$ A) follow different physical paths across Autonomous Systems. This is common in BGP due to independent hot-potato routing policies configured by different ISPs.

---

### Q67. How does a Stateful Firewall track TCP handshakes in its connection table?
* **Answer:**
  * A stateful firewall maintains a **State Table** tracking connection tuples (`SrcIP, DstIP, SrcPort, DstPort, Protocol`).
  * **Handshake tracking:**
    1. **SYN:** Transitions connection state to `SYN_SENT`.
    2. **SYN-ACK:** Transitions connection state to `SYN_RCVD`.
    3. **ACK:** Transitions state to `ESTABLISHED`.
  * **Traffic matching:** Once established, any incoming packets matching the active state table tuple are allowed through. If a packet arrives with the `ACK` flag set but no matching entry in the table, it is dropped.

---

### Q68. What is the difference between a Socket Descriptor and a File Descriptor in Unix-based operating systems?
* **Answer:**
  * **In Unix, everything is a file:** Both sockets and files are represented by an integer reference index in the process descriptor table called a **File Descriptor (FD)**.
  * **Differences:**
    * **File Descriptor:** Represents an active stream to a static physical file, directory, or pipe. Operations are strictly read, write, seek, and close.
    * **Socket Descriptor:** A specific type of file descriptor created via `socket()`. It is bound to a network address and cannot perform random-access offsets (`lseek()`). It supports network-specific operations like `connect()`, `bind()`, `accept()`, and `listen()`.

---

### Q69. Explain standard TCP Congestion Control algorithms (Tahoe, Reno, Cubic). How do they differ?
* **Answer:**
  * **TCP Tahoe:** If a packet loss occurs (timeout or 3 duplicate ACKs), `cwnd` is reset to 1 MSS, and `ssthresh` is set to `cwnd / 2`.
  * **TCP Reno:** Adds **Fast Recovery**. If a loss occurs via 3 duplicate ACKs, `cwnd` is halved (`cwnd = cwnd / 2`) and ssthresh is set to the new `cwnd`, bypassing the slow start phase. If a timeout occurs, it still resets `cwnd` to 1.
  * **TCP Cubic:** Used by default in Linux. Instead of growing `cwnd` linearly during congestion avoidance, it uses a cubic growth function. It scales up window sizes rapidly when the network is stable, slowing down as it approaches the previous limit to prevent sudden packet drops.

---

### Q70. Compare HTTP/1.0, HTTP/1.1, HTTP/2, and HTTP/3.
* **Answer:**
  * **HTTP/1.0:** Opens a brand new TCP connection for every single request/response cycle, creating high connection overhead.
  * **HTTP/1.1:** Introduced **Persistent Connections** (`Connection: keep-alive`), allowing a single TCP connection to serve multiple requests sequentially. Vulnerable to request-level Head-of-Line (HoL) blocking.
  * **HTTP/2:** Introduced **Multiplexing** over a single TCP connection using binary framing. Eliminates request-level HoL blocking but is vulnerable to TCP-level packet loss HoL blocking.
  * **HTTP/3:** Replaces TCP with **QUIC** (over UDP). Streams are independent, eliminating TCP-level HoL blocking.

---

### Q71. Explain HTTP Pipelining vs. Multiplexing.
* **Answer:**
  * **HTTP Pipelining (HTTP/1.1):** A client sends multiple requests over a single TCP connection without waiting for responses. The server **must** return responses in the exact order the requests were received. If the first response is delayed, all subsequent responses are blocked (Head-of-Line blocking).
  * **Multiplexing (HTTP/2):** Client and server break requests and responses into independent binary frames and interleave them over a single connection. The server can return responses out-of-order as soon as they are ready.

---

### Q72. What is the difference between a Public IP and a Private IP? Name the private IP ranges reserved under RFC 1918.
* **Answer:**
  * **Public IP:** Globally unique IP addresses routed across the public Internet.
  * **Private IP:** Non-routable IP addresses used inside local area networks (LANs). Local routers drop private IP packets if they try to enter the public Internet.
  * **RFC 1918 Reserved Ranges:**
    * **Class A:** `10.0.0.0` to `10.255.255.255`
    * **Class B:** `172.16.0.0` to `172.31.255.255`
    * **Class C:** `192.168.0.0` to `192.168.255.255`

---

### Q73. Explain the mechanism of ARP Cache Poisoning (ARP Spoofing).
* **Answer:**
  * **ARP Spoofing:** An attacker sends unsolicited, forged ARP responses to local devices (e.g., telling the gateway router: *"I have IP 192.168.1.10 (Target Host)"* and telling the host: *"I have IP 192.168.1.1 (Gateway Router)"*).
  * **Mechanism:** Since ARP is stateless and lacks authentication, local devices accept these messages and update their ARP caches.
  * **Impact:** All traffic between the host and the gateway routes through the attacker's machine, enabling Man-in-the-Middle (MitM) eavesdropping, credential harvesting, or modification of data in transit.

---

### Q74. Compare Link-State (OSPF) vs. Distance-Vector (RIP) routing protocols in terms of convergence.
* **Answer:**
  * **RIP (Distance-Vector) Convergence:** Slow. RIP routers advertise tables periodically (e.g., every 30 seconds). A route failure takes time to propagate, leading to loops (Count to Infinity) before the network stabilizes.
  * **OSPF (Link-State) Convergence:** Extremely fast. OSPF routers send updates (LSAs) immediately when a link state changes. Since every router maintains a complete topology map, they quickly re-run Dijkstra's algorithm to compute alternative paths.

---

### Q75. What is the purpose of the TCP `RST` (Reset) packet? When is it sent?
* **Answer:**
  * **RST (Reset):** A TCP control flag indicating that the sender wants to abort the connection immediately and release resources.
  * **When sent:**
    1. A connection request (SYN) is received on a port where no service is listening.
    2. A packet arrives for a socket connection that has already closed or does not exist (e.g., after a server reboot, the server rejects old packets).
    3. Firewalls or security devices inject RST packets to force-close connections matching malicious traffic rules.
