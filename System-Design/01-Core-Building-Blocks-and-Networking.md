# Master Guide 01: Core Building Blocks & Networking

> **Focus:** Load Balancers, Reverse Proxies vs. API Gateways, CDNs, Caching Engines & Strategies, Cache Failure Modes, and Communication Protocols in System Design.
> 
> *Targeted for Top-Tier Tech Engineering & Internship Interviews.*

---

# Table of Contents
1. [Load Balancers: Layer 4 vs. Layer 7](#1-load-balancers-layer-4-vs-layer-7)
2. [Reverse Proxy vs. Forward Proxy vs. API Gateway](#2-reverse-proxy-vs-forward-proxy-vs-api-gateway)
3. [Content Delivery Networks (CDN) & Edge Caching](#3-content-delivery-networks-cdn--edge-caching)
4. [Caching Strategies & Invalidation](#4-caching-strategies--invalidation)
5. [Cache Failure Modes: Stampede, Penetration & Breakdown](#5-cache-failure-modes-stampede-penetration--breakdown)
6. [Communication Protocols in System Design](#6-communication-protocols-in-system-design)
7. [Glanceable Summary Matrix](#7-glanceable-summary-matrix)

---

# 1. Load Balancers: Layer 4 vs. Layer 7

### 1. The "Aha!" Intuition
Think of an airport security checkpoint. If 500 travelers arrive at once, a security coordinator stands at the entrance and directs each person to open lanes so no single line is overwhelmed. A **Load Balancer** is that traffic coordinator for incoming web requests.

### 2. ASCII Architecture Flowchart
```
                                       +---> [ Web Server 1 (CPU: 20%) ]
                                       |
[ 10,000 Clients ] ---> [ Load Balancer ] ---> [ Web Server 2 (CPU: 25%) ]
   (Internet)             (L4 / L7)    |
                                       +---> [ Web Server 3 (CPU: 18%) ]
                                             (Stateless Application Tier)
```

### 3. Layer 4 vs. Layer 7 Comparison
```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | LAYER 4 LOAD BALANCING (NLB)          | LAYER 7 LOAD BALANCING (ALB)       |
+---------------------------------------------------------------------------------------------------+
| Operating Layer      | Transport Layer (TCP / UDP)           | Application Layer (HTTP / HTTPS)   |
| Routing Criteria     | IP Address + Port Number only         | URL Path (`/api` vs `/img`),       |
|                      | (Zero packet inspection)              | HTTP Headers, Cookies, JWT Claims  |
| Latency & Throughput | Ultra-fast (Sub-millisecond latency,  | Slightly higher compute overhead,  |
|                      | millions of requests per second)      | rich intelligent routing decisions |
| SSL/TLS Termination  | Passes encrypted TCP stream through   | Terminates SSL/TLS at the balancer |
| Real-World Example   | AWS Network Load Balancer (NLB), IPVS | AWS Application Load Balancer, NGINX|
+---------------------------------------------------------------------------------------------------+
```

### 4. Common Load Balancing Algorithms
- **Round Robin:** Sends request #1 to Server A, #2 to Server B, #3 to Server C sequentially. Best when all backend servers have identical CPU/RAM specs and tasks take equal time.
- **Weighted Round Robin:** Sends $3\times$ more traffic to an 8-core server than a 2-core server.
- **Least Connections:** Inspects active TCP connections and routes to the server currently handling the fewest active users. Best for long-running requests (e.g. video uploads or WebSockets).
- **IP Hash / Sticky Sessions:** Hashes the client's IP to always pin that user to the same server. Best for legacy apps with non-distributed session memory.

### 5. The Interview Trap
Assuming load balancers eliminate all bottlenecks. **The load balancer itself can become a Single Point of Failure (SPOF)** if you only deploy one. In real production, you deploy redundant load balancers using **DNS Anycast** or **Active-Passive failover (VRRP / Keepalived)** with a virtual floating IP.

### 6. 30-Second Verbal Script
> **"A Load Balancer distributes incoming network traffic across a cluster of healthy backend servers to maximize throughput, minimize latency, and eliminate single points of failure. Layer 4 balancers route raw TCP packets based on IP and port with near-zero latency, while Layer 7 balancers inspect HTTP headers, cookies, and paths to perform smart application-level routing."**

### 7. Real-World Product Example
**Uber:** When thousands of riders request rides simultaneously, Uber's Layer 7 load balancers inspect incoming HTTP paths: routing `/api/rider/*` to the Rider Matching cluster and `/api/driver/*` to the Driver Telemetry cluster.

### 8. Follow-up They'll Throw at You
- *How does the load balancer know a backend server crashed?* **By running periodic active health checks (e.g. sending `GET /healthz` every 5 seconds) and automatically removing unresponsive nodes from the active pool.**

---

# 2. Reverse Proxy vs. Forward Proxy vs. API Gateway

### 1. The "Aha!" Intuition
- **Forward Proxy:** A student union lawyer who represents the **students** (clients) when talking to the outside world, hiding their identities.
- **Reverse Proxy:** A hotel front-desk receptionist who represents the **hotel** (servers), greeting guests, checking IDs, and directing them to rooms.
- **API Gateway:** The hotel concierge who not only greets you, but checks your VIP pass, translates foreign languages, charges your credit card, and books your spa appointment in one step.

### 2. ASCII Architecture Flowchart
```
FORWARD PROXY:
[ Client A ] ---\
[ Client B ] ----> [ Forward Proxy ] ---> (Internet) ---> [ Public Web Server ]
[ Client C ] ---/  (Hides Client IP /
                    Bypasses Firewalls)

REVERSE PROXY & API GATEWAY:
(Internet) ---> [ API Gateway / Reverse Proxy ] 
                     |-- 1. SSL/TLS Termination
                     |-- 2. JWT Authentication & Rate Limiting
                     |-- 3. Request Routing / Aggregation
                     |
                     +---> [ Auth Microservice ]
                     +---> [ Payments Microservice ]
                     +---> [ Inventory Microservice ]
```

### 3. The Feature Matrix
```
+---------------------------------------------------------------------------------------------------+
| FEATURE              | FORWARD PROXY               | REVERSE PROXY          | API GATEWAY         |
+---------------------------------------------------------------------------------------------------+
| Sits in Front of     | Clients (Internal network)  | Servers (Data center)  | Microservice APIs   |
| Primary Role         | Anonymity, Caching, Content | Load balancing, SSL,   | Auth, Rate limit,   |
|                      | filtering / Firewall bypass | Caching, DDoS defense  | Routing, Aggregation|
| Protocol Translation | No                          | Minimal                | Yes (gRPC to REST)  |
| Request Aggregation  | No                          | No                     | Yes (Combines 3 APIs|
|                      |                             |                        | into 1 response)    |
| Popular Tools        | Squid, Shadowsocks          | NGINX, HAProxy, Envoy  | Kong, Traefik, AWS  |
+---------------------------------------------------------------------------------------------------+
```

### 4. The Interview Trap
Confusing a standard Reverse Proxy (like basic NGINX) with a modern API Gateway. A reverse proxy handles low-level transport tasks (SSL termination, basic proxying). An API Gateway is application-aware, executing **JWT authentication, dynamic rate limiting, billing telemetry, and API composition**.

### 5. 30-Second Verbal Script
> **"A Forward Proxy acts on behalf of clients to access the internet while masking client IPs. A Reverse Proxy acts on behalf of backend servers to handle load balancing and SSL termination. An API Gateway sits as the single entry point in front of microservices, centralizing cross-cutting concerns like JWT validation, rate limiting, and request transformation."**

### 6. Real-World Product Example
**Spotify:** The Spotify mobile app makes a single request to the API Gateway. The Gateway validates the user's OAuth token and simultaneously fetches song metadata, album artwork URLs, and friend activity from 3 separate internal microservices, returning one unified JSON payload to the phone.

### 7. Follow-up They'll Throw at You
- *What is the risk of putting an API Gateway in front of everything?* **It can become a monolithic bottleneck and a single point of failure; mitigate this by keeping the Gateway stateless and autoscaling it horizontally behind a Layer 4 load balancer.**

---

# 3. Content Delivery Networks (CDN) & Edge Caching

### 1. The "Aha!" Intuition
If you live in London and order a pizza from an authentic kitchen in Naples, Italy, it will take 14 hours to arrive cold. Instead, the pizza chain opens a local franchise store in London that stocks pre-baked pizzas. A **CDN** is that local franchise for website assets.

### 2. ASCII Architecture Flowchart
```
[ User in Tokyo ] ---> (Requests video.mp4) ---> [ Tokyo CDN Edge Server ]
                                                        |
                                       +----------------+----------------+
                                       | (Cache HIT! ~10ms)              | (Cache MISS! ~180ms)
                                       v                                 v
                            [ Immediate Delivery ]            [ Fetch from US Origin (AWS S3) ]
                                                                         |
                                                              [ Store in Tokyo CDN Cache ]
```

### 3. Push vs. Pull CDN Architectures
```
+---------------------------------------------------------------------------------------------------+
| CDN TYPE             | HOW CONTENT GETS TO THE EDGE        | BEST INTERVIEW USE CASE              |
+---------------------------------------------------------------------------------------------------+
| Pull CDN             | Edge server requests content from   | High-traffic websites with dynamic,  |
| (Origin Pull)        | origin ONLY when a user asks for it | frequently accessed assets (default) |
+---------------------------------------------------------------------------------------------------+
| Push CDN             | Content is uploaded to ALL edge     | Software releases, video game patches|
|                      | servers in advance from origin      | where first-download latency matters |
+---------------------------------------------------------------------------------------------------+
```

### 4. Key CDN Mechanics: Origin Shielding & Anycast
- **Time to First Byte (TTFB):** Drastically reduced because physical speed-of-light fiber latency drops from $200\text{ms}$ (across continents) to $5\text{--}10\text{ms}$ (local city PoP).
- **Origin Shielding:** A central caching tier between edge PoPs and the origin server that prevents 50 global edge locations from overwhelming the origin during cache misses.
- **Anycast Routing:** Multiple global edge servers share the exact same public IP address; internet BGP routing automatically sends the client's packet to the geographically nearest server.

### 5. The Interview Trap
Proposing a CDN for dynamic, real-time personalized data (e.g. private user bank balances). **CDNs are designed for static, public, or semi-static media** (images, video segments, CSS/JS bundles).

### 6. 30-Second Verbal Script
> **"A Content Delivery Network is a globally distributed network of edge proxy servers that cache static assets close to end users geographically. This slashes latency (TTFB), conserves origin server bandwidth, and shields the database against massive traffic spikes using Anycast routing."**

### 7. Real-World Product Example
**Netflix:** Netflix built its own custom CDN called **Open Connect**. They install physical storage appliances directly inside local ISPs worldwide, caching popular 4K movies so 95%+ of video streams never touch Netflix's core AWS servers.

### 8. Follow-up They'll Throw at You
- *How do you invalidate an outdated file in a CDN before its TTL expires?* **Either trigger an API Purge request across all edge nodes, or use Cache Busting / File Versioning in URLs (`style.v2.css` or `image.png?hash=a1b2`).**

---

# 4. Caching Strategies & Invalidation

### 1. The "Aha!" Intuition
Reading data from disk is like walking to the library across town to look up a fact (10 milliseconds). Reading from in-memory RAM cache (Redis) is like having a sticky note on your desk (100 nanoseconds). RAM is **100,000x faster than disk**.

### 2. ASCII Architecture Flowchart
```
1. CACHE-ASIDE (Lazy Loading - Read Path):
[ App Server ] ---> 1. Read Cache? ---> (Hit? Return Data!)
      |                                       | (Miss)
      +------------> 2. Read from Database <--+
      |
      +------------> 3. Write data to Cache for future reads

2. WRITE-THROUGH (Write Path):
[ App Server ] ---> 1. Write Cache ---> 2. Cache writes to DB synchronously

3. WRITE-BACK / WRITE-BEHIND (Write Path):
[ App Server ] ---> 1. Write Cache (Returns immediately!)
                          |
                          v (Asynchronous batch flush every 5 seconds)
                     [ Database ]
```

### 3. Caching Strategy Comparison Matrix
```
+---------------------------------------------------------------------------------------------------+
| STRATEGY             | WRITE FLOW                           | READ FLOW             | TRADEOFF            |
+---------------------------------------------------------------------------------------------------+
| Cache-Aside (Lazy)   | App writes directly to DB;           | App checks cache; on  | Stale reads possible|
|                      | invalidates/deletes cache key        | miss, reads DB & sets | if updates bypass   |
+---------------------------------------------------------------------------------------------------+
| Write-Through        | App writes to Cache; Cache writes    | App reads exclusively | High write latency  |
|                      | to DB synchronously before returning | from cache            | (Waits for DB write)|
+---------------------------------------------------------------------------------------------------+
| Write-Back (Behind)  | App writes to Cache only; Cache      | App reads exclusively | Risk of data loss if|
|                      | flushes to DB in async batches       | from cache            | cache node crashes  |
+---------------------------------------------------------------------------------------------------+
```

### 4. Cache Eviction Policies: LRU vs. LFU vs. FIFO
- **LRU (Least Recently Used):** Discards the item that hasn't been read in the longest time. (The industry standard for general workloads).
- **LFU (Least Frequently Used):** Discards the item with the lowest historical hit count. (Best for permanent popular items vs. one-time spikes).
- **FIFO (First In, First Out):** Discards the oldest item regardless of usage.

### 5. The Interview Trap
Writing code that updates the database and then attempts to *update* the cache value. In concurrent systems, two simultaneous writes can cause race conditions where old data overwrites new data. **Senior Practice: Always DELETE the cache key on DB update rather than updating it**, forcing the next read to fetch fresh state.

### 6. 30-Second Verbal Script
> **"Caching stores hot data in in-memory RAM (like Redis) to reduce read latency and shield primary databases. In Cache-Aside, the application coordinates reads and writes directly, falling back to the database on a cache miss. Write-Through guarantees consistency by updating the database synchronously, while Write-Back buffers writes in memory for high-throughput batching at the risk of volatility."**

### 7. Real-World Product Example
**Twitter / X:** Twitter uses in-memory Redis clusters to store the Home Timelines (list of tweet IDs) for active users. When you open the app, your timeline is served in sub-milliseconds straight from RAM.

### 8. Follow-up They'll Throw at You
- *What happens when your Redis cache runs out of memory?* **It enforces its configured eviction policy (e.g. `volatile-lru` or `allkeys-lru`) to purge cold keys, or rejects new writes with an Out of Memory error.**

---

# 5. Cache Failure Modes: Stampede, Penetration & Breakdown

```
+---------------------------------------------------------------------------------------------------+
| FAILURE MODE         | WHAT HAPPENS UNDER THE HOOD          | THE PRODUCTION DEFENSE / FIX        |
+---------------------------------------------------------------------------------------------------+
| Cache Stampede       | A super-hot key expires (e.g. World  | Use Distributed Mutex Locks so only |
| (Thundering Herd)    | Cup final score); 100,000 concurrent | 1 request queries the DB while others|
|                      | requests all miss cache and slam DB  | wait, or use probabilistic early TTL|
+---------------------------------------------------------------------------------------------------+
| Cache Penetration    | Attackers query non-existent keys    | 1. Use a Bloom Filter at cache layer|
|                      | (`id = -9999`); cache misses every   | 2. Cache `NULL` values with short   |
|                      | time and passes 100% queries to DB   |    TTL (e.g. 60 seconds)            |
+---------------------------------------------------------------------------------------------------+
| Cache Breakdown      | Multiple different cache keys expire | Add Jitter (random variance of      |
| (Avalanche)          | at the EXACT same timestamp, causing | $\pm 10\%$ seconds) to TTLs so keys |
|                      | a massive wave of simultaneous misses| expire smoothly over time           |
+---------------------------------------------------------------------------------------------------+
```

### ASCII Flowchart: Bloom Filter Defending Against Cache Penetration
```
[ Incoming Query: id = 999999 ] ---> [ Bloom Filter in RAM ]
                                              |
                          +-------------------+-------------------+
                          | (Key definitely does NOT exist!)      | (Key MIGHT exist)
                          v                                       v
                  [ Fast 404 Rejection ]                 [ Check Redis Cache ]
                   (DB never touched!)                            |
                                                         (Miss? Check Database)
```

---

# 6. Communication Protocols in System Design

### 1. The "Aha!" Intuition
- **HTTP/REST:** Sending a letter and waiting for a reply.
- **WebSocket:** A continuous open telephone call where both people can talk simultaneously.
- **gRPC:** Speaking in ultra-compact shorthand Morse code over a dedicated tunnel.
- **Server-Sent Events (SSE):** Listening to a radio broadcast where the DJ talks to you, but you can't talk back.

### 2. Protocol Decision Matrix
```
+---------------------------------------------------------------------------------------------------+
| PROTOCOL             | TRANSPORT / FORMAT                   | DUPLEX / PERSISTENCE | BEST USE CASE        |
+---------------------------------------------------------------------------------------------------+
| REST (HTTP/1.1)      | TCP, Plaintext JSON                  | Half-Duplex, Request | Public APIs, CRUD ops|
|                      |                                      | and Response         | standard mobile apps |
+---------------------------------------------------------------------------------------------------+
| WebSockets           | TCP, Bidirectional Frames            | Full-Duplex, Single  | Real-time chat, live |
|                      |                                      | Persistent Connection| multiplayer gaming   |
+---------------------------------------------------------------------------------------------------+
| gRPC                 | HTTP/2, Binary Protocol Buffers      | Full-Duplex, Binary  | High-performance     |
|                      | (Protobuf)                           | Streaming & Multiplex| internal microservice|
+---------------------------------------------------------------------------------------------------+
| Server-Sent Events   | HTTP, Text Event Stream              | Unidirectional       | Live stock tickers,  |
| (SSE)                |                                      | (Server -> Client)   | LLM token streaming  |
+---------------------------------------------------------------------------------------------------+
```

### 3. Real-World Product Example
**Discord:** Discord uses **WebSockets** to deliver instant chat messages and typing indicators between users, **REST** for user account settings and profile updates, and **gRPC** internally between its Go/Rust microservices for lightning-fast voice server coordination.

---

# 7. Glanceable Summary Matrix

```
+--------------------------------------------------------------------------------------------------------------------+
| BUILDING BLOCK       | THE FUN MENTAL MODEL             | WHEN TO PICK IN SYSTEM DESIGN INTERVIEW                  |
+--------------------------------------------------------------------------------------------------------------------+
| Layer 4 LB           | Express highway tollbooth        | Routing raw TCP streams at millions of QPS               |
+--------------------------------------------------------------------------------------------------------------------+
| Layer 7 LB           | Airport gate agent checking ticket| Routing by HTTP path (/api/v1/checkout) or cookies      |
+--------------------------------------------------------------------------------------------------------------------+
| API Gateway          | Hotel concierge with VIP badges  | Centralizing auth, rate limiting, and microservice route |
+--------------------------------------------------------------------------------------------------------------------+
| CDN (Edge)           | Local neighborhood pizza franchise| Caching static images, video segments, and JS bundles   |
+--------------------------------------------------------------------------------------------------------------------+
| Cache-Aside          | Sticky note on your desk         | General read-heavy database shielding (Redis / Memcached)|
+--------------------------------------------------------------------------------------------------------------------+
| Bloom Filter         | Memory-efficient club bouncer    | Instantly rejecting non-existent keys to save DB queries |
+--------------------------------------------------------------------------------------------------------------------+
| WebSockets           | Open phone call                  | Bidirectional instant chat and multiplayer gaming        |
+--------------------------------------------------------------------------------------------------------------------+
| gRPC                 | Compact binary shorthand         | Ultra-low latency communication between microservices    |
+--------------------------------------------------------------------------------------------------------------------+
```
