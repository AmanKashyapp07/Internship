# 02. Database Scaling, Replication & Sharding

> **Interview Mindset**: When the interviewer says: *"What if we scale from 10,000 to 10 million users?"*, they are testing whether you understand **where a single database breaks down** and how to scale it incrementally without introducing unnecessary distributed complexity.

---

## 1. Why the Simple Architecture Stops Working

```
SIMPLE ARCHITECTURE (Fails at Scale)
Client ──► Server ──► Database (Single Node)
```

A single database server will hit one of 4 physical resource limits:

| Bottleneck | Why It Happens | Symptom | How to Fix |
| :--- | :--- | :--- | :--- |
| **RAM (Buffer Pool)** | Active dataset & indexes exceed available memory. | Queries constantly hit disk; high latency. | Add RAM (Scale-Up) or add Redis cache. |
| **CPU** | Heavy computational queries, joins, aggregations, high QPS. | CPU spikes to 100%; incoming requests queue up. | Add Read Replicas, optimize indexes, denormalize. |
| **Disk I/O (IOPS)** | Write-heavy workloads exceed disk write throughput. | Write transactions block; database locks up. | Horizontal Sharding, LSM-Tree DBs, Write Queues. |
| **Connections** | Hundreds of web servers open too many DB connections. | `Error: Too many connections` / connection timeouts. | Connection Pooling (PgBouncer, HikariCP). |

---

## 2. Vertical vs Horizontal Scaling

```
VERTICAL SCALING (Scale-Up)                HORIZONTAL SCALING (Scale-Out)
    ┌────────────────┐                       ┌──────┐ ┌──────┐ ┌──────┐
    │  Bigger Box    │                       │Node 1│ │Node 2│ │Node 3│
    │ 64 Cores, 1TB  │                       └──────┘ └──────┘ └──────┘
    └────────────────┘                       Shared nothing, distributed cluster
```

| Dimension | Vertical Scaling (Scale-Up) | Horizontal Scaling (Scale-Out) |
| :--- | :--- | :--- |
| **Mechanism** | Upgrade CPU, RAM, NVMe SSD on the same machine | Add more commodity servers to a cluster |
| **Application Changes** | Zero changes required | High complexity (data routing, rebalancing) |
| **Downtime** | Usually requires downtime to swap hardware | Zero downtime; rolling additions |
| **Cost** | Becomes exponentially expensive at high tiers | Cost-effective commodity hardware |
| **Hard Ceiling** | Hardware limits exist (e.g. max 128 cores, 4TB RAM) | Virtually infinite scalability |
| **Golden Rule** | *"Start with Vertical Scaling + Indexing + Caching."* | *"Use only when single-node limits are exceeded."* |

---

## 3. Read Scaling: Primary + Read Replicas

When reads heavily outnumber writes (e.g., 95% reads, 5% writes), split traffic across a **Primary (Leader)** and **Read Replicas (Followers)**:

```
┌──────────────────────────────────────────────┐
│               Client Requests                │
└──────────────────────┬───────────────────────┘
                       │
                       ▼
┌──────────────────────────────────────────────┐
│             App Server / Router              │
└──────────────┬────────────────┬──────────────┘
               │ Writes         │ Reads
               ▼                ▼
┌─────────────────────────┐   ┌────────────────────────────────────────────────────────┐
│   Primary / Master DB   │   │                     Read Replicas                      │
│   (Handles ALL Writes)  │   │                (Handle ALL Read Queries)               │
└──────────────┬──────────┘   └────────────▲───────────────▲───────────────▲───────────┘
               │                           │               │               │
               └───────────────────────────┴───────────────┴───────────────┘
                                Async Binary Log Stream
```

### Replication Modes & Trade-offs:
- **Asynchronous Replication**: Primary writes locally, returns `SUCCESS` immediately to client. Replicas pull updates in the background. (Fastest latency, but risk of data loss if Primary crashes before replica syncs).
- **Synchronous Replication**: Primary waits for **all** replicas to acknowledge before returning `SUCCESS`. (Zero data loss, but slowest write latency).
- **Semi-Synchronous Replication**: Primary waits for at least **one** replica to acknowledge before returning. (Best production compromise).

---

### The #1 Replication Trap: Replication Lag & Stale Reads

**The Problem**:
1. User updates their profile name to *"Aman"*.
2. Write hits Primary asynchronously.
3. User immediately refreshes the page.
4. Read request routes to `Replica 2`, which is 50ms behind.
5. User sees their old name and reports a bug.

#### How to Solve in an Interview:
1. **Read-Your-Own-Writes**: For 5–10 seconds after a user performs a write, route that specific user's read requests to the **Primary DB**.
2. **Version Tokens**: Write returns a revision token (e.g., `v=42`). Client requests must wait until the replica reaches `v >= 42`, or fallback to Primary.
3. **Cache Updates**: Writes update the in-memory cache directly, ensuring subsequent reads hit fresh cache.

---

## 4. Write Scaling: Partitioning & Sharding

When writes bottleneck the Primary DB, or the dataset grows beyond a single server's disk (multi-terabytes), you must split the data.

### Vertical vs Horizontal Partitioning

```
VERTICAL PARTITIONING (Split Columns by Domain)
Original Table: [User_ID, Name, Email, PasswordHash, Bio, AvatarBlob]
Table A (Auth):    [User_ID, Email, PasswordHash]
Table B (Profile): [User_ID, Name, Bio, AvatarBlob]

HORIZONTAL PARTITIONING / SHARDING (Split Rows by Key)
Shard 1: Stores rows where User_ID is 1 to 1,000,000
Shard 2: Stores rows where User_ID is 1,000,001 to 2,000,000
Shard 3: Stores rows where User_ID is 2,000,001 to 3,000,000
```

---

### Sharding Key Strategies

#### 1. Range-Based Sharding
- Example: Shard 1 (`Users A-H`), Shard 2 (`Users I-P`), Shard 3 (`Users Q-Z`) or by year (`2024`, `2025`, `2026`).
- **Con**: **Hotspots!** If sharding by date, 99% of today's writes hammer the current shard, leaving old shards idle.

#### 2. Hash-Based Sharding
- Formula: `shard_id = hash(user_id) % N` (where $N$ is number of shards).
- **Con**: Adding a new shard ($N \to N+1$) changes the hash modulo for almost **every existing key**, triggering massive data migration.

#### 3. Consistent Hashing (The Industry Standard)
- Maps both servers and data keys onto a circular $2^{32}-1$ hash ring.
- When a server is added or removed, **only $K/N$ keys move on average** ($K$ = keys, $N$ = nodes).
- Uses **Virtual Nodes** to prevent uneven data distribution on the ring.

---

### The 4 Major Costs of Sharding (Always mention in interviews!)

```
┌─────────────────────────┐     1. Cross-Shard Joins: Joining data across network nodes is slow.
│                         ├─────►2. Distributed Transactions: Ensuring ACID across shards requires 2PC.
│  The 4 Costs of         ├─────►3. The Celebrity / Hotspot Problem: High-profile users crash their shard.
│      Sharding           └─────►4. Operational Complexity: Re-sharding production data is high-risk.
└─────────────────────────┘
```

---

## 5. High Availability (HA) & Failover

What happens when the Primary DB hardware dies?

```
┌──────────────────────────────────────────────┐
│             1. Application Tier              │
└──────────────┬────────────────────────▲──────┘
               │ 1. Normal Writes       │ 6. Resumes writes
               ▼                        │    to new Primary
┌─────────────────────────┐             │
│ 2. Primary DB (Active)  │             │
│      💥 CRASHES!        │             │
└──────────────┬──────────┘             │
               │                        │
               │ 2. Async Sync          │
               ▼                        │
┌─────────────────────────┐             │
│   3. Standby Replica    ├─────────────┘
│ (Promoted to Primary!)  │
└──────────────▲──────────┘
               │
               │ 4. Promotes Standby & 5. Updates DNS
               │
┌──────────────┴──────────┐
│ 4. Health Sentinel /    │
│    Quorum Monitor       │
└─────────────────────────┘
```

### The Split-Brain Problem & Quorum
- **Split-Brain**: If a network glitch cuts communication between Primary and Standby, the Standby might declare the Primary dead and promote itself, while the Primary is still alive. You now have **two primaries writing conflicting data**!
- **The Fix (Quorum Consensus)**: Always use an odd number of health monitors (e.g., 3 or 5). Promoting a replica requires a **majority vote ($N/2 + 1$)**.

---

## 6. CAP & PACELC Theorems (The Basic Understanding)

### CAP Theorem
In a distributed system, you can choose at most **two** out of three:
- **Consistency ($C$)**: Every read receives the latest write or an error.
- **Availability ($A$)**: Every non-failing node returns a successful response (may be stale).
- **Partition Tolerance ($P$)**: The system continues operating despite dropped network packets.

> [!CAUTION]
> **Interview Rule**: Network partitions ($P$) are unavoidable in real distributed hardware. Therefore, the real question is: **During a network partition, do you choose CP (Consistency) or AP (Availability)?**

```
┌─────────────────────────────────────────────────────────────────┐
│     Network Partition (Node 1 & Node 2 cannot communicate)      │
└────────────────────────────────┬────────────────────────────────┘
                                 │
                 ┌───────────────┴───────────────┐
                 ▼                               ▼
   ┌───────────────────────────┐   ┌───────────────────────────┐
   │          Node 1           │   │          Node 2           │
   │    Receives Write 'x=5'   │   │     Still holds 'x=1'     │
   └─────────────┬─────────────┘   └─────────────┬─────────────┘
                 │ ⚡ Network Cut (Partition) ⚡  │
                 └ - - - - - - - - - - - - - - - ┘
                                 │
           ┌─────────────────────┴─────────────────────┐
           ▼                                           ▼
┌─────────────────────────────┐             ┌─────────────────────────────┐
│   Choose CP (Consistency)   │             │   Choose AP (Availability)  │
│Node 2 refuses reads / errors│             │Node 2 returns stale data    │
│out until network heals.     │             │('x=1') to callers.          │
│Result: Consistent, but DOWN │             │Result: Available, but STALE │
└─────────────────────────────┘             └─────────────────────────────┘
```

- **CP Systems**: Banking balances, distributed locks (HBase, MongoDB majority, ZooKeeper).
- **AP Systems**: Social media likes, shopping carts, video views (Cassandra, DynamoDB, DNS).

---

## 7. What Changes at "10 Million Users"?

When an interviewer asks: *"How do we handle 10 million daily users?"*, give this structured answer:

```
1. "Single DB reaches connection and CPU limits."
   ──► Add Connection Pooler (PgBouncer) + Scale Up instance.

2. "Read volume dominates (90%+ reads)."
   ──► Add Redis Cache in front of DB + add 2-3 Read Replicas.

3. "Search queries take too long."
   ──► Add Composite B+ Tree Indexes following Leftmost Prefix Rule.

4. "Write throughput or storage exceeds a single box (Terabytes of data)."
   ──► Shard the database horizontally using Consistent Hashing.
```
