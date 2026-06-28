# Phase 3: Transport Layer ⚡

This report covers port-based addressing, end-to-end reliability, connection mechanics, and flow/congestion controls.

---

## 1. TCP vs. UDP

| Feature | TCP (Transmission Control Protocol) | UDP (User Datagram Protocol) |
| :--- | :--- | :--- |
| **Connection Type** | Connection-oriented (Handshake required) | Connectionless |
| **Reliability** | Guaranteed (Retransmits lost packets) | Best-effort (No retransmissions) |
| **Data Order** | Guaranteed in-order delivery (Uses sequence numbers) | Unordered delivery |
| **Data Flow** | Byte-stream oriented | Message/Datagram oriented (Preserves boundaries) |
| **Header Size** | 20 to 60 bytes | Exactly 8 bytes |
| **Flow & Congestion**| Active (Sliding window & Congestion windows) | None |
| **Transmission Speed**| Slower (due to acknowledgement & setup overhead) | Fast (no synchronization or validation overhead) |
| **Common Use Cases** | HTTP/1.1, HTTP/2, SSH, SMTP, FTP | DNS, DHCP, VoIP, Video Streaming, Gaming |

---

## 2. TCP Three-Way Handshake

Establishes a synchronized, reliable connection between a client and a server.

```
Client (SYN_SENT)                                         Server (LISTEN)
       │                                                         │
       │ ───────────── SYN (Seq=X) ────────────────────────────► │ [Enters SYN_RCVD]
       │                                                         │
       │ ◄──────────── SYN-ACK (Seq=Y, Ack=X+1) ──────────────── │
       │                                                         │
       │ ───────────── ACK (Seq=X+1, Ack=Y+1) ─────────────────► │ [Enters ESTABLISHED]
       ▼                                                         ▼
[Enters ESTABLISHED]
```

1. **SYN:** Client sends a `SYN` segment to server with an Initial Sequence Number (`Seq=X`) to initiate synchronization.
2. **SYN-ACK:** Server responds with `SYN-ACK`, setting its sequence number `Seq=Y` and verifying client's number by setting `Ack=X+1`.
3. **ACK:** Client responds with `ACK` (`Ack=Y+1`), completing connection setup. Both sockets enter the `ESTABLISHED` state.

---

## 3. TCP Connection Termination (Four-Way Handshake)

Gracefully closes an active TCP connection. Either side can initiate the close.

```
Client (ESTABLISHED)                                      Server (ESTABLISHED)
       │                                                         │
       │ ───────────── FIN ────────────────────────────────────► │ [Enters CLOSE_WAIT]
       │                                                         │
       │ ◄──────────── ACK ───────────────────────────────────── │ [Client enters FIN_WAIT_2]
       │                                                         │
       │ ◄──────────── FIN ───────────────────────────────────── │ [Enters LAST_ACK]
       │                                                         │
       │ ───────────── ACK ────────────────────────────────────► │ [Enters CLOSED]
       ▼                                                         ▼
[Enters TIME_WAIT (2MSL)] ──► [CLOSED]
```

### The `TIME_WAIT` State
The initiating closer enters the `TIME_WAIT` state for **2MSL** (Maximum Segment Lifetime, usually 2–4 minutes) before fully closing.
* **Why it exists:**
  1. Ensures the final `ACK` is received. If the `ACK` is lost, the server will retransmit its `FIN` packet. The client must remain active to receive and respond to it.
  2. Allows delayed packets to expire, preventing conflicts in subsequent connections reusing the same ports.

---

## 4. Flow Control & Sliding Window

Flow control prevents a fast sender from overwhelming a slow receiver's buffer space.

```
            [◀───────── Receiver Buffer Size ─────────]
            ┌───────────────────┬─────────────────────┐
            │   Buffered Data   │  Free Window Space  │
            └───────────────────┴─────────────────────┘
                                [◄─── rwnd advertised ─]
```

* **Mechanism:** The receiver advertises its available buffer space in the **Receiver Window (`rwnd`)** header field of every ACK packet.
* **Sliding Window:** The sender tracks the advertised `rwnd` and limits the amount of unacknowledged data in transit to match it, dynamically adjusting the transmission window.

---

## 5. Congestion Control

Congestion control prevents a sender from overloading intermediate network routes and devices.

The sender maintains a **Congestion Window (`cwnd`)** and a threshold value **`ssthresh`**.

```
  Window Size (cwnd)
     ▲
     │                 / \  (Packet Loss Event)
     │                /   \ ──► ssthresh set to cwnd / 2
     │  __           /     \_____ (Congestion Avoidance: linear +1)
     │ /  \         /
     │/    \_______/ (Slow Start: exponential x2)
     └────────────────────────────────────────► Time (RTT)
```

### The Four Phases
1. **Slow Start:** Starts with `cwnd = 1` MSS. The window size doubles every RTT (exponential growth) until it reaches `ssthresh`.
2. **Congestion Avoidance:** Once `cwnd >= ssthresh`, window size grows linearly by 1 MSS per RTT to probe network capacity.
3. **Fast Retransmit:** If the sender receives **3 duplicate ACKs** for a packet, it immediately assumes the packet was lost and retransmits it without waiting for the retransmission timeout (RTO).
4. **Fast Recovery:** Upon receiving 3 duplicate ACKs, `ssthresh` is set to `cwnd / 2`, `cwnd` is halved, and the sender resumes Congestion Avoidance directly (linear growth), skipping the Slow Start phase.
