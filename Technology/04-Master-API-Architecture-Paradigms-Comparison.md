# Types of APIs — Interview Notes

A guide to speak about API styles with the confidence of someone who has actually
shipped and debugged them, not just memorized definitions.

---

## 1. REST (Representational State Transfer)

**What it is:** An architectural style (not a protocol) built on HTTP verbs
(GET, POST, PUT, PATCH, DELETE), stateless requests, and resource-oriented URLs.

**Strengths**
- Simple mental model: everything is a "resource" at a URL.
- Cacheable by default (GET requests, HTTP caching semantics).
- Huge tooling and ecosystem support (every language, every gateway, every browser).
- Stateless — easy to horizontally scale, no server-side session affinity needed.

**Weaknesses**
- Over-fetching / under-fetching: a mobile client often gets more or less data
  than it needs, forcing multiple round trips.
- No strict contract by default — Swagger/OpenAPI is bolted on, not native.
- Versioning is awkward (`/v1/`, `/v2/` sprawl, or header-based versioning that's
  easy to get inconsistent).
- Real-time updates are not native; you need polling, long-polling, or a
  separate transport (WebSocket/SSE) alongside it.

**When to use:** Public-facing APIs, CRUD-heavy systems, anything where cacheability
and broad client compatibility (browsers, mobile, third parties) matter more than
squeezing out the last bit of network efficiency.

**Interview-ready line:** "REST is my default because it optimizes for
interoperability and cache-ability, not raw efficiency — that trade-off is right
for most public and CRUD-style APIs."

---

## 2. GraphQL

**What it is:** A query language and runtime where the client specifies exactly
the shape of data it needs in a single request, resolved against a typed schema.

**Strengths**
- Solves over-fetching/under-fetching — client asks for exactly what it needs.
- One endpoint, strongly typed schema, self-documenting via introspection.
- Great for aggregating multiple backend services into one client-facing graph
  (BFF pattern).

**Weaknesses**
- Caching is hard — you lose free HTTP-level caching since everything is a POST
  to one endpoint; need tools like Apollo cache or persisted queries.
- N+1 query problem on the server if resolvers aren't batched (DataLoader pattern
  needed).
- Query complexity can be abused (deeply nested queries) — needs depth/cost
  limiting or it becomes a DoS vector.
- Steeper learning curve and more backend engineering investment than REST.

**When to use:** Complex, nested data with many client types (web, iOS, Android)
that each need different slices of the same data — e.g., a social feed or a
dashboard aggregating several microservices.

**Interview-ready line:** "GraphQL earns its complexity when you have multiple
clients with divergent data needs hitting a complex object graph — otherwise
it's often over-engineering for a simple CRUD service."

---

## 3. gRPC

**What it is:** A high-performance RPC framework from Google using HTTP/2 and
Protocol Buffers (binary serialization) with strict, code-generated contracts.

**Strengths**
- Very fast — binary payload, multiplexed HTTP/2 streams, low latency.
- Native support for streaming (client, server, and bidirectional streams).
- Strong typing via `.proto` files — contract-first, generates client/server
  stubs in many languages.
- Excellent for internal service-to-service (east-west) communication.

**Weaknesses**
- Not browser-friendly out of the box (needs grpc-web + a proxy).
- Binary format is not human-readable — harder to debug with curl/Postman
  without tooling.
- Smaller ecosystem for public-facing use; mostly an internal-network technology.

**When to use:** Internal microservice-to-microservice communication where
performance and strict contracts matter — e.g., a payments service calling an
inventory service, or anywhere polyglot services need a strict interface.

**Interview-ready line:** "I reach for gRPC on the internal service mesh where
I control both ends and care about latency and contract safety, not for
public APIs where accessibility matters more."

---

## 4. SOAP (Simple Object Access Protocol)

**What it is:** An XML-based protocol with a strict envelope structure and
formal contracts (WSDL), historically dominant in enterprise systems.

**Strengths**
- Built-in standards for security (WS-Security), transactions (WS-AtomicTransaction),
  and formal contracts (WSDL) — valuable in regulated industries.
- Protocol-agnostic (can run over HTTP, SMTP, etc.).
- Strong tooling in enterprise/legacy stacks (banking, healthcare, government).

**Weaknesses**
- Verbose XML payloads — heavier than JSON, slower to parse.
- Rigid and complex to implement compared to REST/GraphQL.
- Poor fit for mobile/web-first, lightweight applications.

**When to use:** Legacy enterprise integrations, especially finance, healthcare,
or government systems where formal contracts and built-in security/transaction
standards are non-negotiable requirements, not preferences.

**Interview-ready line:** "SOAP isn't 'outdated' so much as it's optimized for
a different problem — formal, auditable contracts in regulated environments —
and it still shows up there for good reason."

---

## 5. WebSocket

**What it is:** A protocol providing a persistent, full-duplex connection
between client and server over a single TCP connection.

**Strengths**
- True real-time, bidirectional communication with low latency.
- No repeated HTTP handshake overhead once connected.
- Ideal for chat, live dashboards, multiplayer/gaming, collaborative editing.

**Weaknesses**
- Stateful connections complicate horizontal scaling (need sticky sessions or
  a pub/sub layer like Redis to fan out messages across servers).
- No built-in request/response semantics — you design your own message protocol.
- Harder to secure/monitor with standard HTTP tooling (proxies, WAFs).

**When to use:** Anything needing continuous two-way real-time data — chat apps,
live trading tickers, multiplayer games, collaborative tools (e.g., Figma-style
cursors).

**Interview-ready line:** "WebSocket trades statelessness for real-time — I use
it only when the product genuinely needs push updates, since it adds real
scaling complexity."

---

## 6. Server-Sent Events (SSE)

**What it is:** A standard HTTP-based protocol (`text/event-stream`) where the
server maintains a persistent, one-way connection to push live text events to
the client.

**Strengths**
- Native browser support via the `EventSource` API with automatic reconnection
  and `Last-Event-ID` tracking.
- Operates over standard HTTP/1.1 or HTTP/2 — no special protocol upgrade,
  works seamlessly through firewalls, proxies, and load balancers.
- Lightweight compared to WebSocket when communication is strictly one-directional
  (server-to-client).

**Weaknesses**
- Strictly mono-directional: client cannot send data over the same connection
  (must use standard HTTP POST requests).
- Transport is text-only (UTF-8); binary data must be base64-encoded.
- HTTP/1.1 client connection limits (max 6 open streams per domain without HTTP/2).

**When to use:** Live streaming text or notifications where the client only listens
— LLM token streaming (ChatGPT UI), live stock tickers, build logs, status feeds.

**Interview-ready line:** "If data only flows from server to client — like AI token
streaming or live notifications — SSE gives you push capability over standard HTTP
without the operational complexity of managing stateful WebSocket clusters."

---

## 7. Webhooks

**What it is:** Not a full API style but an inversion of control — the server
pushes an HTTP POST to a client-registered URL when an event occurs, instead of
the client polling.

**Strengths**
- Event-driven, efficient — no polling overhead.
- Simple to implement on the receiving end (just an HTTP endpoint).

**Weaknesses**
- Delivery isn't guaranteed by default — needs retries, idempotency keys, and
  signature verification (security) on the receiver's side.
- Debugging is harder (you can't easily "pull" data on demand; you're waiting
  for a push).
- Receiver must be publicly reachable (an issue for local dev — often needs
  tunneling tools like ngrok).

**When to use:** Notifying external systems of events — payment confirmations
(Stripe), CI/CD triggers (GitHub), order status changes — where near-real-time
notification matters but a full duplex connection is overkill.

**Interview-ready line:** "Webhooks are the right tool when polling is wasteful
but you don't need a persistent connection — just make sure to design for
idempotency, since delivery isn't guaranteed exactly-once."

---

## 8. tRPC / Type-Safe RPC

**What it is:** An RPC framework for TypeScript monorepos that provides end-to-end
static type safety between client and server without code generation or schemas.

**Strengths**
- Zero code generation: client automatically inherits backend router types via
  TypeScript type inference (`AppRouter`).
- Blazing fast developer velocity with instant autocomplete and compile-time
  refactoring safety across the full stack.
- Built on top of HTTP/JSON and integrates seamlessly with React Query.

**Weaknesses**
- Strictly locked to TypeScript on both frontend and backend.
- Not suited for public APIs or third-party consumers (no standalone OpenAPI
  contract by default).
- Requires a monorepo or tightly coupled deployment boundary.

**When to use:** Full-stack TypeScript applications (e.g., Next.js / Remix /
React + Node) built by a unified team where developer velocity and type safety
trump language interoperability.

**Interview-ready line:** "tRPC eliminates the boilerplate of REST and GraphQL in
TypeScript monorepos by giving you compile-time type safety for free — but I only
use it internally where both ends are TypeScript."

---

## Side-by-Side Comparison

| Criteria | REST | GraphQL | gRPC | SOAP | WebSocket | Server-Sent Events (SSE) | Webhook | tRPC |
|---|---|---|---|---|---|---|---|---|
| Data format | JSON/XML | JSON | Protobuf (binary) | XML | Any (text/binary) | Text (`text/event-stream`) | JSON/XML | JSON (SuperJSON) |
| Communication | Request/Response | Request/Response | Req/Res, Streaming | Request/Response | Full-duplex, persistent | Server push (one-way) | Server push (one-way) | Request/Response |
| Best for | Public APIs, CRUD | Complex/nested data | Internal microservices | Enterprise/regulated | Real-time bidirectional | One-way live streaming (LLM) | Event notifications | Full-stack TS monorepos |
| Caching | Native (HTTP) | Hard | N/A (not cache-friendly) | Poor | N/A | Native (HTTP) | N/A | Client cache (React Query) |
| Browser support | Native | Native (over HTTP) | Needs grpc-web proxy | Native (heavy) | Native | Native (`EventSource`) | N/A (server-to-server) | Native (TS/JS) |
| Learning curve | Low | Medium-High | Medium | High | Medium | Low | Low | Low (if TS familiar) |
| Typical scale pattern | Stateless, easy | Needs resolver batching | Very efficient, HTTP/2 | Heavy payloads | Needs sticky sessions/pub-sub | Stateless, HTTP/2 multiplexed | Needs retry/idempotency | Stateless, easy |

---

## Decision Framework (say this out loud in interviews)

1. **Public API, many unknown consumers, cacheability matters** → REST.
2. **Multiple client types with different, nested data needs** → GraphQL.
3. **Internal service-to-service, performance-critical, you control both ends** → gRPC.
4. **Regulated industry needing formal contracts and built-in security/transactions** → SOAP.
5. **Continuous two-way real-time updates (chat, multiplayer, gaming)** → WebSocket.
6. **Continuous one-way server streaming (LLM tokens, live notifications)** → Server-Sent Events (SSE).
7. **Notify others of events without them polling you** → Webhook.
8. **Full-stack TypeScript monorepo with fast iteration** → tRPC.

---

## What separates a senior answer from a junior one

### 1. Consumer-Driven API Design vs. Dogmatic Preference
- **Junior:** "REST is outdated; we should build everything in GraphQL or gRPC."
- **Senior:** *"API architecture is dictated by **who consumes the API and the network boundary**:
  - **Public third-party APIs** $\to$ **REST** (universal HTTP compatibility, edge CDN cacheability, zero client SDK friction).
  - **Internal polyglot microservices** $\to$ **gRPC** (Protobuf binary serialization, HTTP/2 multiplexing, sub-5ms latency, strict compile-time contracts).
  - **Multi-client dashboards (Web/iOS/Android)** $\to$ **GraphQL** (eliminates over/under-fetching, client controls payload shape).
  - **Full-stack TypeScript monorepos** $\to$ **tRPC** (instant end-to-end type safety with zero code generation)."*

### 2. Network Physics & Caching Mechanics
- **Junior:** "GraphQL is always more efficient than REST because the payload is smaller."
- **Senior:** *"GraphQL reduces payload bytes over the wire, but it **breaks standard HTTP caching at edge CDNs** because all requests hit a single `/graphql` endpoint via HTTP POST. REST endpoints leverage standard HTTP caching headers (`Cache-Control`, `ETag`, `304 Not Modified`) directly on CDN edge locations (Cloudflare/CloudFront), completely offloading traffic from origin servers."*

### 3. Real-Time Streaming: WebSockets vs. SSE vs. Polling
- **Junior:** "Any real-time feature requires WebSockets."
- **Senior:** *"WebSockets are **stateful and full-duplex**, requiring custom heartbeat ping/pong frames, sticky load balancer sessions, and an external Redis Pub/Sub mesh to scale across clusters. If communication is strictly one-directional from server to client—such as **LLM token streaming (ChatGPT UI)** or live notification feeds—a senior chooses **Server-Sent Events (SSE)**. SSE operates over standard HTTP/2 streams (`text/event-stream`), natively supports auto-reconnections with `Last-Event-ID`, and bypasses WebSocket infrastructure complexity."*

### 4. Async Event Delivery & Webhook Defense
- **Junior:** "Webhooks are simple: we receive an HTTP POST and trigger our internal service."
- **Senior:** *"A senior knows that external webhook delivery is **at-least-once, not exactly-once**. Production webhook consumers must:
  1. Compute **HMAC-SHA256 signatures** on raw untouched byte buffers (`express.raw()`) and verify using **`crypto.timingSafeEqual()`** to prevent timing attacks.
  2. Enforce strict **Idempotency** by storing processed `event_id` keys in Redis with atomic `SET NX` locks to eliminate duplicate charging.
  3. Validate **Timestamp windows** (<5 minutes) to reject replay attacks."*

### 5. Polyglot API Coexistence in Production
- **Junior:** "A company should standardize on one single API protocol."
- **Senior:** *"A modern enterprise architecture combines complementary API paradigms across different network zones: **REST** at the public perimeter, **gRPC** across internal Kubernetes microservices, **SSE** for generative AI streaming, **WebSockets** for live collaborative cursors, and **Webhooks** for asynchronous third-party partner integration."*