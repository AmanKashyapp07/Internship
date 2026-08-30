# CPU Scheduling Algorithms & Dispatcher Theory

> **Scope:** CPU Scheduling Objectives & Metrics, Preemptive vs. Non-Preemptive Execution, Classic Algorithms (FCFS, SJF, SRTF, Round Robin, Priority Scheduling, Priority Inversion & PIP), Multi-Level Feedback Queues (MLFQ), and Linux Completely Fair Scheduler (CFS) Mechanics.

---

# Table of Contents
1. [CPU Scheduling Objectives & Evaluation Metrics](#1-cpu-scheduling-objectives--evaluation-metrics)
2. [Algorithm Classification Matrix](#2-algorithm-classification-matrix)
3. [Classic CPU Scheduling Algorithms](#3-classic-cpu-scheduling-algorithms)
4. [Priority Inversion & The Priority Inheritance Protocol (PIP)](#4-priority-inversion--the-priority-inheritance-protocol-pip)
5. [Multi-Level Feedback Queue (MLFQ) Architecture](#5-multi-level-feedback-queue-mlfq-architecture)
6. [Linux Completely Fair Scheduler (CFS) Internals](#6-linux-completely-fair-scheduler-cfs-internals)
7. [Deterministic Execution Traces](#7-deterministic-execution-traces)
8. [Core Theoretical Summary Principles](#8-core-theoretical-summary-principles)

---

# 1. CPU Scheduling Objectives & Evaluation Metrics

The CPU Scheduler (Short-Term Scheduler) selects an executable task from the Ready Queue and allocates the CPU core. Scheduling policies optimize across competing systems metrics:

```
+---------------------------------------------------------------------------------------------------+
| METRIC                    | FORMAL DEFINITION & FORMULA               | OPTIMIZATION GOAL         |
+---------------------------------------------------------------------------------------------------+
| Arrival Time (AT)         | Timestamp when a process enters Ready Queue| Input parameter          |
| Burst Time (BT)           | Total CPU execution duration required     | Input parameter           |
| Completion Time (CT)      | Timestamp when a process finishes execution| Derived execution time    |
| Turnaround Time (TAT)     | TAT = Completion Time (CT) - Arrival Time (AT)| Minimize average TAT  |
| Waiting Time (WT)         | WT = Turnaround Time (TAT) - Burst Time (BT) | Minimize average WT    |
| Response Time (RT)        | RT = First Execution Time - Arrival Time (AT)| Minimize for interactive  |
| CPU Utilization           | Fraction of time CPU executes user/sys tasks| Maximize (Near 100%)    |
| Throughput                | Number of completed processes per unit time| Maximize                 |
+---------------------------------------------------------------------------------------------------+
```

---

# 2. Algorithm Classification Matrix

```
+---------------------------------------------------------------------------------------------------+
| ALGORITHM                            | PREEMPTION TYPE | PRIMARY ADVANTAGE      | KNOWN LIMITATION        |
+---------------------------------------------------------------------------------------------------+
| First-Come, First-Served (FCFS)      | Non-Preemptive  | Zero scheduling overhead| Convoy Effect           |
| Shortest Job First (SJF)             | Non-Preemptive  | Minimizes Average WT   | Starvation of long jobs |
| Shortest Remaining Time First (SRTF) | Preemptive      | Provably Optimal Avg WT| Requires burst estimates|
| Round Robin (RR)                     | Preemptive      | Fair response latency  | High context switch cost|
| Priority Scheduling                  | Both            | Enforces task urgency  | Starvation / Inversion  |
| Multi-Level Feedback Queue (MLFQ)    | Preemptive      | Adaptive multi-workload| Complex parameter tuning|
| Completely Fair Scheduler (CFS)      | Preemptive      | Proportional fairness  | O(log N) RB-Tree updates|
+---------------------------------------------------------------------------------------------------+
```

---

# 3. Classic CPU Scheduling Algorithms

### A. First-Come, First-Served (FCFS)
- **Mechanics:** Tasks are dispatched strictly according to arrival order in a FIFO queue.
- **The Convoy Effect:** If a long CPU-bound process acquires the CPU, all subsequent short I/O-bound tasks are blocked in the Ready Queue, resulting in low device utilization and elevated average waiting time.

### B. Shortest Job First (SJF) & Shortest Remaining Time First (SRTF)
- **Mechanics:** Selects the ready task with the smallest next CPU burst duration. SRTF preempts the running process if a newly arrived task has a shorter remaining burst.
- **Optimality Proof:** By scheduling tasks in monotonically increasing burst lengths, SRTF minimizes $\sum \text{WT}$, yielding the mathematically minimal average waiting time.
- **Burst Duration Estimation (Exponential Smoothing):**
  $$\tau_{n+1} = \alpha t_n + (1 - \alpha) \tau_n \quad (0 \le \alpha \le 1)$$
  - $t_n$: Actual duration of the $n$-th CPU burst.
  - $\tau_n$: Predicted duration of the $n$-th CPU burst.
  - $\alpha$: Weighting factor balancing recent history against historical baseline.

### C. Round Robin (RR)
- **Mechanics:** Designed for time-sharing systems. The scheduler cycles through the ready queue, granting each process a maximum execution slice known as the **Time Quantum ($q$)**.
- **Time Quantum ($q$) Tradeoff:**
  - $q \to \infty$: Degenerates into FCFS scheduling.
  - $q \to 0$: Processor sharing (infinite context switching overhead degrades effective CPU throughput).
  - **Heuristic:** Quantum $q$ is sized such that 70-80% of typical CPU bursts finish within a single quantum (commonly 10ms to 50ms).

---

# 4. Priority Inversion & The Priority Inheritance Protocol (PIP)

### The Priority Inversion Anomaly
Priority Inversion occurs when a high-priority task is indirectly blocked by a lower-priority task:

```
1. Low-priority task L acquires Mutex M.
2. High-priority task H preempts L and attempts to acquire Mutex M -> H blocks on Mutex M.
3. Medium-priority task M (not requiring Mutex M) arrives and preempts L.
4. RESULT: Medium task M runs while High task H is blocked indefinitely waiting for L.
```

### Priority Inheritance Protocol (PIP)
- When high-priority task $H$ blocks on a resource held by low-priority task $L$, $L$ temporarily **inherits $H$'s priority level**.
- $L$ executes with $H$'s elevated priority, preventing intermediate task $M$ from preempting $L$.
- Once $L$ releases the mutex, its priority reverts to its original baseline, and $H$ acquires the mutex immediately.

---

# 5. Multi-Level Feedback Queue (MLFQ) Architecture

MLFQ optimizes turnaround time for batch tasks while minimizing response time for interactive tasks without prior knowledge of burst lengths.

```
Top Priority (Queue 0):    Round Robin (q = 8ms)   ---> [New jobs enter here; on timeout -> Demote]
                                                            |
Medium Priority (Queue 1): Round Robin (q = 16ms)  ---> [Demoted jobs; on timeout -> Demote]
                                                            |
Bottom Priority (Queue 2): FCFS                    ---> [Long CPU-bound batch jobs]
```

### Formal MLFQ Rules:
1. If $\text{Priority}(A) > \text{Priority}(B)$, task $A$ runs.
2. If $\text{Priority}(A) == \text{Priority}(B)$, tasks run in Round Robin within that queue.
3. New tasks enter the highest-priority queue (Queue 0), assuming short interactive bursts.
4. If a task consumes its entire time quantum without yielding for I/O, it is **demoted** to the next lower queue.
5. If a task yields the CPU before its quantum expires (I/O burst), it retains its current priority level.
6. **Priority Boost (Anti-Starvation):** After a global time period $S$, all tasks in the system are promoted back to Queue 0.

---

# 6. Linux Completely Fair Scheduler (CFS) Internals

The Linux Completely Fair Scheduler (CFS) implements ideal proportional-share scheduling for non-real-time tasks (`SCHED_OTHER`).

```
                             Red-Black Tree (Self-Balancing BST)
                                          [ Task 2 ]
                                         (vruntime=15)
                                         /           \
               Leftmost (Next to Run)   /             \
                       [ Task 1 ]                      [ Task 3 ]
                      (vruntime=8)                    (vruntime=22)
```

### Core Architecture:
1. **Virtual Runtime (`vruntime`):** Tracks the amount of CPU execution time consumed by a task, scaled inversely by its nice value weight:
   $$\Delta \text{vruntime} = \Delta \text{real\_time} \times \frac{\text{NICE\_0\_LOAD}}{\text{task\_weight}}$$
2. **Red-Black Tree Data Structure:** Active tasks are organized in a self-balancing binary search tree keyed on `vruntime`. Finding the next task to run is $O(1)$ by reading the cached `rb_leftmost` pointer; inserting or deleting tasks requires $O(\log N)$.
3. **Nice Values $[-20, 19]$:** Lower nice values (high priority) map to larger task weights, causing `vruntime` to advance more slowly per physical clock tick. Tasks with lower `vruntime` receive CPU execution priority.

---

# 7. Deterministic Execution Traces

### Trace 1: Preemptive SRTF Execution

**Input Task Parameters:**
```
+---------+--------------+------------+
| Process | Arrival Time | Burst Time |
+---------+--------------+------------+
| P1      | 0            | 8          |
| P2      | 1            | 4          |
| P3      | 2            | 2          |
| P4      | 3            | 1          |
| P5      | 4            | 5          |
+---------+--------------+------------+
```

**Step-by-Step Schedule:**
- $t=0$: P1 runs (Remaining: 8).
- $t=1$: P2 arrives (BT=4 vs P1 rem=7). P2 preempts P1.
- $t=2$: P3 arrives (BT=2 vs P2 rem=3). P3 preempts P2.
- $t=3$: P4 arrives (BT=1 vs P3 rem=1). P3 or P4 run (P4 selected, BT=1).
- $t=4$: P4 completes. Queue: P3 (1), P2 (3), P5 (5), P1 (7). P3 runs.
- $t=5$: P3 completes. Next is P2 (3). P2 runs until $t=8$.
- $t=8$: P2 completes. Next is P5 (5). P5 runs until $t=13$.
- $t=13$: P5 completes. Next is P1 (7). P1 runs until $t=20$.

**Gantt Execution Timeline:**
`[P1: 0-1] [P2: 1-2] [P3: 2-3] [P4: 3-4] [P3: 4-5] [P2: 5-8] [P5: 8-13] [P1: 13-20]`

**Output Metrics:**
```
+---------+----+----+----+-----+----+----+
| Process | AT | BT | CT | TAT | WT | RT |
+---------+----+----+----+-----+----+----+
| P1      | 0  | 8  | 20 | 20  | 12 | 0  |
| P2      | 1  | 4  | 8  | 7   | 3  | 0  |
| P3      | 2  | 2  | 5  | 3   | 1  | 0  |
| P4      | 3  | 1  | 4  | 1   | 0  | 0  |
| P5      | 4  | 5  | 13 | 9   | 4  | 4  |
+---------+----+----+----+-----+----+----+
Average Turnaround Time = 40 / 5 = 8.0
Average Waiting Time    = 20 / 5 = 4.0
```

---

# 8. Core Theoretical Summary Principles

1. **Scheduling Optimality:** SRTF yields the minimal theoretical average waiting time by scheduling tasks in increasing remaining burst duration order.
2. **Convoy Effect:** Non-preemptive FCFS permits long compute tasks to monopolize the CPU, causing I/O devices and short tasks to stall.
3. **Priority Inversion Mitigation:** Priority Inheritance dynamically elevates a lock-holding lower-priority task's priority to prevent intermediate tasks from preempting it.
4. **CFS Proportional Share:** Tracks virtual runtime (`vruntime`) in a Red-Black Tree, scheduling the leftmost task with the least consumed scaled runtime to guarantee fair CPU sharing.
