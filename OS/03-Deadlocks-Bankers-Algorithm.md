# Deadlock Theory, Prevention, Avoidance & Banker's Algorithm

> **Scope:** Formal Definitions of Deadlock, Starvation & Livelock, The 4 Coffman Conditions, Resource Allocation Graphs (RAG), Deadlock Prevention Strategies, Deadlock Avoidance & Banker's Algorithm, Deadlock Detection via Wait-For Graphs, and Recovery Mechanisms.

---

# Table of Contents
1. [Deadlock, Starvation & Livelock Definitions](#1-deadlock-starvation--livelock-definitions)
2. [The 4 Coffman Conditions](#2-the-4-coffman-conditions)
3. [Resource Allocation Graph (RAG) Analysis](#3-resource-allocation-graph-rag-analysis)
4. [Deadlock Handling Strategies Matrix](#4-deadlock-handling-strategies-matrix)
5. [Deadlock Prevention Protocols](#5-deadlock-prevention-protocols)
6. [Deadlock Avoidance & The Banker's Algorithm](#6-deadlock-avoidance--the-bankers-algorithm)
7. [Deterministic Banker's Algorithm Matrix Trace](#7-deterministic-bankers-algorithm-matrix-trace)
8. [Deadlock Detection & Recovery Mechanics](#8-deadlock-detection--recovery-mechanics)
9. [Core Theoretical Summary Principles](#9-core-theoretical-summary-principles)

---

# 1. Deadlock, Starvation & Livelock Definitions

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | DEADLOCK                      | STARVATION                   | LIVELOCK                    |
+---------------------------------------------------------------------------------------------------+
| Formal Definition    | A set of processes is blocked | A runnable process waits     | Processes actively alter    |
|                      | because each holds a resource | indefinitely due to greedy   | execution states in lockstep|
|                      | and waits for another resource| higher-priority scheduling   | without progressing forward |
|                      | held by another process in set|                              |                             |
| Process State        | Blocked / Waiting (Sleeping)  | Ready (In Ready Queue)       | Running (Consuming CPU)     |
| CPU Consumption      | 0% CPU cycles consumed        | Normal CPU by active tasks   | 100% CPU busy-spinning      |
| Resolution Required  | External preemption/abort     | Aging / Fair scheduling      | Randomized back-off retry   |
+---------------------------------------------------------------------------------------------------+
```

---

# 2. The 4 Coffman Conditions

All **four conditions must hold simultaneously** for a deadlock state to occur. Invalidating any single condition guarantees deadlock-free operation:

```
+---------------------------------------------------------------------------------------------------+
| COFFMAN CONDITION        | FORMAL DEFINITION                        | STRUCTURAL PREVENTION METHOD|
+---------------------------------------------------------------------------------------------------+
| 1. Mutual Exclusion      | At least one resource must be held in a  | Make resources sharable or  |
|                          | non-shareable mode (exclusive access).   | virtualize access (Spooling)|
+---------------------------------------------------------------------------------------------------+
| 2. Hold and Wait         | A process holds >= 1 resource while      | Request ALL resources       |
|                          | waiting to acquire additional resources. | simultaneously upfront      |
+---------------------------------------------------------------------------------------------------+
| 3. No Preemption         | Resources cannot be preempted; can only  | Forcibly release all held   |
|                          | be released voluntarily by the holder.   | resources on request failure|
+---------------------------------------------------------------------------------------------------+
| 4. Circular Wait         | A closed dependency chain exists:        | Enforce global linear       |
|                          | P0 waits for P1, P1 for P2, ..., Pn for P0.| resource ordering F(R)    |
+---------------------------------------------------------------------------------------------------+
```

---

# 3. Resource Allocation Graph (RAG) Analysis

A Resource Allocation Graph is a directed graph $G = (V, E)$ where vertices $V$ are partitioned into Processes $P = \{P_1, \dots, P_n\}$ and Resource types $R = \{R_1, \dots, R_m\}$:
- **Request Edge ($P_i \to R_j$):** Process $P_i$ is blocked waiting for an instance of resource type $R_j$.
- **Assignment Edge ($R_j \to P_i$):** An instance of resource type $R_j$ is allocated to process $P_i$.

### Cycle Evaluation Theorems:
1. **Single-Instance Resource Types:** A directed cycle in the Resource Allocation Graph is a **necessary and sufficient condition** for deadlock.
2. **Multi-Instance Resource Types:** A directed cycle is a **necessary condition but NOT a sufficient condition**. If un-involved processes holding resource instances terminate and release their assignments, the cycle may break dynamically.

---

# 4. Deadlock Handling Strategies Matrix

```
+---------------------------------------------------------------------------------------------------+
| STRATEGY             | EXECUTION PHASE         | PRIMARY MECHANISM          | SYSTEM OVERHEAD     |
+---------------------------------------------------------------------------------------------------+
| 1. Prevention        | Design / Compile-Time   | Restrict resource requests | Zero runtime check; |
|                      |                         | to break 1 of 4 conditions | Low utilization     |
+---------------------------------------------------------------------------------------------------+
| 2. Avoidance         | Dynamic Run-Time        | Evaluate safe state before | High (Requires Max  |
| (Banker's Algorithm) | (Every Allocation)      | granting resource requests | claim declarations) |
+---------------------------------------------------------------------------------------------------+
| 3. Detection/Recovery| Periodic Background     | Inspect Wait-For Graph,    | Zero allocation lag;|
|                      | Thread                  | abort/preempt on deadlock  | Abort/rollback cost |
+---------------------------------------------------------------------------------------------------+
| 4. Ignorance         | None                    | Ignore deadlock possibility| Zero runtime        |
| (Ostrich Algorithm)  |                         | (General-Purpose OS default)| overhead            |
+---------------------------------------------------------------------------------------------------+
```

---

# 5. Deadlock Prevention Protocols

### 1. Eliminating Mutual Exclusion
- Construct shareable read-only access where applicable.
- Virtualize exclusive devices via daemons and spooling subsystems (e.g. print spoolers).

### 2. Eliminating Hold and Wait
- **Static Protocol:** A process must declare and acquire all required resources atomically before beginning execution.
- **Dynamic Protocol:** A process must release all currently allocated resources before submitting a new resource request.

### 3. Eliminating No Preemption
- If process $P_1$ holding resources requests resource $R_1$ which cannot be immediately allocated, all resources currently held by $P_1$ are implicitly preempted and returned to the available pool.

### 4. Eliminating Circular Wait (Global Linear Ordering)
- Define a 1-to-1 indexing function $F: R \to \mathbb{N}$ over all resource types.
- **The Ordering Rule:** A process holding resource $R_i$ can request resource $R_j$ if and only if $F(R_j) > F(R_i)$.
- **Proof of Correctness:** A circular dependency requires $F(R_0) < F(R_1) < \dots < F(R_k) < F(R_0)$, producing a mathematical contradiction $F(R_0) < F(R_0)$.

---

# 6. Deadlock Avoidance & The Banker's Algorithm

Deadlock avoidance dynamically evaluates every allocation request, ensuring the system remains strictly within a **Safe State**.

```
                             +-------------------------------+
                             |       ALL POSSIBLE STATES     |
                             |   +-----------------------+   |
                             |   |      SAFE STATES      |   |
                             |   |   (Safe sequence exists   |
                             |   |    => ZERO DEADLOCK)      |
                             |   +-----------------------+   |
                             |               |               |
                             |   +-----------------------+   |
                             |   |     UNSAFE STATES     |   |
                             |   |   (May lead to deadlock)  |
                             |   |       +-----------+   |   |
                             |   |       | DEADLOCK  |   |   |
                             |   |       +-----------+   |   |
                             |   +-----------------------+   |
                             +-------------------------------+
```

- **Definition of Safe State:** A state is safe if there exists a sequence $\langle P_1, P_2, \dots, P_n \rangle$ such that for each $P_i$, the maximum remaining resources $P_i$ can request can be satisfied by current $\text{Available} + \sum_{j < i} \text{Allocation}_j$.
- **Theorem:** A safe state is not deadlocked. An unsafe state is not necessarily deadlocked, but can transition into a deadlock state under worst-case runtime requests.

---

# 7. Deterministic Banker's Algorithm Matrix Trace

### System State Setup:
- 5 Processes: $P_0, P_1, P_2, P_3, P_4$
- 3 Resource Types: $A$ (10 total instances), $B$ (5 instances), $C$ (7 instances).

```
+---------+------------------+------------------+------------------+
| Process | Allocation (A B C)| Max Need (A B C) | Available (A B C)|
+---------+------------------+------------------+------------------+
| P0      | 0  1  0          | 7  5  3          | 3  3  2          |
| P1      | 2  0  0          | 3  2  2          |                  |
| P2      | 3  0  2          | 9  0  2          |                  |
| P3      | 2  1  1          | 2  2  2          |                  |
| P4      | 0  0  2          | 4  3  3          |                  |
+---------+------------------+------------------+------------------+
```

### Step 1: Compute Need Matrix ($\text{Need}[i, j] = \text{Max}[i, j] - \text{Allocation}[i, j]$)
```
+---------+---------------+
| Process | Need (A B C)  |
+---------+---------------+
| P0      | 7  4  3       |
| P1      | 1  2  2       |
| P2      | 6  0  0       |
| P3      | 0  1  1       |
| P4      | 4  3  1       |
+---------+---------------+
```

### Step 2: Safety Algorithm Verification Trace
- Set $\text{Work} = \text{Available} = [3, 3, 2]$. Initialize $\text{Finish}[i] = \text{False}$ for all $i$.
- **Evaluate P0:** $\text{Need}_0 [7, 4, 3] \le [3, 3, 2]$ -> **False**.
- **Evaluate P1:** $\text{Need}_1 [1, 2, 2] \le [3, 3, 2]$ -> **True**.
  - $\text{Work} \leftarrow [3, 3, 2] + \text{Alloc}_1 [2, 0, 0] = [5, 3, 2]$. $\text{Finish}[1] \leftarrow \text{True}$.
- **Evaluate P3:** $\text{Need}_3 [0, 1, 1] \le [5, 3, 2]$ -> **True**.
  - $\text{Work} \leftarrow [5, 3, 2] + \text{Alloc}_3 [2, 1, 1] = [7, 4, 3]$. $\text{Finish}[3] \leftarrow \text{True}$.
- **Evaluate P0:** $\text{Need}_0 [7, 4, 3] \le [7, 4, 3]$ -> **True**.
  - $\text{Work} \leftarrow [7, 4, 3] + \text{Alloc}_0 [0, 1, 0] = [7, 5, 3]$. $\text{Finish}[0] \leftarrow \text{True}$.
- **Evaluate P2:** $\text{Need}_2 [6, 0, 0] \le [7, 5, 3]$ -> **True**.
  - $\text{Work} \leftarrow [7, 5, 3] + \text{Alloc}_2 [3, 0, 2] = [10, 5, 5]$. $\text{Finish}[2] \leftarrow \text{True}$.
- **Evaluate P4:** $\text{Need}_4 [4, 3, 1] \le [10, 5, 5]$ -> **True**.
  - $\text{Work} \leftarrow [10, 5, 5] + \text{Alloc}_4 [0, 0, 2] = [10, 5, 7]$. $\text{Finish}[4] \leftarrow \text{True}$.

**Result:** System is in a **Safe State** with valid safe sequence $\langle P_1, P_3, P_0, P_2, P_4 \rangle$.

---

# 8. Deadlock Detection & Recovery Mechanics

### 1. Wait-For Graph (WFG) Detection
For single-instance resource systems, the kernel maintains a directed graph where nodes represent processes:
- Directed edge $P_i \to P_j$ indicates process $P_i$ is waiting for process $P_j$ to release a resource.
- The kernel periodically executes cycle detection algorithms (DFS / Tarjan's strongly connected components) in $O(V + E)$ time.

### 2. Recovery Strategies
1. **Process Termination:**
   - Abort all deadlocked processes (High cost; discards partial computation).
   - Abort one process at a time in the cycle until the deadlock cycle is broken.
2. **Resource Preemption:**
   - **Victim Selection:** Determine which process to preempt based on priority, consumed CPU time, and allocated resources.
   - **Rollback:** Roll back the victim process to a safe checkpoint state.
   - **Starvation Prevention:** Track preemption counts per process; if a process is selected as victim repeatedly, artificially boost its priority.

---

# 9. Core Theoretical Summary Principles

1. **Deadlock Conditions:** Deadlock requires all four Coffman conditions (Mutual Exclusion, Hold and Wait, No Preemption, Circular Wait) to hold concurrently.
2. **RAG Cycles:** A cycle in a Resource Allocation Graph guarantees deadlock only if all involved resource types contain single instances.
3. **Safe State Invariant:** A safe state guarantees the existence of at least one execution ordering (safe sequence) permitting all tasks to satisfy maximum resource claims.
4. **Global Resource Ordering:** Deadlock prevention by assigning monotonic ranks $F(R)$ to resource types mathematically eliminates circular wait conditions.
