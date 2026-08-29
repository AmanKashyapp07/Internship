# Master Guide 02: CPU Scheduling Algorithms

> **Focus:** Scheduling Decision Matrix, Core Metrics (TAT, WT, RT), FCFS & Convoy Effect, SJF vs SRTF, Round Robin Quantum Sizing, Priority Inversion & Aging, MLFQ Mechanics, Linux CFS (vruntime & Red-Black Tree), and Rapid-Fire Gantt Drills.
> 
> *Targeted for Top-Tier Tech System & Backend Engineering Interviews.*

---

# Table of Contents
1. [The Master Algorithm Selector Matrix](#1-the-master-algorithm-selector-matrix)
2. [Scheduling Metrics & Core Formulas](#2-scheduling-metrics--core-formulas)
3. [Classic Scheduling Algorithms Breakdown](#3-classic-scheduling-algorithms-breakdown)
4. [Linux Completely Fair Scheduler (CFS) Internals](#4-linux-completely-fair-scheduler-cfs-internals)
5. [Fast-Track Numerical Drills (Gantt Chart Mastery)](#5-fast-track-numerical-drills-gantt-chart-mastery)
6. [High-Frequency Interview Drill & Verbal Q&A](#6-high-frequency-interview-drill--verbal-qa)

---

# 1. The Master Algorithm Selector Matrix

```
+---------------------------------------------------------------------------------------------------+
| INTERVIEWER OBJECTIVE / CONSTRAINT   | OPTIMAL ALGORITHM        | TRADE-OFF / DRAWBACK            |
+---------------------------------------------------------------------------------------------------+
| Minimize Average Waiting Time        | SJF / SRTF (Preemptive)  | Causes starvation for long jobs |
| Interactive / Time-Sharing Fairness  | Round Robin (q = 10-50ms)| Context-switch overhead if q tiny|
| Simple, Zero Scheduling Overhead     | FCFS (Non-Preemptive)    | Convoy Effect (high avg WT)     |
| Strict Deadlines & Urgency           | Priority (with Aging)    | Starvation without aging        |
| General Purpose Multi-Workload OS    | MLFQ / Linux CFS         | Complex implementation tuning   |
| Hard Real-Time Embedded Systems      | Rate Monotonic / EDF     | Requires static predictability  |
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** CPU scheduling is picking who gets the microphone next to balance throughput, fairness, and response latency.
- **The Interview Trap:** Saying SJF is used in general-purpose desktop operating systems. SJF requires knowing the future CPU burst length in advance, which is impossible for arbitrary user programs; real systems estimate bursts or use Round Robin / MLFQ.
- **30-Second Verbal Answer:** **"CPU scheduling allocates CPU cores across ready processes. For batch systems, Shortest Remaining Time First (SRTF) is provably optimal for minimizing average waiting time. For interactive multi-user systems, Round Robin or Multi-Level Feedback Queues provide fair response times without requiring prior knowledge of burst lengths."**

---

# 2. Scheduling Metrics & Core Formulas

```
+---------------------------------------------------------------------------------------------------+
| METRIC                    | FORMULA                                   | MEANING                   |
+---------------------------------------------------------------------------------------------------+
| Arrival Time (AT)         | Given in problem                          | Timestamp process enters RQ|
| Burst Time (BT)           | Given in problem                          | Total CPU execution needed |
| Completion Time (CT)      | Deduced from Gantt chart                  | Timestamp process finishes |
| Turnaround Time (TAT)     | TAT = Completion Time (CT) - Arrival Time (AT) | Total time in system |
| Waiting Time (WT)         | WT = Turnaround Time (TAT) - Burst Time (BT) | Total time idling in RQ  |
| Response Time (RT)        | RT = First CPU Allocation - Arrival Time (AT)| Time until first progress|
+---------------------------------------------------------------------------------------------------+
```

### Invariant Check Rule:
- In non-preemptive systems with no idle waiting during execution: $\text{Response Time (RT)} = \text{Waiting Time (WT)}$.
- In preemptive systems (SRTF, Round Robin): $\text{Response Time (RT)} \le \text{Waiting Time (WT)}$.

---

# 3. Classic Scheduling Algorithms Breakdown

### A. First-Come, First-Served (FCFS)
- **Mechanics:** Non-preemptive FIFO queue based on arrival order.
- **The Convoy Effect:** A single massive CPU-bound process blocks dozens of tiny I/O-bound processes, tanking device utilization and sky-rocketing average waiting time.
- **Follow-up:** *How to mitigate Convoy Effect?* **Use preemption or separate I/O and CPU queues.**

### B. Shortest Job First (SJF) & Shortest Remaining Time First (SRTF)
- **Mechanics:** Pick process with shortest CPU burst. Non-preemptive = SJF; Preemptive = SRTF.
- **Optimality:** Provably achieves the **minimum possible average waiting time**.
- **Burst Prediction Formula (Exponential Moving Average):**
  $$\tau_{n+1} = \alpha t_n + (1 - \alpha) \tau_n \quad (\text{where } 0 \le \alpha \le 1)$$
  - $\alpha = 0$: History ignored, constant prediction.
  - $\alpha = 1$: Only the most recent burst matters.

### C. Round Robin (RR)
- **Mechanics:** Preemptive time-sliced circular queue. Each process gets a maximum Time Quantum $q$.
- **Quantum Sizing Goldilocks Rule:**
  - $q \to \infty$: Degenerates into **FCFS**.
  - $q \to 0$: Processor sharing (infinite context-switching overhead stalls the CPU).
  - **Rule of Thumb:** $q$ should be sized such that **$80\%$ of CPU bursts are shorter than $q$** (typically $10\text{--}50\text{ms}$).

### D. Priority Scheduling & Priority Inversion
- **Starvation Solution:** **Aging** — gradually increment priority of waiting processes over time.
- **Priority Inversion:** High-priority task $H$ is blocked on a mutex held by Low-priority task $L$. Medium-priority task $M$ preempts $L$, indirectly starving $H$.
- **Solution:** **Priority Inheritance Protocol (PIP)** — temporarily elevate $L$'s priority to $H$'s priority until $L$ releases the mutex.

### E. Multi-Level Feedback Queue (MLFQ)
```
Top Priority (Queue 0):    Round Robin (q = 8ms)   ---> [New jobs start here; if time expires -> Demote]
                                                            |
Medium Priority (Queue 1): Round Robin (q = 16ms)  ---> [Demoted jobs; if time expires -> Demote]
                                                            |
Bottom Priority (Queue 2): FCFS                    ---> [Long CPU-bound batch jobs]
```
- **Rule 1:** If $\text{Priority}(A) > \text{Priority}(B)$, process $A$ runs.
- **Rule 2:** If $\text{Priority}(A) == \text{Priority}(B)$, run Round Robin within the queue.
- **Rule 3:** New processes enter top queue (assumed short I/O-bound).
- **Rule 4:** If a process consumes its entire time slice without yielding for I/O, it is **demoted** down one queue.
- **Rule 5 (Anti-Starvation):** Periodic Priority Boost — every $S$ seconds, move all processes to the top queue.

---

# 4. Linux Completely Fair Scheduler (CFS) Internals

```
                             Red-Black Tree (Self-Balancing BST)
                                          [ Task 2 ]
                                         (vruntime=15)
                                         /           \
               Leftmost (Next to Run)   /             \
                       [ Task 1 ]                      [ Task 3 ]
                      (vruntime=8)                    (vruntime=22)
```

- **One-Line Intuition:** CFS simulates an ideal multi-tasking CPU by giving every task an equal slice of time, tracking deficit using virtual runtime (`vruntime`).
- **Core Mechanics in 3 Points:**
  1. **`vruntime` Tracking:** Represents actual CPU time scaled by task nice weight. Task with the smallest `vruntime` has received the least CPU time.
  2. **Data Structure:** Uses a **Red-Black Tree** ordered by `vruntime`. Finding the next process to run takes $O(1)$ by caching `rb_leftmost`; insertion/deletion takes $O(\log N)$.
  3. **Nice Values $[-20, 19]$:** Higher nice value ($+19$) = lower priority = `vruntime` increases faster per real clock tick; lower nice value ($-20$) = higher priority = `vruntime` increases slower.
- **Interview Trap:** Claiming Linux uses Round Robin for standard processes. Linux uses CFS for normal tasks (`SCHED_OTHER`) and standard FIFO/RR only for real-time priority tasks (`SCHED_FIFO` / `SCHED_RR`).

---

# 5. Fast-Track Numerical Drills (Gantt Chart Mastery)

### Drill 1: Preemptive SRTF vs. Non-Preemptive SJF

**Given Process Table:**
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

#### Preemptive SRTF Execution Trace:
- $t=0$: P1 runs (remaining: 8).
- $t=1$: P2 arrives (BT=4 vs P1 remaining=7). P2 preempts P1.
- $t=2$: P3 arrives (BT=2 vs P2 remaining=3). P3 preempts P2.
- $t=3$: P4 arrives (BT=1 vs P3 remaining=1). Tie goes to current P3 or P4. Run P4 (BT=1).
- $t=4$: P4 completes. Ready queue: P3 (1), P2 (3), P5 (5), P1 (7). P3 runs.
- $t=5$: P3 completes. Next shortest is P2 (3). P2 runs until $t=8$.
- $t=8$: P2 completes. Next shortest is P5 (5). P5 runs until $t=13$.
- $t=13$: P5 completes. Next is P1 (7). P1 runs until $t=20$.

**Gantt Chart:**
`[P1: 0-1] [P2: 1-2] [P3: 2-3] [P4: 3-4] [P3: 4-5] [P2: 5-8] [P5: 8-13] [P1: 13-20]`

**Result Table:**
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
Total TAT = 40, Avg TAT = 8.0
Total WT  = 20, Avg WT  = 4.0
```

---

### Drill 2: Round Robin ($q = 2$)

**Given Process Table:**
```
+---------+--------------+------------+
| Process | Arrival Time | Burst Time |
+---------+--------------+------------+
| P1      | 0            | 5          |
| P2      | 1            | 3          |
| P3      | 2            | 1          |
| P4      | 3            | 2          |
+---------+--------------+------------+
```

**Ready Queue Trace ($q = 2$):**
- $t=0$: P1 enters. Runs $[0\text{--}2]$ (rem: 3). At $t=1$ P2 arrives, at $t=2$ P3 arrives. RQ: `P2, P3, P1`.
- $t=2$: P2 runs $[2\text{--}4]$ (rem: 1). At $t=3$ P4 arrives. RQ: `P3, P1, P4, P2`.
- $t=4$: P3 runs $[4\text{--}5]$ (rem: 0). P3 finishes at $t=5$. RQ: `P1, P4, P2`.
- $t=5$: P1 runs $[5\text{--}7]$ (rem: 1). RQ: `P4, P2, P1`.
- $t=7$: P4 runs $[7\text{--}9]$ (rem: 0). P4 finishes at $t=9$. RQ: `P2, P1`.
- $t=9$: P2 runs $[9\text{--}10]$ (rem: 0). P2 finishes at $t=10$. RQ: `P1`.
- $t=10$: P1 runs $[10\text{--}11]$ (rem: 0). P1 finishes at $t=11$.

**Result Table:**
```
+---------+----+----+----+-----+----+----+
| Process | AT | BT | CT | TAT | WT | RT |
+---------+----+----+----+-----+----+----+
| P1      | 0  | 5  | 11 | 11  | 6  | 0  |
| P2      | 1  | 3  | 10 | 9   | 6  | 1  |
| P3      | 2  | 1  | 5  | 3   | 2  | 2  |
| P4      | 3  | 2  | 9  | 6   | 4  | 4  |
+---------+----+----+----+-----+----+----+
Total TAT = 29, Avg TAT = 7.25
Total WT  = 18, Avg WT  = 4.50
```

---

# 6. High-Frequency Interview Drill & Verbal Q&A

### Q1: Why is Round Robin better than FCFS for interactive workloads?
> **Answer:** Because Round Robin guarantees a **bounded maximum response time of $(N - 1) \times q$**, ensuring short requests get quick CPU slices without waiting behind long-running batch processes.

### Q2: Can Priority Scheduling cause deadlock?
> **Answer:** **Yes, via Priority Inversion.** If a low-priority thread holds a lock needed by a high-priority thread, and an intermediate-priority thread runs indefinitely, the high-priority thread is indefinitely starved/blocked.

### Q3: Why does SJF provide minimal average waiting time?
> **Answer:** Mathematically, short jobs contribute to the waiting times of fewer total processes. Giving the CPU to the shortest job completes it earliest, reducing the number of waiting tasks remaining in the queue as fast as possible.

### Q4: What is the relationship between CPU utilization and throughput?
> **Answer:** **CPU utilization** is the percentage of time the processor is executing non-idle user/kernel code. **Throughput** is the count of completed processes per unit time. High CPU utilization does not guarantee high throughput if processes spend excessive time on context switches or thrashing.

### Q5: How does MLFQ prevent gaming the scheduler?
> **Answer:** In early MLFQ designs, a process could run for $99\%$ of its quantum, issue a dummy I/O, keep its high priority, and hog the CPU. Modern MLFQ accounts for **cumulative CPU time** spent at a given level; once the threshold is exceeded across multiple bursts, the process is demoted regardless of I/O frequency.
