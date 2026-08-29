# Master Guide 03: Deadlocks & Banker's Algorithm

> **Focus:** Deadlock vs Starvation vs Livelock, The 4 Coffman Conditions, Deadlock Prevention vs Avoidance vs Detection, Resource Allocation Graphs (RAG), Banker's Algorithm (Safety & Request Matrices), and Fast-Track Matrix Drills.
> 
> *Targeted for Top-Tier Tech System & Backend Engineering Interviews.*

---

# Table of Contents
1. [Deadlock vs. Starvation vs. Livelock](#1-deadlock-vs-starvation-vs-livelock)
2. [The 4 Coffman Conditions (Acronym: MHNC)](#2-the-4-coffman-conditions-acronym-mhnc)
3. [The Deadlock Handling Triad: Prevention vs. Avoidance vs. Detection](#3-the-deadlock-handling-triad-prevention-vs-avoidance-vs-detection)
4. [Deadlock Prevention (Breaking the 4 Conditions)](#4-deadlock-prevention-breaking-the-4-conditions)
5. [Deadlock Avoidance & Banker's Algorithm](#5-deadlock-avoidance--bankers-algorithm)
6. [Fast-Track Banker's Algorithm Matrix Drill](#6-fast-track-bankers-algorithm-matrix-drill)
7. [Deadlock Detection & Recovery Mechanics](#7-deadlock-detection--recovery-mechanics)
8. [High-Frequency Interview Drill & Verbal Q&A](#8-high-frequency-interview-drill--verbal-qa)

---

# 1. Deadlock vs. Starvation vs. Livelock

```
+---------------------------------------------------------------------------------------------------+
| PROPERTY             | DEADLOCK                      | STARVATION                   | LIVELOCK                    |
+---------------------------------------------------------------------------------------------------+
| Definition           | Set of processes permanently  | Process waits indefinitely   | Processes actively change   |
|                      | blocked waiting for each other| due to greedy higher-priority| states in response to each  |
|                      | (Circular dependency)         | scheduling decisions         | other without doing real work|
| Process State        | Blocked / Waiting (Sleeping)  | Ready (waiting in Ready Queue)| Running (Consuming 100% CPU)|
| CPU Utilization      | 0% CPU consumed               | Normal CPU consumed by others| 100% CPU busy-spinning      |
| Resolution Needed    | External OS/Admin intervention| Aging / Fair scheduling      | Randomized back-off / retry |
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:**
  - **Deadlock:** Two people in a narrow hallway facing each other, neither taking a step (frozen).
  - **Starvation:** A quiet person at a buffet who never gets food because aggressive people keep cutting in front.
  - **Livelock:** Two polite people in a hallway constantly sidestepping in the same direction at the exact same time (moving, but zero progress).
- **The Interview Trap:** Confusing deadlock with starvation. A starved process *can* run if high-priority traffic stops; a deadlocked process can *never* run without external intervention.
- **30-Second Verbal Answer:** **"Deadlock is a permanent blocking state where every process in a set holds a resource and waits for another resource held by another process in that set. Starvation is indefinite delay due to unfair scheduling where the process remains ready. Livelock is active CPU spinning where processes continuously change state in response to each other without making forward progress."**

---

# 2. The 4 Coffman Conditions (Acronym: MHNC)

All **4 conditions must hold simultaneously** for a deadlock to exist. Breaking even ONE condition guarantees a deadlock-free system.

```
+---------------------------------------------------------------------------------------------------+
| CONDITION                | MEANING                                  | HOW TO ELIMINATE / BREAK IT |
+---------------------------------------------------------------------------------------------------+
| 1. Mutual Exclusion (M)  | At least one resource must be non-       | Make resources shareable or |
|                          | shareable (only 1 holder at a time).     | virtualize (e.g. Spooling)  |
+---------------------------------------------------------------------------------------------------+
| 2. Hold and Wait (H)     | A process holds at least 1 resource while| Request ALL resources at    |
|                          | waiting to acquire additional resources. | once up front (or release all)|
+---------------------------------------------------------------------------------------------------+
| 3. No Preemption (N)     | Resources cannot be forcibly taken; only | If request fails, preempt & |
|                          | released voluntarily by the holder.      | release all held resources  |
+---------------------------------------------------------------------------------------------------+
| 4. Circular Wait (C)     | A closed chain exists: P0 waits for P1,  | Enforce strict Global Linear|
|                          | P1 waits for P2, ..., Pn waits for P0.   | Resource Ordering (Id_A<Id_B)|
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** Acronym **MHNC** — **M**utual exclusion, **H**old & wait, **N**o preemption, **C**ircular wait.
- **The Interview Trap:** Thinking circular wait and circular resource allocation graphs are identical. A cycle in a Resource Allocation Graph (RAG) is a **necessary and sufficient condition ONLY for single-instance resources**. For multi-instance resources, a cycle is a necessary condition, but NOT sufficient.

---

# 3. The Deadlock Handling Triad: Prevention vs. Avoidance vs. Detection

```
+---------------------------------------------------------------------------------------------------+
| STRATEGY             | WHEN IT RUNS            | CORE MECHANISM             | SYSTEM OVERHEAD     |
+---------------------------------------------------------------------------------------------------+
| 1. Prevention        | Static / Compile-Time   | Constrain resource requests| Low runtime overhead|
|                      | (System Design Stage)   | to invalidate 1 of 4 rules | Low resource util.  |
+---------------------------------------------------------------------------------------------------+
| 2. Avoidance         | Dynamic / Run-Time      | Check safe state before    | High (Requires prior|
| (Banker's Algorithm) | (Every Allocation Step) | granting any request       | maximum claim info) |
+---------------------------------------------------------------------------------------------------+
| 3. Detection & Recov.| Periodic / Asynchronous | Allow deadlocks, detect via| Zero request delay; |
|                      | (Background Inspector)  | Wait-for graph, kill/abort | High cost on abort  |
+---------------------------------------------------------------------------------------------------+
| 4. Ostrich Algorithm | Never                   | Ignore the problem (Linux, | Zero overhead       |
|                      |                         | Windows default strategy)  |                     |
+---------------------------------------------------------------------------------------------------+
```

---

# 4. Deadlock Prevention (Breaking the 4 Conditions)

### 1. Breaking Mutual Exclusion
- Use lock-free data structures (atomic CAS operations) or spooling (e.g. printer spooler daemon queue).

### 2. Breaking Hold and Wait
- **Protocol 1:** Process must request and receive all resources before execution begins.
- **Protocol 2:** Process must release all currently held resources before requesting new ones.
- *Drawback:* Low resource utilization and starvation of processes needing many resources.

### 3. Breaking No Preemption
- If process $P$ holding resources requests resource $R$ which cannot be immediately allocated, all resources currently held by $P$ are **preempted** and added to the waiting pool.

### 4. Breaking Circular Wait (Most Practical in Real Systems)
- Impose a **global total order** on all resource types: $F: R \to \mathbb{N}$.
- Rule: A process can only request resource $R_j$ if $F(R_j) > F(R_i)$ for all currently held $R_i$.
- *Proof:* A circular wait requires an increasing sequence $F(R_0) < F(R_1) < \dots < F(R_n) < F(R_0)$, which is mathematically impossible.

---

# 5. Deadlock Avoidance & Banker's Algorithm

```
                             +-------------------------------+
                             |       ALL POSSIBLE STATES     |
                             |   +-----------------------+   |
                             |   |      SAFE STATES      |   |
                             |   |   (Safe sequence exists|  |
                             |   |    => NO DEADLOCK)    |   |
                             |   +-----------------------+   |
                             |               |               |
                             |   +-----------------------+   |
                             |   |     UNSAFE STATES     |   |
                             |   |   (MAY lead to        |   |
                             |   |    deadlock)          |   |
                             |   |       +-----------+   |   |
                             |   |       | DEADLOCK  |   |   |
                             |   |       +-----------+   |   |
                             |   +-----------------------+   |
                             +-------------------------------+
```

- **Definition of Safe State:** A state is safe if there exists at least one **Safe Sequence** $\langle P_1, P_2, \dots, P_n \rangle$ such that for each $P_i$, the maximum resources $P_i$ still needs can be satisfied by current $\text{Available} + \sum_{j < i} \text{Allocation}_j$.
- **Golden Rule:** **Safe State $\implies$ No Deadlock.** Unsafe State $\implies$ Potential Deadlock (not guaranteed, depends on future runtime requests).

---

# 6. Fast-Track Banker's Algorithm Matrix Drill

### Problem Setup:
- 5 Processes: $P_0, P_1, P_2, P_3, P_4$
- 3 Resource Types: $A$ (10 instances), $B$ (5 instances), $C$ (7 instances).

**Given State Table:**
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

### Step 1: Calculate Need Matrix ($\text{Need} = \text{Max} - \text{Allocation}$)
```
+---------+---------------+
| Process | Need (A B C)  |
+---------+---------------+
| P0      | 7  4  3       | (7-0, 5-1, 3-0)
| P1      | 1  2  2       | (3-2, 2-0, 2-0)
| P2      | 6  0  0       | (9-3, 0-0, 2-2)
| P3      | 0  1  1       | (2-2, 2-1, 2-1)
| P4      | 4  3  1       | (4-0, 3-0, 3-2)
+---------+---------------+
```

### Step 2: Safety Algorithm Execution Trace:
- $\text{Work} = \text{Available} = [3, 3, 2]$
- **Check P0:** $\text{Need}_0 [7, 4, 3] \le [3, 3, 2]$ -> **False** (Cannot run).
- **Check P1:** $\text{Need}_1 [1, 2, 2] \le [3, 3, 2]$ -> **True**!
  - P1 executes and completes.
  - $\text{New Work} = [3, 3, 2] + \text{Alloc}_1 [2, 0, 0] = [5, 3, 2]$.
  - Finish: `[P1]`.
- **Check P3:** $\text{Need}_3 [0, 1, 1] \le [5, 3, 2]$ -> **True**!
  - P3 executes and completes.
  - $\text{New Work} = [5, 3, 2] + \text{Alloc}_3 [2, 1, 1] = [7, 4, 3]$.
  - Finish: `[P1, P3]`.
- **Check P0:** $\text{Need}_0 [7, 4, 3] \le [7, 4, 3]$ -> **True**!
  - P0 executes and completes.
  - $\text{New Work} = [7, 4, 3] + \text{Alloc}_0 [0, 1, 0] = [7, 5, 3]$.
  - Finish: `[P1, P3, P0]`.
- **Check P2:** $\text{Need}_2 [6, 0, 0] \le [7, 5, 3]$ -> **True**!
  - P2 executes and completes.
  - $\text{New Work} = [7, 5, 3] + \text{Alloc}_2 [3, 0, 2] = [10, 5, 5]$.
  - Finish: `[P1, P3, P0, P2]`.
- **Check P4:** $\text{Need}_4 [4, 3, 1] \le [10, 5, 5]$ -> **True**!
  - P4 executes and completes.
  - $\text{New Work} = [10, 5, 5] + \text{Alloc}_4 [0, 0, 2] = [10, 5, 7]$.
  - Finish: `[P1, P3, P0, P2, P4]`.

**Conclusion:** System is in a **Safe State** with Safe Sequence: $\langle P_1, P_3, P_0, P_2, P_4 \rangle$.

### Resource Request Sub-Case:
- Suppose $P_1$ requests $[1, 0, 2]$.
  1. Check $\text{Request}_1 [1, 0, 2] \le \text{Need}_1 [1, 2, 2]$ -> **True**.
  2. Check $\text{Request}_1 [1, 0, 2] \le \text{Available} [3, 3, 2]$ -> **True**.
  3. Pretend allocation:
     - $\text{Available}' = [3, 3, 2] - [1, 0, 2] = [2, 3, 0]$
     - $\text{Allocation}_1' = [2, 0, 0] + [1, 0, 2] = [3, 0, 2]$
     - $\text{Need}_1' = [1, 2, 2] - [1, 0, 2] = [0, 2, 0]$
  4. Run Safety algorithm on modified state -> Safe sequence $\langle P_1, P_3, P_4, P_0, P_2 \rangle$ exists $\implies$ **Grant Request immediately**.

---

# 7. Deadlock Detection & Recovery Mechanics

### 1. Wait-For Graph (WFG) (Single Instance Resources)
- Remove resource nodes from RAG and collapse edges directly between processes ($P_i \to P_j$ means $P_i$ is waiting for $P_j$ to release a resource).
- Cycle in WFG $\iff$ **Deadlock exists**.
- Algorithmic Complexity: Run DFS/Tarjan's algorithm in $O(V + E)$ periodically.

### 2. Recovery Strategies
- **Process Termination:**
  - Option A: Abort all deadlocked processes (expensive, lost work).
  - Option B: Abort one process at a time until cycle is broken (based on lowest priority, lowest CPU time consumed, or highest resources held).
- **Resource Preemption:**
  - Select a victim, roll back to a safe checkpoint, and restart process.
  - *Risk:* **Starvation** (same process chosen as victim repeatedly; fixed by maintaining a rollback counter).

---

# 8. High-Frequency Interview Drill & Verbal Q&A

### Q1: Why don't general-purpose operating systems like Linux use Banker's Algorithm?
> **Answer:** Banker's Algorithm requires processes to declare their **maximum resource claims upfront** and assumes the total number of processes and resources remains static. In real desktop/server OSs, processes dynamically spawn, allocate arbitrary memory, and cannot know their peak resource requirements in advance.

### Q2: Does a cycle in a Resource Allocation Graph always indicate a deadlock?
> **Answer:** **No.** For **multi-instance resources**, a cycle is only a necessary condition, not sufficient. A deadlock only exists if there is no process in the cycle that can complete using resources from outside the cycle.

### Q3: What is the difference between Deadlock Prevention and Deadlock Avoidance?
> **Answer:** **Prevention** constrains how resource requests are designed statically to guarantee that at least one Coffman condition can never occur. **Avoidance** dynamically evaluates every single request at runtime to verify that granting it keeps the system in a Safe State.

### Q4: How does hierarchical resource ordering prevent deadlock?
> **Answer:** By assigning each resource an integer ID and forcing every thread to acquire locks in strictly **monotonically increasing ID order**, it is mathematically impossible to form a circular dependency chain ($P_0 \to P_1 \to \dots \to P_0$), breaking the Circular Wait condition.

### Q5: What is the difference between a Safe State and an Unsafe State?
> **Answer:** A **Safe State** guarantees that at least one execution ordering (safe sequence) exists where all processes can finish without deadlock. An **Unsafe State** does not guarantee a deadlock will happen, but the OS loses the ability to prevent processes from entering a deadlock if they all request their maximum claims.
