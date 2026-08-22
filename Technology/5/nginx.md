# Nginx: Reverse Proxy, Load Balancing & SSL Termination

> **Core Concept:** Nginx (pronounced "Engine-X") is an open-source, asynchronous, event-driven HTTP server, reverse proxy, and Layer 7 / Layer 4 load balancer. Its master-worker process architecture (using non-blocking `epoll`/`kqueue`) handles tens of thousands of concurrent TCP connections with minimal memory (~2.5MB per 10k idle connections).

---

## 💬 "Say It Out Loud" in an Interview (The 30-Second Elevator Pitch)

> **When the interviewer asks:** *"What is NGINX and why/when do we use it?"*
>
> **You say:** *"Nginx is an event-driven, asynchronous reverse proxy and load balancer. Using non-blocking worker processes, it handles SSL/TLS termination, static asset delivery via Linux zero-copy sendfile, and Layer 7 request routing to protect and scale upstream backend application servers."*

---

## 1. What It Is in Plain English

When a user visits `https://cloudide.example.com/api/workspaces`, they should not connect directly to your Node.js application process running on port `3000`. Exposing Node.js directly to the raw internet is risky because:
- Node is not optimized for SSL/TLS handshakes or serving large static image/JS files.
- Slow client connections (e.g. 2G mobile networks) tie up Node.js sockets.
- You cannot easily load balance traffic across multiple backend server instances.

**Nginx sits at the edge as a Reverse Proxy:**
1. It handles public HTTPS encryption (SSL/TLS Termination).
2. It serves static HTML/JS/CSS assets directly from disk at near-wire speeds.
3. It forwards clean, unencrypted HTTP and WebSocket requests to your upstream backend services and distributes load across multiple servers.

---

## 2. Reverse Proxy & WebSocket Gateway Architecture

```
[ Public Clients (Internet) ]
         |
         v (HTTPS Port 443 / WSS)
+-----------------------------------------------------------------------------------+
|                              NGINX REVERSE PROXY                                  |
|                                                                                   |
|  - SSL/TLS Termination (Let's Encrypt / OpenSSL)                                  |
|  - Static Asset Caching (/var/www/static -> Gzip / Brotli)                        |
|  - Rate Limiting (limit_req_zone: 20 req/s)                                       |
|  - WebSocket Upgrade Headers Routing                                              |
+-----------------------------------------------------------------------------------+
         |                                                       |
         | (Fast local HTTP: port 3000)                          | (Upstream Load Balancing)
         v                                                       v
+------------------------------------+         +------------------------------------+
|       REST API EXPRESS SERVER      |         |     WEBSOCKET PTY STREAM CLUSTER   |
|                                    |         |  upstream ws_backend {             |
|  location /api {                   |         |    ip_hash;                        |
|    proxy_pass http://api_server;   |         |    server 10.0.0.1:4000;           |
|  }                                 |         |    server 10.0.0.2:4000;           |
+------------------------------------+         |  }                                 |
                                               +------------------------------------+
```

---

## 3. Analogy for Live Interviews

> *"A Forward Proxy (like a VPN) hides the **Client's** identity from the internet (the server only sees the VPN's IP). A Reverse Proxy (like Nginx) hides the **Server's** internal architecture from the client. When you walk into a hotel, you don't go to the kitchen to talk to the chef or the basement to talk to the boiler operator. You talk to the Concierge at the front desk (Nginx). The Concierge answers simple questions, verifies your ID, and directs your request to the right department behind closed doors."*

---

## 4. Nginx vs. The Alternatives

| Dimension | Nginx | Apache HTTP Server | Caddy | Envoy / HAProxy |
| :--- | :--- | :--- | :--- | :--- |
| **Architecture** | **Asynchronous / Event-driven (`epoll`)** | Process / Thread per connection | Event-driven (Go) | Event-driven (C++) |
| **Concurrency Scale** | **100k+ concurrent connections** | Degrades at > 5k connections | High | Ultra-high (Service mesh standard) |
| **SSL Automation** | Requires Certbot / manual config | Requires Certbot | **Automatic HTTPS by default** | Manual / control-plane dynamic |
| **Static File Delivery** | **Zero-copy (`sendfile` syscall)** | Moderate | High | Proxy focused |

---

## 5. Core Load Balancing Algorithms in Nginx

1. **Round Robin (Default):** Distributes incoming requests sequentially down the list of upstream servers.
2. **Least Connections (`least_conn`):** Routes the new request to the server with the fewest active, in-flight connections (ideal for long-running CI build requests or terminal sessions).
3. **IP Hash (`ip_hash`):** Uses the client's IPv4/IPv6 address as a hash key to guarantee that a specific client always routes to the **same backend server** (**Sticky Sessions** for stateful session apps).
4. **Weighted Round Robin (`server srv1 weight=3;`):** Directs $3\times$ more traffic to high-capacity hardware nodes.

---

## 6. 5–8 High-Yield Interview Questions & Direct Answers

### Q1: What is the difference between a Forward Proxy and a Reverse Proxy?
> **Answer:**
> - **Forward Proxy:** Sits in front of a **client** (or internal office network). It intercepts outbound requests, hides client IPs, bypasses geographic firewalls, and caches outbound traffic (e.g. corporate internet filters, Tor).
> - **Reverse Proxy:** Sits in front of one or more **backend origin servers**. It intercepts inbound requests from the public internet, hides server internal IPs, handles SSL termination, and balances traffic across backend servers.

### Q2: Why is the `sendfile on;` directive in Nginx so fast for serving static files?
> **Answer:** Standard file transfer reads data from disk into kernel memory, copies it to user-space application RAM, and then copies it back to kernel network socket buffers (**4 context switches + 2 data copies**). The Linux **`sendfile` system call** performs a **Zero-Copy transfer**: it instructs the kernel to stream bytes directly from the disk page cache to the network socket descriptor within kernel space, bypassing user-space RAM entirely.

### Q3: What happens if you forget `proxy_set_header Upgrade $http_upgrade;` when proxying WebSockets?
> **Answer:** Standard HTTP/1.0 and HTTP/1.1 proxies treat `Upgrade` and `Connection` as **Hop-by-Hop headers** and strip them out when forwarding requests to the backend server. The backend receives a plain HTTP `GET` request without the upgrade headers, fails to trigger the HTTP 101 Switching Protocols handshake, and the WebSocket connection fails immediately.

### Q4: How does Nginx achieve zero-downtime configuration reloads (`nginx -s reload`)?
> **Answer:** When you run `nginx -s reload`, the Master process checks the syntax of the new configuration file. It spawns **new Worker processes** running the new configuration and sends a `SIGQUIT` graceful shutdown signal to the **old Worker processes**. Old workers stop accepting new connections, finish serving their current in-flight requests, and terminate cleanly with zero dropped requests.

---

## 7. Common "Gotcha" Questions Interviewers Ask

### Gotcha 1: "Why does `proxy_pass http://backend;` lose the real client IP address in Node.js (`req.ip = 127.0.0.1`)?"
- **The Answer:** Because Nginx initiates a new TCP connection to the backend server, the backend sees Nginx's IP as the remote client.
- **The Fix:** Nginx must explicitly forward the original client IP in standard HTTP headers:
  ```nginx
  proxy_set_header X-Real-IP $remote_addr;
  proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
  proxy_set_header X-Forwarded-Proto $scheme;
  ```
  And Express must configure `app.set('trust proxy', true);`.
