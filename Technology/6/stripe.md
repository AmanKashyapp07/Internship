# Stripe & Payment Infrastructure Architecture: Interview Guide

> **Core Concept:** Stripe is a developer-first global payment infrastructure platform. It provides APIs, client SDKs (**Stripe Elements**), and webhook event streaming that abstract away the extreme complexities of credit card networks, multi-currency processing, banking rails, Strong Customer Authentication (SCA / 3D Secure 2), and strict **PCI-DSS Level 1 compliance** (ensuring sensitive card numbers never touch your application servers).

---

## 💬 "Say It Out Loud" in an Interview (The 30-Second Elevator Pitch)

> **When the interviewer asks:** *"What is STRIPE and why/when do we use it?"*
>
> **You say:** *"Stripe is a developer-first payment infrastructure platform. It uses client-side tokenization via Stripe Elements to keep raw card numbers off application servers for PCI-DSS compliance, models payments through a multi-step Payment Intents state machine, and fulfills purchases via cryptographically signed webhooks with idempotency keys."*

---

## 1. What It Is in Plain English

Handling credit cards directly on your backend server is a legal and security nightmare: if an attacker breaches your server and steals raw card numbers, you face massive regulatory fines and liability (**PCI-DSS compliance violation**).

Stripe eliminates this risk using **Tokenization & Payment Intents**:
1. The user enters their credit card into an encrypted iframe hosted by Stripe on your frontend (**Stripe Elements**).
2. The card data goes directly from the user's browser to Stripe's PCI-certified servers, returning a safe, single-use token (`pm_12345`).
3. Your backend server only ever sees the token. Your backend calls `stripe.paymentIntents.create({ amount: 2000, currency: 'usd', payment_method: 'pm_12345' })` to safely charge the card.

---

## 2. Stripe Payment Intent Lifecycle & Webhook Architecture

```
[ CLIENT BROWSER ]                     [ YOUR BACKEND (Node.js) ]                     [ STRIPE SERVERS ]
        |                                          |                                          |
        | 1. "Checkout" Clicked                    |                                          |
        | ---------------------------------------> |                                          |
        |                                          | 2. Create PaymentIntent                  |
        |                                          |    (Idempotency-Key: UUID)               |
        |                                          | ---------------------------------------> |
        |                                          |                                          |
        |                                          | 3. Returns Client Secret: 'pi_99_sec_...' |
        |                                          | <--------------------------------------- |
        | 4. Returns Client Secret                 |                                          |
        | <--------------------------------------- |                                          |
        |                                                                                     |
        | 5. stripe.confirmCardPayment(clientSecret, cardElement)                             |
        | ----------------------------------------------------------------------------------> |
        |    (Handles 3D Secure / OTP Bank Modal popup if required)                           |
        |                                                                                     |
        |                                          [ 6. ASYNC WEBHOOK EVENT: payment_intent.succeeded ]
        |                                          | <--- (Signed HTTPS POST with HMAC) ------ |
        |                                          |                                          |
        |                                          | 7. Fulfills order & updates Postgres DB  |
```

---

## 3. Analogy for Live Interviews

> *"Imagine you want to buy diamond jewelry at a high-security vault. Instead of carrying $50,000 cash in your backpack through a dangerous alley (storing raw credit card numbers on your server), an armored security truck (Stripe Elements) collects the cash directly from your bank. The truck gives your merchant a sealed, tamper-proof claim receipt (the PaymentIntent ID). The merchant hands the receipt to the vault clerk, and the money is transferred safely behind bulletproof glass."*

---

## 4. 5–8 High-Yield Interview Questions & Direct Answers

### Q1: What is the Payment Intents API and why did Stripe replace the legacy Charges API?
> **Answer:** The legacy Charges API assumed a single, synchronous payment step. However, global regulations (like European PSD2 / SCA - Strong Customer Authentication) require **3D Secure 2 (3DS2)** two-factor authentication (e.g. mobile OTP / bank app authorization).
> - The **Payment Intents API** models payment as a state machine (`requires_payment_method` $\to$ `requires_action` $\to$ `processing` $\to$ `succeeded`). If a bank demands OTP verification, Stripe automatically pauses and returns `requires_action`, allowing the frontend SDK to display the bank's authentication modal.

### Q2: Why is using Webhooks mandatory for fulfilling purchases rather than relying on the frontend redirect?
> **Answer:** Relying on the frontend `confirmPayment()` callback to fulfill an order (e.g. provisioning a subscription in your database) is a critical vulnerability:
> 1. The user might close their browser tab or lose internet connection right after the card is charged, before the frontend can notify your backend.
> 2. A malicious user could forge a fake frontend success callback without ever paying.
> - **Webhooks are the sole source of truth**: Stripe's backend sends an authoritative, retried, cryptographically signed server-to-server HTTP POST (`payment_intent.succeeded`) directly to your API.

### Q3: How do Idempotency Keys prevent double charging?
> **Answer:** If your backend sends `POST /v1/payment_intents` to charge a customer $50, but the connection drops before receiving Stripe's response, your backend doesn't know if the charge succeeded or timed out.
> - By attaching an **`Idempotency-Key: <unique_uuid>`** header, if your backend retries the exact same request with the same UUID, Stripe recognizes the key and returns the cached result of the original charge rather than charging the customer a second time.

---

## 5. Common "Gotcha" Questions Interviewers Ask

### Gotcha 1: "What happens if your database is down when Stripe delivers a webhook event?"
- **The Answer:** If your server returns an HTTP error status (`500`, `502`, `504`) or times out, Stripe automatically schedules **exponential backoff retries** over the next 72 hours (retrying after 1 min, 5 mins, 30 mins, up to days). Once your database recovers, your webhook handler processes the retried event and returns `200 OK`.
