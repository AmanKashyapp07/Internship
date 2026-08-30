# Authentication, Authorization & Identity Architecture Reference

> **Scope:** Deep Architectural Analysis of Distributed Identity Systems: Authentication (AuthN) vs. Authorization (AuthZ), Stateful In-Memory Sessions, Stateless JSON Web Tokens (HS256 vs. Asymmetric RS256 with JWKS), OAuth 2.0 PKCE Authorization Frameworks, OpenID Connect (OIDC), Machine-to-Machine Security (API Keys & Mutual TLS mTLS), Access Control Models (RBAC, ABAC, ReBAC), and Enterprise SAML 2.0 Federation.

---

# Table of Contents
1. [Theoretical Foundations: Authentication (AuthN) vs. Authorization (AuthZ)](#1-theoretical-foundations-authentication-authn-vs-authorization-authz)
2. [Stateful Session Management (Cookie + In-Memory State)](#2-stateful-session-management-cookie--in-memory-state)
3. [Stateless JSON Web Tokens (JWT: HS256 vs. RS256 & JWKS)](#3-stateless-json-web-tokens-jwt-hs256-vs-rs256--jwks)
4. [OAuth 2.0 Delegated Authorization Framework & PKCE](#4-oauth-20-delegated-authorization-framework--pkce)
5. [OpenID Connect (OIDC): Identity Layer & Discovery Protocols](#5-openid-connect-oidc-identity-layer--discovery-protocols)
6. [Machine-to-Machine Security: API Keys & Mutual TLS (mTLS)](#6-machine-to-machine-security-api-keys--mutual-tls-mtls)
7. [Access Control Topologies: RBAC vs. ABAC vs. ReBAC](#7-access-control-topologies-rbac-vs-abac-vs-rebac)
8. [Enterprise Federation: SAML 2.0 Architecture](#8-enterprise-federation-saml-20-architecture)
9. [Master Identity & Access Control Comparison Matrix](#9-master-identity--access-control-comparison-matrix)

---

# 1. Theoretical Foundations: Authentication (AuthN) vs. Authorization (AuthZ)

```
Identity & Access Control Pipeline:
[ Client Ingress ] ---> [ Step 1: Authentication (AuthN) ]
                                | (Verifies Identity: "Who are you?")
                                v
                        [ Validated Principal ]
                                |
                                v
                        [ Step 2: Authorization (AuthZ) ]
                                | (Evaluates Policy: "What can you execute?")
                                v
               +----------------+----------------+
               | (Authorized)                    | (Unauthorized)
               v                                 v
       [ Execute Resource API ]           [ 403 Forbidden ]
```

```
+----------------------------------------------------------------------------------------------------+
| CORE DIMENSION       | AUTHENTICATION (AuthN)               | AUTHORIZATION (AuthZ)                |
+----------------------------------------------------------------------------------------------------+
| Semantic Question    | "Who is the requesting principal?"   | "Does this principal have access?"   |
| Verification Target  | Passwords, Passkeys, OTP, Biometrics | RBAC Roles, Scopes, ABAC Attributes  |
| Protocol Standards   | OIDC, SAML 2.0, FIDO2 / WebAuthn     | OAuth 2.0 Scopes, XACML, Zanzibar    |
| HTTP Error Codes     | 401 Unauthorized (Unauthenticated)   | 403 Forbidden (Insufficient Rights)  |
+----------------------------------------------------------------------------------------------------+
```

---

# 2. Stateful Session Management (Cookie + In-Memory State)

Stateful session architectures store active session metadata on server-side databases (Redis) and attach an opaque cryptographic session ID to client requests via HTTP cookies:

```
Stateful Session Architecture:
[ Browser Client ] <=== HTTP Set-Cookie: sid=a8f1... (HttpOnly, Secure, SameSite=Strict) ===> [ Web Server ]
                                                                                                    |
                                                                        [ Centralized Redis Cache ]<+
                                                                        (Key: "sid:a8f1..." -> UserID)
```

```
+----------------------------------------------------------------------------------------------------+
| ARCHITECTURAL CHARACTERISTIC                                                                       |
+----------------------------------------------------------------------------------------------------+
| - State Model: Stateful (Centralized in-memory session database required across all web nodes).     |
| - Inherent Strengths: Instantaneous revocation (deleting Redis key terminates access everywhere).  |
| - Inherent Weaknesses: Centralized session store dependency; cross-domain cookie restrictions.     |
| - Security Protections: `HttpOnly` (blocks XSS access), `Secure` (TLS only), `SameSite=Strict` CSRF|
| - Optimal Application Domain: Server-rendered web applications (SSR), high-security banking portals|
+----------------------------------------------------------------------------------------------------+
```

---

# 3. Stateless JSON Web Tokens (JWT: HS256 vs. RS256 & JWKS)

A JSON Web Token (RFC 7519) is a compact, URL-safe, self-contained claims container digitally signed using symmetric or asymmetric cryptography:

```
JSON Web Token Anatomical Structure:
[ Base64URL(Header) ] . [ Base64URL(Payload) ] . [ Base64URL(Signature) ]
  - Header:    { "alg": "RS256", "typ": "JWT", "kid": "key-2026-v1" }
  - Payload:   { "sub": "usr_1042", "role": "admin", "exp": 1774915200 }
  - Signature: RSASSA-PKCS1-v1_5(Header + "." + Payload, PrivateKey)
```

```
Decoupled Asymmetric Verification (RS256 / EdDSA via JWKS):
[ Auth Microservice ] ---> Signs JWT with Private Key (.pem)
                                  |
                                  v (Transmitted via Authorization: Bearer <JWT>)
[ Edge Microservice A ] <--- Verifies Signature using Public JWKS (/certs/.well-known/jwks.json)
[ Edge Microservice B ] <--- Verifies Signature using Public JWKS (Zero Database I/O Overhead!)
```

```
+----------------------------------------------------------------------------------------------------+
| ALGORITHM CLASS      | SIGNING MECHANICS                    | VERIFICATION TOPOLOGY                |
+----------------------------------------------------------------------------------------------------+
| Symmetric (HS256)    | Single shared secret key for signing | All verifying microservices must     |
|                      | and verification (HMAC-SHA256)       | hold the master signing secret (Risk)|
| Asymmetric (RS256)   | Signed with private key; verified via| Any service verifies tokens using the|
|                      | public key certificate (RSA / ECDSA) | public JSON Web Key Set (JWKS)       |
+----------------------------------------------------------------------------------------------------+
```

---

# 4. OAuth 2.0 Delegated Authorization Framework & PKCE

OAuth 2.0 (RFC 6749) allows third-party clients to obtain scoped access to user-owned resources on a resource server without exposing credentials:

```
Authorization Code Flow with Proof Key for Code Exchange (PKCE):
[ Client App (SPA/Mobile) ]        [ Authorization Server ]           [ Resource API ]
            |                                  |                             |
            |-- 1. /authorize?code_challenge ->|                             |
            |-- 2. Authenticate User --------->|                             |
            |<- 3. Redirect: ?code=auth_code --|                             |
            |                                  |                             |
            |-- 4. /token (code + verifier) -->|                             |
            |<- 5. Return Access Token (JWT) --|                             |
            |                                                                |
            |-- 6. Request Resource with Bearer <AccessToken> -------------->|
```

- **PKCE Cryptographic Invariant:** The client generates a cryptographically random `code_verifier` ($V$) and sends its SHA-256 digest `code_challenge` ($C = \text{BASE64URL}(\text{SHA256}(V))$). Upon token exchange, sending $V$ proves that the token requester is identical to the initial authorization initiator, preventing authorization code interception attacks.

---

# 5. OpenID Connect (OIDC): Identity Layer & Discovery Protocols

OpenID Connect (OIDC) is an identity layer built on top of OAuth 2.0 that introduces a standardized **ID Token** (JWT) and discovery metadata:

```
OIDC Token Roles:
- ID Token (JWT):     Consumed by Client Application to render UI identity (name, email, sub).
- Access Token (JWT): Consumed by Resource Servers to authorize backend API mutations.
```

- **Discovery Endpoint (`/.well-known/openid-configuration`):** Publishes standardized JSON descriptors defining authorization endpoints, token issuance routes, supported cryptographic signing algorithms, and the public `jwks_uri`.

---

# 6. Machine-to-Machine Security: API Keys & Mutual TLS (mTLS)

```
+----------------------------------------------------------------------------------------------------+
| SECURITY PATTERN     | AUTHENTICATION PRIMITIVE             | SYSTEM TRADEOFF                      |
+----------------------------------------------------------------------------------------------------+
| API Keys             | Opaque high-entropy secret string    | Simple implementation; vulnerable to |
|                      | passed in HTTP Authorization header  | credential leaks without rotation    |
| Mutual TLS (mTLS)    | Bidirectional X.509 cryptographic    | Strong zero-trust perimeter security;|
|                      | certificates validated at TLS layer  | high PKI management complexity       |
+----------------------------------------------------------------------------------------------------+
```

```
Mutual TLS (mTLS) Handshake Data Flow:
Client                                           Server
  |                                                |
  |--- 1. ClientHello ---------------------------->|
  |<-- 2. ServerHello + Server X.509 Certificate---| (Client verifies Server Identity)
  |<-- 3. CertificateRequest ----------------------|
  |--- 4. Client X.509 Certificate --------------->| (Server verifies Client Identity)
  |<== 5. Encrypted Bidirectional Session TLS ====>|
```

---

# 7. Access Control Topologies: RBAC vs. ABAC vs. ReBAC

```
+----------------------------------------------------------------------------------------------------+
| MODEL                | ACCESS EVALUATION MECHANICS          | OPTIMAL SCALE & USE CASE             |
+----------------------------------------------------------------------------------------------------+
| Role-Based (RBAC)    | Permissions mapped to static roles:  | Coarse-grained enterprise hierarchies|
|                      | `User -> Roles -> Permissions`       | (e.g. Admin, Editor, Viewer)         |
| Attribute-Based      | Dynamic boolean rule evaluation:     | Fine-grained contextual governance   |
| (ABAC)               | $f(\text{Subject}, \text{Resource},  | (e.g. Allow if Department = Legal    |
|                      | \text{Action}, \text{Environment})$  | AND Time is within Business Hours)   |
| Relationship-Based   | Directed graph traversal over entity | Google Zanzibar, Google Drive / Docs |
| (ReBAC)              | relationships (Parent / EditorOf)    | (Nested team and folder permissions) |
+----------------------------------------------------------------------------------------------------+
```

---

# 8. Enterprise Federation: SAML 2.0 Architecture

Security Assertion Markup Language (SAML 2.0) is an XML-based federated identity standard used for enterprise Single Sign-On (SSO):

```
SAML 2.0 SP-Initiated SSO Flow:
[ Browser ]                  [ Service Provider (SP) ]              [ Identity Provider (IdP) ]
    |                                   |                                        |
    |-- 1. Access Protected Route ----->|                                        |
    |<- 2. SAML Request (Redirect) -----|                                        |
    |------------------------------------------------ 3. Authenticate User ----->|
    |<----------------------------------------------- 4. Signed SAML Assertion --|
    |-- 5. POST SAML Response Assertion>|                                        |
    |<- 6. Establish Session Cookie ----|                                        |
```

---

# 9. Master Identity & Access Control Comparison Matrix

```
+----------------------------------------------------------------------------------------------------+
| PARADIGM      | STATE MODEL   | VERIFICATION MECHANISM | CRYPTOGRAPHIC BASE  | PRIMARY DOMAIN      |
+----------------------------------------------------------------------------------------------------+
| Sessions      | Stateful      | Database Cache Lookup  | Random Hex / UUID   | Monolithic Web Apps |
| JWT (RS256)   | Stateless     | Asymmetric Public JWKS | RSA / ECDSA Sign    | Microservices & APIs|
| OAuth 2.0     | Delegated     | Access Token Scopes    | Bearer Signatures   | Delegated 3rd-Party |
| OIDC          | Federated     | ID Token Validation    | Asymmetric JWTs     | Modern SSO & AuthN  |
| mTLS          | Zero-Trust    | Bidirectional Handshake| X.509 PKI Certs     | Service Mesh (East) |
| SAML 2.0      | Federated     | XML Signature Verify   | XMLDSIG X.509 Certs | Enterprise Okta SSO |
| ReBAC         | Graph Model   | Graph Tuple Traversal  | Relational Tuples   | Complex Object ACLs |
+----------------------------------------------------------------------------------------------------+
```
