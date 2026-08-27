# Types of Authentication & Authorization — Interview Notes

A guide to speak about identity, access control, token mechanics, and security
protocols with the confidence of an engineer who has implemented, hardened, and
scaled authentication systems in production, not just recited spec definitions.

---

## What They Are & Why They Are Needed

### 1. Authentication (AuthN) vs. Authorization (AuthZ)
- **Authentication (AuthN) = *"Who are you?"***
  The process of verifying the identity of a user, device, or service (e.g., username/password, OTP, biometric, SSO login).
- **Authorization (AuthZ) = *"What are you allowed to do?"***
  The process of determining the permissions, access rights, and boundaries granted to an authenticated identity (e.g., can User A edit Document 42? Can Service B write to the billing database?).

```
+-----------------------------------------------------------------------------------+
| STEP 1: AUTHENTICATION (AuthN)            | STEP 2: AUTHORIZATION (AuthZ)         |
+-----------------------------------------------------------------------------------+
| "Here are my credentials (login/password)" | "I want to DELETE /api/v1/orders/99"  |
| ───► Verifies Identity (Valid User)        | ───► Checks Permissions (Is Admin?)  |
| ───► Issues Session / Token                | ───► Allows (200) or Denies (403)    |
+-----------------------------------------------------------------------------------+
```

### 2. Why Are They Needed?
1. **Perimeter & Resource Isolation:** Prevents unauthorized users from accessing or modifying sensitive tenant data.
2. **Horizontal & Vertical Privilege Escalation Defense:** Ensures regular users cannot execute administrative operations (vertical) or access other users' private accounts (horizontal / IDOR).
3. **Decoupled Stateless Scaling:** Enables hundreds of independent microservices to verify incoming requests without hammering a central database on every HTTP call.
4. **Audit Logging & Compliance:** Satisfies strict regulatory requirements (SOC2, GDPR, HIPAA, PCI-DSS) by proving who performed what action at what exact timestamp.

---

## 1. Server-Side Stateful Sessions (Cookie + Redis)

**What it is:** The classic web authentication model where the server validates
credentials, generates a cryptographically random session ID stored in a server-side
database (Redis/Postgres), and sends it to the browser as an `HttpOnly` cookie.

**Strengths**
- Instant revocation: logging out or banning a user deletes the session from Redis,
  invalidating access immediately across all devices.
- Small payload footprint: the browser transmits only a tiny 32-byte opaque session ID
  string in the `Cookie` header.
- Zero client-side token management logic required (browsers attach cookies automatically).

**Weaknesses**
- Stateful server storage: requires a centralized, low-latency in-memory store (Redis)
  that must be queried on every incoming request.
- Vulnerable to **Cross-Site Request Forgery (CSRF)** unless protected by `SameSite`
  cookies and anti-CSRF tokens.
- Cross-domain friction: sharing cookies across distinct top-level domains (`app.com`
  vs `api.io`) requires complex CORS and domain cookie configuration.

**When to use:** Monolithic web applications, traditional server-rendered websites
(Next.js SSR, Rails, Django), and applications requiring instantaneous session termination.

**Interview-ready line:** "Stateful sessions trade server memory for absolute control —
you get instantaneous revocation at the cost of querying a shared Redis session store on every request."

---

## 2. Stateless JSON Web Tokens (JWTs - HS256 vs RS256)

**What it is:** A self-contained, digitally signed JSON string formatted as three
Base64URL-encoded segments: `Header.Payload.Signature`.

**Strengths**
- Completely stateless: the payload contains identity claims (`sub`, `roles`, `exp`),
  allowing any microservice to verify authentication without database lookups.
- Cross-domain and mobile friendly: transmitted via the `Authorization: Bearer <token>`
  header, bypassing browser cookie restrictions.
- Decoupled asymmetric verification (**RS256**): signed with a private key by the Auth
  Service and verified across all microservices using public **JWKS keys**.

**Weaknesses**
- Revocation problem: once issued, a stateless JWT cannot be revoked before its
  `exp` timestamp without building a stateful revocation blacklist in Redis.
- Payload bloat: transmitting large claim payloads on every HTTP request increases
  bandwidth overhead.
- Storage vulnerability: storing JWTs in browser `localStorage` exposes them to
  theft via **Cross-Site Scripting (XSS)**.

**When to use:** Microservice architectures, mobile applications, and high-throughput
distributed APIs where hitting a database on every request creates an I/O bottleneck.

**Interview-ready line:** "JWTs trade instant revocation for stateless scalability —
I keep access token lifetimes short (5–15 min) and store refresh tokens in HttpOnly
cookies with automated rotation."

---

## 3. OAuth 2.0 (Delegated Authorization Framework)

**What it is:** An open industry-standard authorization framework that allows a
third-party application to obtain limited access to a user's resources on an HTTP
service without exposing user passwords.

**Strengths**
- Eliminates credential sharing: users authenticate with the Identity Provider (Google, GitHub)
  and grant scoped access (`read:profile`, `write:orders`) to client applications.
- **Authorization Code Flow with PKCE (Proof Key for Code Exchange)**: secures Single
  Page Apps (SPAs) and mobile apps against authorization code interception attacks.
- **Client Credentials Flow**: provides secure machine-to-machine (M2M) server authentication.

**Weaknesses**
- Protocol complexity: involves multiple actors (Resource Owner, Client, Authorization Server,
  Resource Server) and multi-step redirect handshakes.
- Not an authentication protocol by itself: OAuth 2.0 grants *authorization* tokens,
  not user identity proofs (which led to the creation of OIDC).

**When to use:** "Login with Google/GitHub" integrations, third-party developer API access,
and delegating permissions across microservice boundaries.

**Interview-ready line:** "OAuth 2.0 is a delegated authorization framework, not an authentication
protocol — it gives applications a scoped valet key to user data without handing over the master password."

---

## 4. OpenID Connect (OIDC - Identity Layer on OAuth 2.0)

**What it is:** An identity authentication layer built directly on top of OAuth 2.0
that introduces a standardized **ID Token** (JWT) and a `/userinfo` endpoint.

**Strengths**
- Standardized Identity: provides a signed ID Token containing verifiable user profile
  information (`sub`, `name`, `email`, `email_verified`).
- Universal Single Sign-On (SSO): powers modern identity providers (Auth0, Okta,
  Firebase Auth, Keycloak, Google Identity).
- Discovery endpoint (`.well-known/openid-configuration`): enables clients to automatically
  discover public keys, endpoints, and supported scopes dynamically.

**Weaknesses**
- Overhead: requires running or paying for a compliant OpenID Provider.
- Redundant tokens: clients receive both an **ID Token** (for client UI identity) and
  an **Access Token** (for backend API authorization), which confuses junior developers.

**When to use:** User authentication across web/mobile applications, Single Sign-On (SSO),
and federated corporate login portals.

**Interview-ready line:** "OIDC is the identity layer that OAuth 2.0 was always missing —
it standardizes authentication by giving you a signed ID Token alongside your API access token."

---

## 5. API Keys & Mutual TLS (mTLS - Service-to-Service Auth)

**What it is:** Two primary machine-to-machine (M2M) authentication patterns:
opaque static secret keys passed in headers vs. cryptographic bidirectional TLS certificates.

**Strengths**
- **API Keys:** Extremely simple to implement, fast to validate, and easy for third-party
  developers to use in scripts and CLI tools.
- **mTLS (Mutual TLS):** Cryptographically authenticates *both* the client and server
  at the network transport layer (Layer 4/7) using X.509 certificates, rendering man-in-the-middle
  attacks impossible.

**Weaknesses**
- API Keys: static, long-lived secrets that are frequently leaked in GitHub commits;
  lack fine-grained identity claims unless mapped in a database.
- mTLS: high operational overhead for certificate rotation, PKI infrastructure, and
  service mesh configuration.

**When to use:** API keys for external developer access (Stripe, OpenAI API keys);
mTLS for internal zero-trust service-to-service communication inside Kubernetes clusters.

**Interview-ready line:** "API keys are great for developer simplicity at the public edge,
but inside our zero-trust service mesh, we enforce mTLS so services cryptographically verify
each other at the transport layer."

---

## 6. RBAC (Role-Based) vs. ABAC (Attribute-Based Access Control)

**What it is:** The two primary paradigms for structuring authorization rules and
permissions inside applications.

**Strengths**
- **RBAC (Role-Based):** Simple mental model — users are assigned roles (`Admin`, `Editor`, `Viewer`),
  and roles map to static permissions (`posts:create`, `posts:delete`).
- **ABAC (Attribute-Based):** Fine-grained dynamic evaluation based on user attributes,
  resource attributes, action, and environment context (e.g., *"Allow doctor to view patient record ONLY IF doctor is assigned to patient AND time is during hospital shift"*).

**Weaknesses**
- RBAC: "Role Explosion" — as business requirements become granular, roles proliferate
  into dozens of overlapping variations (`RegionalBillingAdminViewer`).
- ABAC: High computational complexity — policy evaluation engines (like Open Policy Agent / OPA)
  must evaluate complex rules at runtime on every request.

**When to use:** RBAC for standard SaaS applications and admin dashboards; ABAC for healthcare,
defense, multi-tenant enterprise platforms, and fine-grained data ownership.

**Interview-ready line:** "I start with RBAC for simplicity, but when permissions depend on
dynamic context — like resource ownership or tenant boundaries — I transition to ABAC or policy engines like OPA."

---

## Side-by-Side Comparison

| Criteria | Stateful Sessions | Stateless JWTs | OAuth 2.0 + PKCE | OpenID Connect (OIDC) | API Keys | mTLS |
|---|---|---|---|---|---|---|
| Primary Focus | User Authentication | API Authorization | Delegated Access | User Identity / SSO | Machine-to-Machine | Zero-Trust M2M |
| State Location | Server RAM / Redis | Client (Self-contained)| Auth Server | Auth Server | DB / Cache Hash | PKI Certificates |
| Revocation | Instant ($O(1)$) | Hard (Needs blacklist) | Token Revocation Endpoint| Token Revocation | Instant (DB disable) | Certificate Revocation |
| Transport | `Cookie` (HttpOnly) | `Authorization: Bearer`| `Authorization: Bearer` | `Authorization: Bearer` | `X-API-Key` Header | TLS Handshake (L4/7) |
| Caching / DB Load | High (DB per request) | Zero (Stateless crypto) | Low (JWKS cached) | Low (JWKS cached) | Low (Cached in Redis)| Zero (Crypto handshake) |
| Primary Threat | CSRF (needs SameSite) | XSS (localStorage leak) | Interception without PKCE| Scope / token misuse | Key leakage in git | Expired cert outage |

---

## Decision Framework (say this out loud in interviews)

1. **Monolithic or server-rendered web app needing instant logout** → Stateful Sessions in Redis with `HttpOnly; SameSite=Strict` cookies.
2. **Distributed microservices needing high-throughput stateless verification** → Asymmetric RS256 JWTs verified via public JWKS.
3. **Allowing third parties to access user data securely without passwords** → OAuth 2.0 (Auth Code + PKCE).
4. **Implementing corporate Single Sign-On (SSO) or federated login** → OpenID Connect (OIDC) / SAML 2.0.
5. **Public developer programmatic access (like Stripe / OpenAI)** → Hashed API Keys (`sk_live_...`).
6. **Internal zero-trust service-to-service communication** → Mutual TLS (mTLS) via Service Mesh (Istio / Linkerd).
7. **Simple role assignment** → RBAC; **Complex context-aware rules (time, ownership, tenancy)** → ABAC / OPA.

---

## What separates a senior answer from a junior one

### 1. Token Storage Security (XSS vs. CSRF)
- **Junior:** "I save the JWT in `localStorage` because it's easy to read in React."
- **Senior:** *"Storing tokens in `localStorage` leaves them completely vulnerable to **Cross-Site Scripting (XSS)** — any compromised npm package or injected `<script>` can steal the token. A senior engineer stores sensitive Refresh Tokens in **`HttpOnly; Secure; SameSite=Strict` cookies**, which are completely inaccessible to JavaScript, and keeps short-lived Access Tokens in application memory (React state/closure)."*

### 2. The Stateless JWT Revocation Dilemma
- **Junior:** "JWTs are amazing because they are 100% stateless and never touch the database."
- **Senior:** *"Statelessness is a double-edged sword: if a user changes their password, gets fired, or gets compromised, **you cannot revoke a stateless JWT before its expiration time**. A senior solves this by:
  1. Keeping Access Token lifetimes ultra-short (5 to 15 minutes).
  2. Enforcing **Refresh Token Rotation**, where using a refresh token issues a new pair and revokes the old one.
  3. Maintaining a fast Redis-based **Token Blacklist** (indexed by JWT `jti` ID) checked only during critical or privileged operations."*

### 3. PKCE Physics & Public Client Security
- **Junior:** "SPAs use the Authorization Code Flow with a Client Secret."
- **Senior:** *"Single Page Apps and mobile apps are **Public Clients** — any Client Secret embedded in client JavaScript or decompiled APKs can be extracted by an attacker. A senior mandates **Authorization Code Flow with PKCE (RFC 7636)**: the client generates a dynamic cryptographically random `code_verifier` and sends its SHA-256 hash (`code_challenge`) during authorization. The Authorization Server verifies that the client presenting the verifier is the exact client that initiated the flow, neutralizing authorization code interception attacks without exposing static secrets."*

### 4. Symmetric (HS256) vs. Asymmetric (RS256) Architecture
- **Junior:** "We sign our JWTs with a shared secret string across all backend services."
- **Senior:** *"Using symmetric HS256 means every microservice that verifies tokens must possess the secret key. If a single reporting microservice is compromised, the attacker can **forge valid admin tokens for the entire ecosystem**. A senior uses **asymmetric RS256 (or Ed25519)**: the Auth Service signs tokens using a private key, and all downstream services verify tokens statelessly using public **JWKS (JSON Web Key Set)** endpoints without ever seeing the signing private key."*

### 5. Authorization Granularity: Beyond Simple Roles
- **Junior:** "I check `if (user.role === 'admin')` on my endpoints."
- **Senior:** *"Hardcoding role checks causes **Role Explosion** and security vulnerabilities as business requirements evolve. A senior decouples authorization from identity:
  - Backends enforce **Permission-based checks** (`can('orders:delete')`) rather than role checks.
  - For resource-level ownership (e.g. User A can only edit Order 99 if `order.tenant_id === user.tenant_id`), we use **Attribute-Based Access Control (ABAC)** or policy engines like **Open Policy Agent (OPA)** evaluated at the gateway or service layer."*
