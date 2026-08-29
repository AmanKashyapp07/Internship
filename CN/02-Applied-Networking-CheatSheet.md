# Master Guide 02: Applied Networking & Master Interview Cheat Sheet

> **Focus:** Cookies vs. Sessions vs. JWT, Load Balancing Topologies, CDNs, Sockets, The Canonical "Type a URL & Hit Enter" 60-Second Script, 15 Rapid-Fire Verbal Q&As, and Top Red-Flag Networking Mistakes.
> 
> *Targeted for Top-Tier Tech Internship & Software Engineering Interviews.*

---

# Table of Contents
1. [Cookies vs. Sessions vs. JWT Tokens](#1-cookies-vs-sessions-vs-jwt-tokens)
2. [Load Balancing: Layer 4 vs. Layer 7](#2-load-balancing-layer-4-vs-layer-7)
3. [Content Delivery Networks (CDN) & Edge Caching](#3-content-delivery-networks-cdn--edge-caching)
4. [Sockets: The Network Endpoint Primitive](#4-sockets-the-network-endpoint-primitive)
5. [The Canonical Interview Question: "What Happens When You Type a URL & Hit Enter?"](#5-the-canonical-interview-question-what-happens-when-you-type-a-url--hit-enter)
6. [15 Rapid-Fire Networking Q&A with Bold Answers](#6-15-rapid-fire-networking-qa-with-bold-answers)
7. [Top 5 Red-Flag Networking Mistakes That Sound Junior](#7-top-5-red-flag-networking-mistakes-that-sound-junior)

---

# 1. Cookies vs. Sessions vs. JWT Tokens

```
+---------------------------------------------------------------------------------------------------+
| AUTH MECHANISM       | STORAGE LOCATION                      | STATEFUL / STATELESS | HORIZONTAL SCALING  |
+---------------------------------------------------------------------------------------------------+
| Cookie               | Client-side browser storage           | N/A (Storage format) | Automatically sent  |
|                      | (Max 4KB per cookie domain)           |                      | on every HTTP header|
+---------------------------------------------------------------------------------------------------+
| Session              | Server-side memory / Redis cache      | STATEFUL             | Requires centralized|
|                      | (Client holds only `session_id` cookie)| (Server tracks state)| session store/Redis |
+---------------------------------------------------------------------------------------------------+
| JWT (JSON Web Token) | Client-side (LocalStorage or Cookie)  | STATELESS            | Seamless horizontal |
|                      | (Signed payload: Header.Payload.Sign) | (Zero server lookup) | microservice scaling|
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** A Session is a coat-check ticket (server holds the coat, you hold a number); a JWT is a driver's license with a cryptographic hologram signature (all your info is stamped directly on the card).
- **The Interview Trap:** Saying JWTs are encrypted. **JWTs are signed, NOT encrypted** (base64 payload is publicly readable by anyone). Never put sensitive passwords or credit card numbers in a JWT payload!
- **30-Second Verbal Answer:** **"Sessions store user state in a server-side cache like Redis and identify users via a lightweight session ID cookie. JWTs are self-contained, cryptographically signed tokens containing user claims directly in the payload, enabling stateless authentication that scales effortlessly across decoupled microservices without database lookups."**
- **Follow-up:** *How do you invalidate a JWT before its expiration time?* **Because JWTs are stateless, you cannot revoke them instantly without introducing state: either maintain a short token TTL (5-15 mins) with refresh tokens, or maintain an in-memory Redis token blacklist.**

---

# 2. Load Balancing: Layer 4 vs. Layer 7

```
Layer 4 Load Balancer (Transport Layer: HAProxy, AWS NLB):
Routes raw TCP/UDP packets based ONLY on (Source IP, Source Port, Dest IP, Dest Port). Zero packet inspection.

Layer 7 Load Balancer (Application Layer: Nginx, AWS ALB):
Parses full HTTP payload; routes traffic based on HTTP Headers, Cookies, URL Paths (/api vs /static).
```

### Core Load Balancing Algorithms:
- **Round Robin:** Sequentially cycles requests across server pools (best for identical stateless servers).
- **Least Connections:** Routes traffic to the server with the fewest active TCP connections (best for long-running requests/websockets).
- **IP Hash / Sticky Sessions:** Hashes the client IP to pin a user to the same physical server (used for non-distributed legacy session architectures).

---

# 3. Content Delivery Networks (CDN) & Edge Caching

```
[ User in Tokyo ] ---> (Requests image.png) ---> [ Tokyo CDN Edge Server (Cache Hit! ~10ms) ]
                                                               |
                                              (If Cache Miss: Fetches from Origin in US ~200ms)
                                                               v
                                                 [ US Origin Server (AWS S3) ]
```

- **One-Line Intuition:** A CDN is a local franchise store stocking popular products in every city so customers don't have to wait for deliveries from the central international factory.
- **Key Concepts:** Caches static assets (images, videos, JS/CSS bundles) geographically close to end users, reducing origin server load and slashing latency (Time to First Byte / TTFB).

---

# 4. Sockets: The Network Endpoint Primitive

```
Socket Endpoint Identifier = (IP Address + Port Number + Transport Protocol)
Example: (192.168.1.10 : 8080 : TCP)
```

- **One-Line Intuition:** An IP address is the street address of an apartment building; a Port is the apartment number; a Socket is the physical door you knock on.
- **Socket Types:**
  - **Stream Sockets (`SOCK_STREAM`):** Connection-oriented, reliable byte stream over TCP.
  - **Datagram Sockets (`SOCK_DGRAM`):** Connectionless, unordered packet delivery over UDP.

---

# 5. The Canonical Interview Question: "What Happens When You Type a URL & Hit Enter?"

### The 60-Second End-to-End Master Script:

```
[ 1. URL Parsing & HSTS ]
           |
           v
[ 2. DNS Resolution ] ---------------> (Browser -> OS Cache -> ISP Resolver -> Root -> TLD -> Authoritative)
           |
           v
[ 3. TCP 3-Way Handshake ] ----------> (Client sends SYN -> Server returns SYN-ACK -> Client sends ACK)
           |
           v
[ 4. TLS Handshake (HTTPS) ] --------> (Certificate Verification -> Key Exchange -> Symmetric Session Key)
           |
           v
[ 5. HTTP Request & LB Routing ] ----> (GET /index.html sent to Load Balancer -> Routed to Web Server)
           |
           v
[ 6. Server & Database Processing ] -> (Application logic executes, queries DB, generates HTML response)
           |
           v
[ 7. HTTP 200 OK Response ] ---------> (Server sends response headers and HTML payload back to client)
           |
           v
[ 8. Browser Critical Rendering Path]  (Parse HTML to DOM -> Parse CSS to CSSOM -> Layout -> Paint)
```

---

# 6. 15 Rapid-Fire Networking Q&A with Bold Answers

1. **What is the difference between TCP and UDP?**
   - **TCP is a reliable, connection-oriented byte stream with guaranteed in-order delivery; UDP is connectionless, lightweight, and unordered with zero delivery guarantees.**
2. **What is the purpose of the TCP 3-Way Handshake?**
   - **To synchronize sequence numbers and verify that both client and server can independently send and receive data before transferring payload.**
3. **What is the difference between HTTP and HTTPS?**
   - **HTTPS is HTTP layered over TLS on port 443, using asymmetric cryptography to authenticate servers and establish a symmetric key for encrypted data transfer.**
4. **What is DNS and how does it work?**
   - **The internet's distributed directory translating domain names into IP addresses by recursively querying Root, TLD, and Authoritative name servers.**
5. **What is the difference between a Cookie and a Session?**
   - **A Cookie is client-side browser storage; a Session stores user state on the server (or Redis) and references it via a session ID cookie.**
6. **Why are JWTs preferred for microservice architectures?**
   - **Because JWTs are stateless and self-contained, allowing microservices to verify authentication signatures independently without querying a shared session database.**
7. **What is a Reverse Proxy vs. a Forward Proxy?**
   - **A Forward Proxy sits in front of clients to hide client IPs or bypass firewalls; a Reverse Proxy sits in front of servers to handle load balancing, TLS termination, and caching.**
8. **What is the difference between Layer 4 and Layer 7 Load Balancing?**
   - **Layer 4 routes packets based on IP and Port without inspecting application data; Layer 7 inspects full HTTP headers, cookies, and URL paths.**
9. **What is a CDN and what does it cache?**
   - **A Content Delivery Network is a distributed network of edge servers that cache static assets (images, JS, CSS) close to end users to minimize latency.**
10. **What is a Socket?**
    - **A software endpoint for network communication uniquely identified by the combination of an IP address, Port number, and Transport protocol.**
11. **What is the difference between HTTP/1.1 and HTTP/2?**
    - **HTTP/2 introduces binary framing and multiplexing over a single TCP connection, eliminating HTTP/1.1 Head-of-Line blocking.**
12. **Why does HTTP/3 use QUIC over UDP instead of TCP?**
    - **To eliminate TCP-level Head-of-Line packet loss blocking and enable 0-RTT connection re-establishment.**
13. **What is `TIME_WAIT` in TCP?**
    - **A client-side termination state lasting $2\times\text{MSL}$ (Maximum Segment Lifetime) to ensure the final ACK was received and drain duplicate packets from the network.**
14. **What is the difference between Symmetric and Asymmetric Encryption?**
    - **Symmetric encryption uses the same single secret key for encryption and decryption (fast); Asymmetric encryption uses a public/private key pair (slow, used for key exchange).**
15. **What is CORS (Cross-Origin Resource Sharing)?**
    - **A browser security mechanism that restricts web pages from making AJAX requests to a different domain, protocol, or port unless authorized via HTTP response headers.**

---

# 7. Top 5 Red-Flag Networking Mistakes That Sound Junior

```
+---------------------------------------------------------------------------------------------------+
| #  | THE JUNIOR MISTAKE                     | THE SENIOR CORRECTION / INTERVIEW PIVOT             |
+---------------------------------------------------------------------------------------------------+
| 1  | "Saying UDP is bad or useless because  | UDP is intentionally designed for low-latency live  |
|    | it drops packets."                     | streams and gaming where late packets are useless.  |
+---------------------------------------------------------------------------------------------------+
| 2  | "Thinking JWT payloads are encrypted." | JWTs are signed, NOT encrypted; payload data is     |
|    |                                        | plain base64 and visible to anyone inspecting it.   |
+---------------------------------------------------------------------------------------------------+
| 3  | "Stating HTTPS uses asymmetric crypto  | Asymmetric crypto is used ONLY for the handshake;   |
|    | for all data transfer."                | fast symmetric crypto encrypts the actual payload.  |
+---------------------------------------------------------------------------------------------------+
| 4  | "Confusing IP and MAC addresses."      | IP routes packets globally across networks (L3);    |
|    |                                        | MAC addresses transfer frames on a local link (L2). |
+---------------------------------------------------------------------------------------------------+
| 5  | "Blanking on the URL-to-render flow."  | Always structure into 4 phases: DNS -> TCP/TLS ->   |
|    |                                        | HTTP request/response -> Browser DOM rendering.     |
+---------------------------------------------------------------------------------------------------+
```
