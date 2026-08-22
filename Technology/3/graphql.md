# GraphQL vs. REST: Architecture & Interview Guide

> **Core Concept:** GraphQL is a declarative, strongly-typed query language and runtime for APIs created by Meta (Facebook). Instead of accessing fixed endpoints returning fixed data structures (as in REST), the client specifies **the exact fields and nested relationships it needs in a single request**, completely eliminating over-fetching and under-fetching.

---

## 1. What It Is in Plain English

In a traditional REST API, if a mobile app needs a User's name, their last 3 Project titles, and the current build status of each project, it often has to make **3 separate HTTP round-trips**:
1. `GET /api/users/1` (returns 40 fields, but we only need `name` $\to$ **Over-fetching**)
2. `GET /api/users/1/projects` (returns 10 projects)
3. `GET /api/projects/42/build-status` $\to$ **Under-fetching / Waterfall requests**

In GraphQL, there is only a **single endpoint** (`POST /graphql`). The client sends a declarative query describing the exact data tree it needs:
```graphql
query GetUserDashboard {
  user(id: "1") {
    name
    projects(limit: 3) {
      title
      latestBuild {
        status
      }
    }
  }
}
```
The GraphQL server executes the corresponding **Resolvers**, packages the exact requested JSON structure, and returns it in a single HTTP response.

---

## 2. Architectural Comparison: REST vs. GraphQL

```
REST API (Fixed Endpoints & Structures)
Client ---> GET /users/1 -----------> [ Server: returns 5KB User JSON ]
Client ---> GET /users/1/pipelines -> [ Server: returns 15KB Pipeline JSON ]
Client ---> GET /pipelines/99/stages -> [ Server: returns 20KB Stage JSON ]
(3 Network roundtrips, ~40KB payload with 80% unused fields)

GRAPHQL (Single Endpoint, Declarative Query)
Client ---> POST /graphql (Query: { user { name, pipelines { id, status } } })
Server ---> [ GraphQL Engine executes Resolvers ] ---> Returns ONLY requested JSON (2KB)
(1 Network roundtrip, 100% exact payload efficiency)
```

---

## 3. How I Used It (Full-Stack Backend Context)

- **Backend & Schema Architecture:**
  - Designed strongly-typed GraphQL schemas using SDL (`type Pipeline`, `type BuildStage`, `enum StageStatus`) mapping complex hierarchical relationships (Pipelines $\to$ Stages $\to$ StepLogs).
  - Implemented custom query and mutation resolvers with authentication context guards.
  - Used **`DataLoader`** to batch and cache database queries, resolving the classic GraphQL **N+1 Resolver Problem** when fetching nested relational lists.

---

## 4. Analogy for Live Interviews

> *"A REST API is like ordering a pre-packaged combo meal at a fast-food counter: Combo #1 comes with a burger, fries, and a soda. Even if you only want the burger, you must buy and carry the entire tray. GraphQL is like a high-end buffet with a personal chef: you hand the chef a custom checklist containing exactly 2 slices of tomato, 1 grilled chicken breast, and zero carbs, and the chef prepares and serves precisely that custom plate on a single dish."*

---

## 5. GraphQL vs. REST Comparison Matrix

| Dimension | GraphQL | RESTful APIs |
| :--- | :--- | :--- |
| **Endpoint Architecture** | Single endpoint (`/graphql` via `POST`) | Multiple semantic endpoints (`/users`, `/orders/1`) |
| **Data Fetching** | Client specifies exact shape; zero over/under-fetching | Fixed server-defined responses per endpoint |
| **Type System** | Strongly-typed via Schema Definition Language (SDL) | Informal / requires external OpenAPI/Swagger specs |
| **HTTP Caching** | Complex (all requests are POST; requires client cache) | **Trivial & Native (HTTP GET + ETags / CDNs)** |
| **File Uploads & Binary** | Complex (requires multipart spec / base64) | **Simple & Native (multipart/form-data)** |
| **Learning Curve** | High (Resolvers, Schema, DataLoader, AST parsing) | Low (Standard HTTP primitives) |

---

## 6. 5–8 High-Yield Interview Questions & Direct Answers

### Q1: What is the N+1 Resolver Problem in GraphQL and how do you solve it?
> **Answer:** If a client queries 100 `Users` and requests their `Company` for each user, the `User` resolver runs 1 SQL query (`SELECT * FROM users`), and then the nested `Company` resolver runs 100 subsequent SQL queries inside a loop (`SELECT * FROM companies WHERE id = $1`)—totaling **101 database queries (N+1)**.
> - *Solution:* Use Facebook's **`DataLoader`** utility. DataLoader intercepts and coalesces individual resolver calls within a single tick of the Node.js event loop into a single batched query: `SELECT * FROM companies WHERE id IN (1, 2, 3, ... 100)` and caches results in memory.

### Q2: How does HTTP Caching differ between REST and GraphQL?
> **Answer:**
> - **REST:** Uses native HTTP semantics. `GET /api/products/42` can be cached directly by browser caches, edge CDNs (Cloudflare), and proxies via standard `Cache-Control` and `ETag` headers.
> - **GraphQL:** Almost all requests are sent as `POST` requests to the same URL (`/graphql`), which cannot be cached by standard HTTP edge proxies. Caching must be handled on the client via normalized graph caches (Apollo Client / Urql) or server-side persisted queries.

### Q3: What are GraphQL Subscriptions and how do they work?
> **Answer:** Subscriptions are a GraphQL feature for real-time bidirectional event streaming. While Queries and Mutations operate over standard HTTP POST, Subscriptions establish a persistent **WebSocket connection** (`graphql-ws` protocol). When a backend mutation occurs, the server pushes updated query payloads down to subscribed clients over the open socket.

### Q4: How do you prevent Denial of Service (DoS) attacks on GraphQL APIs?
> **Answer:** Because clients define their own queries, a malicious user can send a deeply nested recursive query (e.g. `user { friends { friends { friends { ... } } } }`) that overwhelms server CPU and crashes the database.
> - *Defenses:*
>   1. **Query Depth Limiting:** Rejects queries exceeding a maximum nesting depth (e.g. max depth = 5).
>   2. **Query Cost / Complexity Analysis:** Assigns cost points to fields and rejects queries exceeding a maximum budget.
>   3. **Persisted Queries (White-listing):** Production servers reject arbitrary dynamic query strings and only execute pre-registered query hashes generated at build time.

---

## 7. Common "Gotcha" Questions Interviewers Ask

### Gotcha 1: "Why is error handling in GraphQL unique compared to REST?"
- **The Answer:** In REST, an error returns standard HTTP error status codes (`400`, `401`, `404`, `500`). In GraphQL, the HTTP response status is almost always **`200 OK`**, even if an error occurred! The response payload contains a top-level `errors` array alongside partial `data`:
  ```json
  {
    "data": { "user": null },
    "errors": [{ "message": "User not found", "path": ["user"] }]
  }
  ```
  Client libraries must inspect the `errors` field rather than relying on HTTP network status codes.
