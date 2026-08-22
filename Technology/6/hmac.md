# HMAC-SHA256 & Webhook Cryptographic Security: Interview Guide

> **Core Concept:** HMAC (Hash-based Message Authentication Code) is a cryptographic construction that calculates a message authentication code using a secret key combined with a cryptographic hash function (SHA-256). It guarantees both **Data Integrity** (verifying that the payload was not altered by a Man-in-the-Middle) and **Authenticity** (proving the request genuinely originated from the trusted sender, such as GitHub or Stripe).

---

## 💬 "Say It Out Loud" in an Interview (The 30-Second Elevator Pitch)

> **When the interviewer asks:** *"What is HMAC and why/when do we use it?"*
>
> **You say:** *"HMAC-SHA256 is a symmetric cryptographic construction that verifies both data integrity and authenticity using a shared secret key and nested hashing. It ensures webhook payloads have not been altered in transit and uses constant-time string comparisons to prevent microsecond timing attacks."*

---

## 1. What It Is in Plain English

When you expose a public webhook endpoint (`POST https://ci.example.com/api/webhooks/github`), **anyone on the internet can send an HTTP POST request to that URL**. A malicious attacker could forge a fake webhook payload claiming "User pushed code to main branch" to trigger expensive build servers and deploy unauthorized code.

To prevent this:
1. GitHub and your server share a private secret string (**Webhook Secret**).
2. Before sending the request, GitHub calculates:
   $$\text{Signature} = \text{HMAC-SHA256}(\text{Secret Key}, \text{Raw Request Body})$$
   and attaches it to the HTTP header (`X-Hub-Signature-256: sha256=a1b2c3...`).
3. When your server receives the request, it recomputes the HMAC using its local secret key and compares it against GitHub's header. If the signatures match, the request is authentic; if they differ by even a single bit, the server rejects it immediately with **`401 Unauthorized`**.

---

## 2. HMAC Cryptographic Architecture & Flow

```
[ GITHUB / STRIPE SENDER ]                                           [ YOUR BACKEND (CI/CD Pipeline Engines) ]
+-----------------------------------+                               +-----------------------------------+
| Shared Secret: "my_secret_key"    |                               | Shared Secret: "my_secret_key"    |
| Raw Payload: '{"ref":"main"...}'  |                               | Raw Buffer: (express.raw())       |
|                                   |                               |                                   |
| -> Compute HMAC_SHA256(Key, Body) |                               | -> Compute HMAC_SHA256(Key, Body) |
| -> Signature: "sha256=9f8e7d..."  |                               | -> Local Hash: "9f8e7d..."        |
+-----------------------------------+                               +-----------------------------------+
                  |                                                                   |
                  v (HTTPS POST with Header: X-Hub-Signature-256)                     |
                  +-------------------------------------------------------------------+
                                                      |
                                                      v
                                      [ crypto.timingSafeEqual() ]
                                      - Matches? ---> Process CI Build (202 Accepted)
                                      - Fails?   ---> Reject Request (401 Unauthorized)
```

---

## 3. Analogy for Live Interviews

> *"Imagine ordering confidential military supplies by letter. A signature on plain paper can be forged, and anyone could tamper with the letter in transit. An HMAC is like applying a custom wax seal using a unique brass stamp that only you and the General possess. When the General receives the letter, they inspect the unbroken wax seal with a microscope. If the wax is intact and matches the stamp, the General knows with 100% mathematical certainty that you wrote the letter and nobody opened the envelope along the way."*

---

## 4. HMAC vs. Simple Hash Concatenation (`SHA256(Key + Msg)`)

| Dimension | Proper HMAC (`HMAC-SHA256`) | Naive Hash Concatenation (`SHA256(Key + Msg)`) |
| :--- | :--- | :--- |
| **Security Proof** | Formally proven secure against all known attacks | **Broken: Vulnerable to Length Extension Attacks** |
| **Hashing Construction** | Nested hashing: $H(K \oplus \text{opad} \parallel H(K \oplus \text{ipad} \parallel M))$ | Simple one-pass hashing: $H(K \parallel M)$ |
| **Industry Standard** | Universal standard (RFC 2104: GitHub, Stripe, AWS SigV4) | Deprecated / Insecure |

---

## 5. 5–8 High-Yield Interview Questions & Direct Answers

### Q1: What is a Timing Attack in signature verification and how do you prevent it?
> **Answer:** Standard string equality comparisons (`signature === expectedSignature`) compare characters sequentially from left to right and return `false` the **exact microsecond they encounter the first mismatched character**. An attacker measuring microsecond response times across thousands of requests can guess the signature character-by-character.
> - *Prevention:* Always use **Constant-Time Comparison** (`crypto.timingSafeEqual(a, b)` in Node.js), which executes in the exact same number of CPU cycles regardless of where or whether the strings differ.

### Q2: What is a Length Extension Attack and why does HMAC prevent it?
> **Answer:** In Merkle–Damgård hash functions (MD5, SHA-1, SHA-256), the internal state is output directly as the hash digest. If you use naive hashing `Hash(Secret || Message)`, an attacker who knows `Message` and the hash output can append malicious data (`|| EvilData`) and calculate the valid hash for the extended message **without ever knowing the Secret Key**. HMAC prevents this by using a **two-pass nested hashing formula** that masks the internal state with inner and outer padding keys (`ipad` and `opad`).

### Q3: What is a Replay Attack and how do you protect webhook endpoints against it?
> **Answer:** A Replay Attack occurs when an eavesdropper intercepts a valid, signed webhook request and repeatedly re-sends the exact same HTTP packet to your server. Because the HMAC signature is mathematically valid, the server accepts it repeatedly (e.g. charging a card twice).
> - *Defenses:*
>   1. **Timestamp Headers & Clock Tolerance:** Stripe/GitHub send a timestamp header (`X-Stripe-Timestamp`). The server rejects any payload where `|now - timestamp| > 300 seconds` (5-minute drift).
>   2. **Nonce / Delivery ID Deduplication:** Track unique webhook delivery IDs (`X-GitHub-Delivery`) in Redis with a 24-hour TTL and reject duplicates.

---

## 6. Common "Gotcha" Questions Interviewers Ask

### Gotcha 1: "Why does `JSON.stringify(req.body)` fail HMAC verification when using `express.json()`?"
- **The Trap:** Expecting `JSON.stringify()` to reproduce the exact byte sequence of the original HTTP request.
- **The Reality:** JSON parsers do not preserve whitespace, newline characters, or object key insertion ordering. If GitHub sent `{"a": 1, "b": 2}` and your server re-stringifies it as `{"b":2,"a":1}`, the resulting SHA-256 hash is **completely different**, causing signature verification to fail. You must capture and hash the **raw unparsed Buffer**.
