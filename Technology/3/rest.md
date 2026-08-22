# RESTful API Design & Express.js: Architecture & Interview Guide

> **Core Concept:** REST (Representational State Transfer) is an architectural style for network applications based on stateless, client-server communication using standard HTTP verbs (`GET`, `POST`, `PUT`, `PATCH`, `DELETE`) to manipulate resource representations (JSON/XML). Express.js is a minimalist, unopinionated Node.js web framework built around a composable **Middleware Pipeline** (`(req, res, next) => void`).

---

## 1. What It Is in Plain English

REST treats everything on the internet as a **Resource** identified by a clean URL path (e.g. `/api/v1/workspaces/42/containers`). Instead of inventing custom action names in URLs (like `/deleteWorkspaceById?id=42`), you use standard HTTP verbs on nouns (`DELETE /api/v1/workspaces/42`).

Express.js is the glue on the server that receives the raw HTTP request stream, passes it through an ordered chain of **Middleware functions** (for authentication, logging, body parsing, and validation), routes it to the correct controller, and sends back a formatted JSON response with standard HTTP status codes.

---

## 2. The Express.js Middleware Pipeline Flow

```
[ Incoming HTTP Request: POST /api/pipelines/run ]
                         |
                         v
+-----------------------------------------------------------------------------------+
|                            EXPRESS MIDDLEWARE PIPELINE                            |
|                                                                                   |
|  [ 1. Logging Middleware ]       (Logs: Method, IP, Timestamp) -> next()          |
|  [ 2. CORS Middleware ]          (Sets Access-Control-Allow-Origin) -> next()     |
|  [ 3. express.json() ]           (Parses raw buffer to req.body) -> next()        |
|  [ 4. Auth / JWT Guard ]         (Verifies token, attaches req.user) -> next()    |
|  [ 5. Zod Validation Guard ]     (Validates payload schema) -> next()             |
|                                                                                   |
|  [ 6. Route Controller Handler ] (Executes business logic & writes DB/Queue)     |
|                                  -> Calls res.status(201).json({ ... })           |
|                                                                                   |
|  [ 7. Global Error Handler ]     (app.use((err, req, res, next) => ...))          |
|                                  -> Catches unhandled exceptions & returns 500    |
+-----------------------------------------------------------------------------------+
```

---

## 3. How I Used It (NexusIDE & MagnusCI)

- **NexusIDE:**
  - Built REST endpoints for user authentication, workspace session initialization (`POST /api/workspaces`), and container status polling.
  - Implemented custom rate-limiting and authorization middleware verifying JWT tokens and project ownership before allocating Docker container resources.
- **MagnusCI:**
  - Designed the webhook ingestion endpoint (`POST /api/webhooks/github`) using `express.raw({ type: 'application/json' })` to preserve exact raw byte payloads for cryptographic **HMAC-SHA256 signature verification**.
  - Structured modular Express routers (`/api/v1/pipelines`, `/api/v1/runs`, `/api/v1/stages`) with centralized global error-handling middleware.

---

## 4. Analogy for Live Interviews

> *"A REST API is like the menu in a classic restaurant: standard nouns (Burgers, Drinks) with standard actions (Order, Change, Cancel). The Express Middleware Pipeline is like the airport security checkpoint. To get to your flight (the Controller Handler), your request passes through ID Verification (Auth middleware), the Luggage Scanner (Body Parser & Validation), and Customs (CORS). If any checkpoint fails, you are turned away immediately with an error (401/403/400) without ever reaching the gate."*

---

## 5. HTTP Methods: Semantic Idempotency Matrix

| HTTP Verb | CRUD Action | Idempotent? | Safe (Read-Only)? | Example Resource Path |
| :--- | :--- | :---: | :---: | :--- |
| **`GET`** | Read | **YES** | **YES** | `GET /api/pipelines/10` |
| **`POST`** | Create / Action | **NO** | **NO** | `POST /api/pipelines` (Creates new ID) |
| **`PUT`** | Full Replace | **YES** | **NO** | `PUT /api/pipelines/10` (Replaces whole entity) |
| **`PATCH`** | Partial Update | **NO / YES** | **NO** | `PATCH /api/pipelines/10` (Updates single field) |
| **`DELETE`** | Delete | **YES** | **NO** | `DELETE /api/pipelines/10` |

> **Idempotency Defined:** An operation is idempotent if executing it once has the **exact same server state effect** as executing it 100 times consecutively (e.g. `DELETE /users/5` deletes user 5 on first call; subsequent calls do nothing further to the database).

---

## 6. HTTP Status Code Hierarchy

- **`2xx` Success:** `200 OK`, `201 Created` (POST), `202 Accepted` (Enqueued in BullMQ/async), `204 No Content` (DELETE).
- **`3xx` Redirection:** `301 Moved Permanently`, `304 Not Modified` (ETag caching).
- **`4xx` Client Errors:** `400 Bad Request` (Validation error), `401 Unauthorized` (Unauthenticated / missing token), `403 Forbidden` (Authenticated, but lacks role/permission), `404 Not Found`, `409 Conflict` (Duplicate unique key), `429 Too Many Requests` (Rate limited).
- **`5xx` Server Errors:** `500 Internal Server Error`, `502 Bad Gateway`, `503 Service Unavailable`, `504 Gateway Timeout`.

---

## 7. 5–8 High-Yield Interview Questions & Direct Answers

### Q1: What is the difference between `401 Unauthorized` and `403 Forbidden`?
> **Answer:**
> - `401 Unauthorized`: Actually means **Unauthenticated**. The server doesn't know who you are (missing, invalid, or expired JWT/session cookie).
> - `403 Forbidden`: Means **Unauthorized**. The server knows exactly who you are, but you **do not have permission** to access the requested resource (e.g. a regular Developer trying to delete an Organization Owner account).

### Q2: What is the difference between `PUT` and `PATCH`?
> **Answer:**
> - `PUT`: Replaces the **entire resource**. If an entity has fields `{ id, name, email, age }`, and you send `PUT /users/1` with `{ name: "Bob" }`, the missing fields (`email`, `age`) are overwritten to `null` or defaults.
> - `PATCH`: Performs a **partial update**. Sending `PATCH /users/1` with `{ name: "Bob" }` updates only the `name` field while leaving `email` and `age` intact.

### Q3: What is CORS (Cross-Origin Resource Sharing) and how do Preflight `OPTIONS` requests work?
> **Answer:** CORS is a browser security mechanism that blocks web pages hosted on `domain-a.com` from making AJAX/fetch requests to `api-domain-b.com` unless the server explicitly grants permission via HTTP response headers (`Access-Control-Allow-Origin: *`).
> - For non-simple requests (requests with `Content-Type: application/json`, `PUT/DELETE`, or custom `Authorization` headers), the browser automatically sends a **Preflight `OPTIONS` request** first to ask the server if the cross-origin request is allowed.

### Q4: How does Error-Handling middleware work in Express?
> **Answer:** Express recognizes error-handling middleware by its **4-argument signature**: `(err, req, res, next) => void`. It must be placed at the very end of the middleware pipeline after all routes. When any preceding route handler or middleware calls `next(err)` or throws an unhandled error inside async code, Express skips all remaining standard middleware and jumps directly to this error handler.

---

## 8. Common "Gotcha" Questions Interviewers Ask

### Gotcha 1: "Why does an unhandled Promise rejection crash Express in older Node versions?"
- **The Trap:** Writing `app.get('/', async (req, res) => { throw new Error(); })` without a `try/catch`.
- **The Reality:** In Express 4, route handlers do not automatically catch rejected Promises from async functions. If an error is thrown inside an `async` route handler without `try/catch` calling `next(err)`, the request hangs forever (or crashes the process with `UnhandledPromiseRejection`). In Express 5, async errors are natively forwarded to `next()`.

### Gotcha 2: "Why do you need `express.raw()` when verifying GitHub/Stripe Webhook signatures?"
- **The Trap:** Using standard `express.json()` and then running `JSON.stringify(req.body)` to compute the HMAC hash.
- **The Reality:** `JSON.stringify()` does not guarantee exact key order or whitespace preservation. If key order or spacing differs by a single byte from GitHub's original raw payload, the computed HMAC-SHA256 signature **fails verification**. You must verify against the original raw buffer.
