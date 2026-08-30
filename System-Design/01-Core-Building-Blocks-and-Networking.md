# Core Infrastructure, Distributed Networking & Caching Architecture

> **Scope:** Layer 4 vs. Layer 7 Load Balancing Topologies, Routing & Traffic Balancing Algorithms, Proxy Systems & API Gateways, Content Delivery Networks (CDN) & Anycast Edge Caching, In-Memory Cache Coherence Patterns (Cache-Aside, Write-Through, Write-Back), Cache Resilience & Failure Modes (Stampede, Penetration, Avalanche), and Distributed Communication Protocols (HTTP/REST, WebSockets, gRPC, Server-Sent Events).

---

# Table of Contents
1. [Load Balancing Architectures: Layer 4 vs. Layer 7](#1-load-balancing-architectures-layer-4-vs-layer-7)
2. [Traffic Scheduling & Load Distribution Algorithms](#2-traffic-scheduling--load-distribution-algorithms)
3. [Proxy Architectures: Forward Proxy, Reverse Proxy & API Gateway](#3-proxy-architectures-forward-proxy-reverse-proxy--api-gateway)
4. [Content Delivery Networks (CDN) & Edge Caching Topologies](#4-content-delivery-networks-cdn--edge-caching-topologies)
5. [In-Memory Caching Topologies & Write Invalidation Strategies](#5-in-memory-caching-topologies--write-invalidation-strategies)
6. [Cache System Failure Modes & Structural Mitigations](#6-cache-system-failure-modes--structural-mitigations)
7. [Distributed Transport & Application Communication Protocols](#7-distributed-transport--application-communication-protocols)
8. [Core Architectural Summary Matrix](#8-core-architectural-summary-matrix)

---

# 1. Load Balancing Architectures: Layer 4 vs. Layer 7

A **Load Balancer** serves as a reverse proxy orchestrating the ingress distribution of network traffic across a horizontally scalable pool of compute instances to maximize throughput, minimize latency, and prevent resource saturation.

```
                                        +---> [ Application Worker 1 ]
                                        |
[ Client Ingress ] ---> [ Load Balancer ] ---> [ Application Worker 2 ]
  (TCP/HTTP Traffic)     (L4 / L7 Tier) |
                                        +---> [ Application Worker 3 ]
```

```
+----------------------------------------------------------------------------------------------------+
| ARCHITECTURAL ATTRIBUTE | LAYER 4 LOAD BALANCING (L4 / NLB)       | LAYER 7 LOAD BALANCING (L7 / ALB)       |
+----------------------------------------------------------------------------------------------------+
| Protocol Level          | Transport Layer (OSI Layer 4: TCP / UDP)| Application Layer (OSI Layer 7: HTTP)   |
| Packet Inspection       | Flow inspection: Source/Dest IP + Port  | Deep Packet Inspection: URLs, Headers,  |
|                         | (Zero payload parsing)                  | Cookies, HTTP Methods, JWT Claims       |
| Throughput & Latency    | Sub-millisecond latency; millions of    | Higher computational overhead; executes |
|                         | packets/sec per node (Kernel bypass)    | complex application-level routing logic |
| TLS / SSL Mechanics     | Transparent TCP pass-through            | TLS termination and certificate offload |
| Exemplar Implementations| IPVS, Linux LVS, AWS NLB, HAProxy TCP   | NGINX, Envoy, Traefik, AWS ALB          |
+----------------------------------------------------------------------------------------------------+
```

---

# 2. Traffic Scheduling & Load Distribution Algorithms

```
+----------------------------------------------------------------------------------------------------+
| ALGORITHM            | SCHEDULING MECHANISM                         | OPTIMAL SYSTEM WORKLOAD      |
+----------------------------------------------------------------------------------------------------+
| Round Robin          | Sequential circular pointer distribution     | Homogeneous nodes and uniform|
|                      | across active backend servers                | request execution durations  |
| Weighted Round Robin | Distributes requests proportional to node    | Heterogeneous server specs   |
|                      | capacity coefficients (e.g. CPU/RAM weights) | (e.g. 16-core vs 4-core nodes|
| Least Connections    | Routes traffic to the node with the fewest   | Long-lived persistent sessions|
|                      | active concurrent TCP connections            | (WebSockets, file streaming) |
| IP / Consistent Hash | Maps client IP hash or request key to a      | Stateful caches requiring    |
|                      | deterministic server slot                    | session or memory stickiness |
| Power of Two Choices | Picks two random servers and selects the one | Distributed load balancing   |
|                      | with lower active load                       | avoiding thundering herds    |
+----------------------------------------------------------------------------------------------------+
```

### High-Availability Load Balancer Topologies:
To eliminate the load balancer as a Single Point of Failure (SPOF):
1. **Active-Passive High Availability:** Paired load balancers share a virtual floating IP via **Virtual Router Redundancy Protocol (VRRP / Keepalived)** with automated heartbeat failover.
2. **BGP Equal-Cost Multi-Path (ECMP) & Anycast:** Upstream routers use Border Gateway Protocol (BGP) Anycast to announce the same IP address across multiple physical load balancers simultaneously.

---

# 3. Proxy Architectures: Forward Proxy, Reverse Proxy & API Gateway

```
Forward Proxy (Client-Facing Boundary):
[ Internal Client ] ---> [ Forward Proxy ] ---> (Public Internet) ---> [ Origin Server ]
                         (Egress security,
                          Caching, Anonymity)

Reverse Proxy & API Gateway (Server-Facing Boundary):
(Public Internet) ---> [ API Gateway / Reverse Proxy ]
                             |-- TLS Offloading & DDoS Filtering
                             |-- JWT Authentication & Token Validation
                             |-- Dynamic Rate Limiting (Token Bucket)
                             |-- Service Discovery & Reverse Routing
                             |
                             +---> [ Service: Identity / Auth ]
                             +---> [ Service: Transaction Engine ]
                             +---> [ Service: Catalog / Query ]
```

```
+----------------------------------------------------------------------------------------------------+
| DIMENSION            | FORWARD PROXY               | REVERSE PROXY          | API GATEWAY          |
+----------------------------------------------------------------------------------------------------+
| Deployment Boundary  | Client perimeter / LAN      | Ingress of Data Center | Edge of Microservices|
| Primary Function     | Content filtering, anonymity| TLS termination, DDoS  | Auth, rate limiting, |
|                      | and egress caching          | mitigation, proxying   | API aggregation      |
| Protocol Translation | Absent                      | Basic (HTTP -> HTTPS)  | Advanced (REST-gRPC) |
| Request Aggregation  | Absent                      | Absent                 | Fan-out / Composition|
| Reference Engines    | Squid, Envoy Egress         | NGINX, HAProxy         | Kong, Envoy, Traefik |
+----------------------------------------------------------------------------------------------------+
```

---

# 4. Content Delivery Networks (CDN) & Edge Caching Topologies

A **Content Delivery Network (CDN)** is a geographically distributed mesh of Points of Presence (PoPs) caching static and semi-static assets at network edges to reduce physical fiber latency and minimize Time to First Byte (TTFB).

```
[ Edge Client (Tokyo) ] ---> (Request: media.mp4) ---> [ Tokyo CDN Edge PoP ]
                                                               |
                                        +----------------------+----------------------+
                                        | (Cache HIT: ~5ms)                           | (Cache MISS: ~160ms)
                                        v                                             v
                             [ Direct Edge Return ]                       [ Regional Origin Shield ]
                                                                                      |
                                                                             [ Core Storage (S3) ]
```

```
+----------------------------------------------------------------------------------------------------+
| CDN ARCHITECTURE     | ASSET REPLICATION FLOW                       | OPTIMAL USE CASE             |
+----------------------------------------------------------------------------------------------------+
| Origin Pull (Lazy)   | Edge PoP fetches asset from origin storage   | Dynamic web media, user-     |
|                      | only upon an initial cache miss              | generated content, web assets|
| Push CDN (Proactive) | Storage origin pushes assets out to all edge | Software patches, OS updates,|
|                      | servers prior to client request availability | planned major media releases |
+----------------------------------------------------------------------------------------------------+
```

### Advanced Edge Invariants:
- **BGP Anycast Routing:** Global edge PoPs announce identical IP prefixes; internet BGP routing automatically converges traffic to the topologically closest edge data center.
- **Origin Shielding:** An intermediary centralized caching layer shielding backend storage origins from simultaneous multi-edge cache miss spikes.

---

# 5. In-Memory Caching Topologies & Write Invalidation Strategies

```
1. Cache-Aside (Lazy Loading):
[ Application Server ] ---> 1. Check Cache ---> (Hit: Return Data)
      |                                                | (Miss)
      +-------------------> 2. Query Database <--------+
      |
      +-------------------> 3. Populate Cache with Database Result

2. Write-Through:
[ Application Server ] ---> 1. Write Data to Cache ---> 2. Cache writes synchronously to DB

3. Write-Back (Write-Behind):
[ Application Server ] ---> 1. Write to In-Memory Cache (Immediate Acknowledgment)
                                  |
                                  v (Asynchronous periodic batch flush to DB)
                             [ Database ]
```

```
+----------------------------------------------------------------------------------------------------+
| CACHING PATTERN      | WRITE MECHANICS                      | READ MECHANICS        | SYSTEM TRADEOFF      |
+----------------------------------------------------------------------------------------------------+
| Cache-Aside (Lazy)   | Application updates DB and invalidates| App queries cache; on | Potential read of    |
|                      | (deletes) corresponding cache key    | miss, queries DB & set| stale state during race|
| Write-Through        | App writes to cache; cache executes  | App reads directly    | Increased write      |
|                      | synchronous write to database        | from cache            | latency overhead     |
| Write-Back (Behind)  | App writes to cache; cache flushes to| App reads directly    | Risk of data loss if |
|                      | DB via asynchronous batch threads    | from cache            | cache node fails     |
+----------------------------------------------------------------------------------------------------+
```

### Cache Key Invalidation Concurrency Invariant:
When modifying persistent records, systems should **delete (evict) the cache key rather than updating it**. In-place updates are vulnerable to concurrent write race conditions where out-of-order writes persist stale values into the cache permanently.

---

# 6. Cache System Failure Modes & Structural Mitigations

```
+----------------------------------------------------------------------------------------------------+
| FAILURE MODE         | ROOT CAUSE & BEHAVIOR                | ARCHITECTURAL DEFENSE                |
+----------------------------------------------------------------------------------------------------+
| Cache Stampede       | Highly concurrent access to a hot key| 1. Distributed Mutex Lock (Single DB |
| (Thundering Herd)    | during expiration; thousands of      |    worker while other requests wait) |
|                      | requests bypass cache and saturate DB| 2. Probabilistic early recomputation |
| Cache Penetration    | Queries targeting non-existent keys  | 1. In-Memory Bloom Filter evaluation |
|                      | (e.g. invalid IDs) cause 100% cache  | 2. Cache null values with short TTL  |
|                      | misses, hitting the underlying DB    |    (e.g. 60 seconds)                 |
| Cache Avalanche      | Massive set of distinct cache keys   | Add pseudorandom TTL Jitter          |
| (Mass Expiration)    | expire at identical timestamps,      | ($TTL = \text{base} \pm \Delta$) to  |
|                      | overwhelming backend databases       | smooth out expiration distribution   |
+----------------------------------------------------------------------------------------------------+
```

```
Bloom Filter Interceptor for Cache Penetration Mitigation:
[ Ingress Query ID ] ---> [ In-Memory Bloom Filter ]
                                 |
           +---------------------+---------------------+
           | (Bit array confirms non-existence)        | (Bit array confirms probable existence)
           v                                           v
    [ Reject Request: 404 ]                    [ Query Cache / Database ]
```

---

# 7. Distributed Transport & Application Communication Protocols

```
+----------------------------------------------------------------------------------------------------+
| PROTOCOL             | TRANSPORT / SERIALIZATION            | COMMUNICATION MODEL  | SYSTEM DOMAIN        |
+----------------------------------------------------------------------------------------------------+
| REST (HTTP/1.1)      | TCP, Textual JSON / XML              | Half-Duplex, Request | Public APIs, CRUD,   |
|                      |                                      | and Response         | stateless web apps   |
| WebSockets           | TCP, Framing over persistent socket  | Full-Duplex, State-  | Real-time messaging, |
|                      | (RFC 6455)                           | ful Bidirectional    | live telemetry feeds |
| gRPC                 | HTTP/2, Binary Protocol Buffers      | Full-Duplex, Stream  | Inter-microservice   |
|                      | (Protobuf)                           | Multiplexing         | internal RPC systems |
| Server-Sent Events   | HTTP/1.1 or HTTP/2, Text stream      | Unidirectional       | Push notifications,  |
| (SSE)                | (text/event-stream)                  | (Server -> Client)   | LLM token streaming  |
+----------------------------------------------------------------------------------------------------+
```

---

# 8. Core Architectural Summary Matrix

```
+----------------------------------------------------------------------------------------------------+
| INFRASTRUCTURE LAYER | PRIMARY MECHANISM                    | CORE OPERATIONAL CHARACTERISTIC      |
+----------------------------------------------------------------------------------------------------+
| Layer 4 Balancing    | Transport stream routing (IP/Port)   | Minimal latency, high packet volume  |
| Layer 7 Balancing    | Application-aware routing (HTTP/URL) | Intelligent path routing & TLS term  |
| API Gateway          | Unified microservice edge contract   | Rate limiting, authentication, proxy |
| Content Delivery Net | Anycast edge caching mesh            | Slashes TTFB, shields origin storage |
| Cache-Aside Pattern  | Application-managed memory caching   | Read-optimized database insulation   |
| Bloom Filter Shield  | Space-efficient set membership       | Prevents cache penetration attacks   |
| WebSockets Protocol  | Persistent bidirectional TCP stream  | Low-overhead real-time duplex data   |
| gRPC / Protobuf RPC  | Binary HTTP/2 multiplexed streaming  | High-throughput service-to-service   |
+----------------------------------------------------------------------------------------------------+
```
