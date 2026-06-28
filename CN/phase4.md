# Phase 4: Application Layer 🌐

This report covers application protocols, state management, HTTP semantics, and DNS.

---

## 1. HTTP vs. HTTPS

* **HTTP (Hypertext Transfer Protocol):** A stateless, text-based protocol that runs over TCP (typically port 80). Data is transmitted in cleartext, leaving it vulnerable to eavesdropping (man-in-the-middle attacks).
* **HTTPS (HTTP Secure):** Encapsulates HTTP traffic within an encrypted **TLS (Transport Layer Security)** connection (typically port 443).
  * *Benefits:* Provides **encryption** (confidentiality), **data integrity** (prevents tampering), and **authentication** (verifies server identity via digital certificates).

---

## 2. HTTP Methods

HTTP defines methods to specify the desired action for a given resource.

| Method | Description | Safe? | Idempotent? |
| :--- | :--- | :---: | :---: |
| **GET** | Retrieves a representation of a resource. | ✅ Yes | ✅ Yes |
| **POST**| Submits data to be processed (often creates a new resource). | ❌ No | ❌ No |
| **PUT** | Replaces the target resource or creates it if it doesn't exist. | ❌ No | ✅ Yes |
| **DELETE**| Removes the target resource. | ❌ No | ✅ Yes |
| **PATCH**| Applies partial modifications to a resource. | ❌ No | ❌ No |

* **Safe:** The method does not modify resource state on the server.
* **Idempotent:** Making multiple identical requests yields the same server state as making a single request (e.g., calling `DELETE /user/5` ten times has the same outcome as calling it once; subsequent calls return the same deleted state).

---

## 3. HTTP Status Codes

Status codes are grouped into five classes indicating request outcomes.

* **1xx (Informational):** Request received, continuing process.
  * *Example:* `101 Switching Protocols` (upgrades HTTP to WebSockets).
* **2xx (Success):** Action successfully received, understood, and accepted.
  * *Examples:* `200 OK`, `201 Created` (successful POST), `204 No Content` (successful request with empty body response).
* **3xx (Redirection):** Further action is needed to complete the request.
  * *Examples:* `301 Moved Permanently` (caches redirect), `302 Found` (temporary redirect), `304 Not Modified` (client can use cached representation).
* **4xx (Client Error):** The request contains bad syntax or cannot be fulfilled.
  * *Examples:* `400 Bad Request`, `401 Unauthorized` (needs credentials), `403 Forbidden` (authenticated but lacks access rights), `404 Not Found`, `429 Too Many Requests`.
* **5xx (Server Error):** The server failed to fulfill an apparently valid request.
  * *Examples:* `500 Internal Server Error`, `502 Bad Gateway`, `503 Service Unavailable`, `504 Gateway Timeout`.

---

## 4. State Management: Cookies, Sessions, and JWT

Since HTTP is stateless, applications track user state using three primary mechanisms:

### A. Cookies
* **Mechanism:** Small text key-value pairs stored in the client's browser.
* **Usage:** Sent automatically in the `Cookie` header of every outbound request to the same domain.
* **Security:** Vulnerable to Cross-Site Scripting (XSS) if not marked `HttpOnly`, and Cross-Site Request Forgery (CSRF) if not configured with `SameSite`.

### B. Sessions
* **Mechanism:** Stateful server-side storage.
* **Usage:** The server stores session data in memory or a database (e.g., Redis) and writes a unique `Session ID` (typically `JSESSIONID` or `PHPSESSID`) to a client cookie.
* **Trade-off:** High server-side memory overhead, requiring distributed cache synchronization across multiple servers.

### C. JWT (JSON Web Tokens)
* **Mechanism:** Stateless client-side storage.
* **Usage:** The server authenticates a user and generates a cryptographically signed token containing user claims (e.g., `{ "userId": 101, "role": "admin" }`). The client stores the token in `localStorage` or a cookie and transmits it via the `Authorization: Bearer <token>` header.
* **Verification:** The server validates the signature using a shared secret key, eliminating the need to query a database for authentication.

---

## 5. Domain Name System (DNS)

DNS translates human-readable hostnames (e.g., `google.com`) into machine-readable IP addresses.

```
Client ──► [Recursive Resolver] ──► [Root Server (.)]
                                ◄── Returns TLD Server (.com)
                 │
                 ▼
          [TLD Server (.com)]   ──► Returns Authoritative Server
                 │
                 ▼
    [Authoritative (google.com)] ──► Returns IP (1.2.3.4)
```

1. **Root Nameservers (`.`):** Direct resolvers to the correct Top-Level Domain (TLD) server.
2. **TLD Nameservers (e.g., `.com`, `.org`):** Manage domain extensions and direct resolvers to the authoritative nameserver.
3. **Authoritative Nameservers:** Maintain the master DNS zone records (A, AAAA, CNAME, MX) for specific domain names and return the final IP address.
