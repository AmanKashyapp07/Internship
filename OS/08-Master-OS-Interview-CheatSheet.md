# Core Operating Systems Theorems, Formulas & Architecture Reference

> **Scope:** Unified OS Architecture Matrix, Mathematical Formulas & Relational Bounds, 50 Foundational Operating Systems Principles, and Kernel Subsystem Failure Modes & Pitfalls.

---

# Table of Contents
1. [Unified OS Architecture & Subsystems Matrix](#1-unified-os-architecture--subsystems-matrix)
2. [Mathematical Formulas & Theoretical Quick-Reference](#2-mathematical-formulas--theoretical-quick-reference)
3. [50 Foundational Operating Systems Principles](#3-50-foundational-operating-systems-principles)
4. [Critical Kernel & Concurrency Failure Modes](#4-critical-kernel--concurrency-failure-modes)

---

# 1. Unified OS Architecture & Subsystems Matrix

```
+--------------------------------------------------------------------------------------------------------------------+
| OS SUBSYSTEM         | CORE KERNEL DATA STRUCTURE       | ARCHITECTURAL RESPONSIBILITY & ENGINE MECHANISM   |
+--------------------------------------------------------------------------------------------------------------------+
| Process Management   | `task_struct`, Process Table     | Address space isolation, context switching, signals|
+--------------------------------------------------------------------------------------------------------------------+
| CPU Scheduling       | Red-Black Tree (`cfs_rq`), Queues| Proportional CPU fairness, vruntime tracking, PIP  |
+--------------------------------------------------------------------------------------------------------------------+
| Concurrency Control  | Mutexes, Semaphores, `futex`     | Mutual exclusion, atomics (CAS), monitor condition |
+--------------------------------------------------------------------------------------------------------------------+
| Deadlock Subsystem   | Wait-For Graph, Allocation Matrix| Coffman condition breaking, Banker's safe sequence |
+--------------------------------------------------------------------------------------------------------------------+
| Virtual Memory       | Multi-Level Page Tables, PTEs    | MMU translation, demand paging, COW page faults    |
+--------------------------------------------------------------------------------------------------------------------+
| Page Replacement     | Clock Circular Buffer, LRU Lists | Working set maintenance, thrashing avoidance       |
+--------------------------------------------------------------------------------------------------------------------+
| Inter-Process Comm.  | Ring Buffers, Shared Memory Maps | Zero-copy data streaming, DMA engine transfers     |
+--------------------------------------------------------------------------------------------------------------------+
```

---

# 2. Mathematical Formulas & Theoretical Quick-Reference

```
+---------------------------------------------------------------------------------------------------+
| 1. CPU SCHEDULING FORMULAS                                                                        |
|    * Turnaround Time (TAT) = Completion Time (CT) - Arrival Time (AT)                             |
|    * Waiting Time (WT)    = Turnaround Time (TAT) - Burst Time (BT)                               |
|    * Response Time (RT)   = First Execution Time - Arrival Time (AT)                              |
|    * Exponential Burst Prediction: tau_{n+1} = alpha * t_n + (1 - alpha) * tau_n                  |
+---------------------------------------------------------------------------------------------------+
| 2. PAGING & ADDRESS TRANSLATION SIZING                                                            |
|    * Page Offset Bits: d = log2(Page Size in Bytes)                                               |
|    * Number of Virtual Pages = 2^(Virtual Address Bits - d)                                       |
|    * Single-Level Page Table Size = (Number of Virtual Pages) * (PTE Size in Bytes)               |
|    * Number of Physical Frames = (Physical RAM Size) / (Page Size)                                |
+---------------------------------------------------------------------------------------------------+
| 3. EFFECTIVE MEMORY ACCESS TIME (EMAT) WITH TLB                                                   |
|    * 1-Level Paging: EMAT = h * (t_tlb + t_mem) + (1 - h) * (t_tlb + 2 * t_mem)                   |
|    * k-Level Paging: EMAT = h * (t_tlb + t_mem) + (1 - h) * (t_tlb + (k + 1) * t_mem)             |
+---------------------------------------------------------------------------------------------------+
| 4. DEADLOCK AVOIDANCE (BANKER'S ALGORITHM)                                                         |
|    * Need Matrix Equation: Need[i][j] = Max[i][j] - Allocation[i][j]                              |
|    * Safety Invariant: Need_i <= Available -> Available += Allocation_i -> Safe Sequence Exists   |
+---------------------------------------------------------------------------------------------------+
| 5. VIRTUAL MEMORY & WORKING SET DYNAMICS                                                          |
|    * Thrashing Invariant: Sum of all Active Working Sets > Total Physical Frame Allocation        |
|    * Stack Algorithm Inclusion Property: S(N, t) subset of S(N + 1, t) -> Immune to Belady's     |
+---------------------------------------------------------------------------------------------------+
```

---

# 3. 50 Foundational Operating Systems Principles

### Category A: Processes, Threads & Context Switching
1. **Process vs. Thread Distinction:** A process is an isolated resource allocation boundary with private virtual memory; a thread is an independent stream of CPU execution sharing heap and text.
2. **Private Thread Regions:** Each thread maintains its own Program Counter (PC), CPU registers, Stack, and Thread-Local Storage (TLS).
3. **Shared Thread Regions:** All threads within a process share the Code (`.text`), Initialized Data (`.data`), Uninitialized Data (`.bss`), Heap, and Open File Descriptors.
4. **Context Switch Performance Cost:** Direct cost involves register save/restore; dominant indirect cost stems from TLB invalidation and L1/L2 cache cold misses.
5. **Mode Transition:** Transitioning from User Mode (Ring 3) to Kernel Mode (Ring 0) occurs via hardware traps, interrupts, or `syscall` instructions.
6. **Zombie Process:** A terminated child process whose exit code has not been collected by its parent via `wait()`, holding a slot in the process table.
7. **Orphan Process:** A running process whose parent has terminated; adopted and reaped by `init` (PID 1 / `systemd`).
8. **`fork()` Return Values:** Returns `0` to the child process, the child's `PID` to the parent, and `-1` on allocation failure.
9. **Copy-on-Write (COW):** Defers physical frame copying on `fork()`, marking pages read-only until a write triggers a page fault.
10. **Thread Models (1:1 vs. M:1):** 1:1 maps user threads directly to kernel schedulable entities for multi-core parallelism; M:1 executes in user space without multi-core capability.

---

### Category B: CPU Scheduling & Dispatch
11. **Average Waiting Time Minimization:** SRTF mathematically minimizes average waiting time by executing shortest remaining bursts first.
12. **SJF Practical Infeasibility:** Requires future knowledge of arbitrary user burst lengths, which is unknown in general-purpose computing.
13. **Convoy Effect:** Occurs under non-preemptive FCFS when short I/O tasks are blocked behind long CPU-bound tasks.
14. **Time Quantum Sizing:** In Round Robin, $q$ should be sized so 70-80% of bursts complete within one slice (10-50ms) to minimize context switch overhead.
15. **Quantum Degradation:** As $q \to \infty$, Round Robin degenerates into FCFS; as $q \to 0$, context switch costs dominate the CPU.
16. **Starvation & Aging:** Starvation occurs when low-priority jobs are delayed indefinitely; Aging solves this by monotonically increasing priority over wait duration.
17. **Linux CFS Selection:** Dispatches the task with the smallest `vruntime` in $O(1)$ time from the leftmost node of a Red-Black Tree.
18. **Nice Value Mechanics:** Nice values $[-20, 19]$ scale `vruntime` accumulation inversely relative to task priority.

---

### Category C: Deadlocks & Banker's Algorithm
19. **The 4 Coffman Conditions:** Mutual Exclusion, Hold and Wait, No Preemption, and Circular Wait must all hold simultaneously for deadlock.
20. **RAG Cycle Interpretation:** A cycle in a Resource Allocation Graph proves deadlock only for single-instance resource systems.
21. **Deadlock Prevention via Ordering:** Imposing a strict global total order $F(R)$ on resources mathematically eliminates circular wait.
22. **Prevention vs. Avoidance:** Prevention restricts resource acquisition rules statically; Avoidance dynamically checks for safe states prior to granting requests.
23. **Safe State Definition:** A state is safe if there exists at least one execution ordering allowing all processes to complete without deadlock.
24. **Livelock vs. Deadlock:** Deadlocked processes are sleeping/blocked; livelocked processes are actively executing, altering state with zero progress.

---

### Category D: Process Synchronization & Concurrency
25. **Critical Section Requirements:** Mutual Exclusion, Progress, and Bounded Waiting must be satisfied concurrently.
26. **Mutex vs. Semaphore:** Mutexes enforce strict thread ownership (lock and unlock by the same thread); Semaphores operate as arbitrary signaling counters.
27. **Spinlock Deployment:** Optimal on multi-core systems when critical sections are shorter than context switch latencies ($< 1\text{--}2\mu\text{s}$).
28. **Condition Variable While Loop:** Re-evaluating condition predicates inside `while` loops protects against spurious wakeups and Mesa scheduling semantics.
29. **Priority Inversion & PIP:** Priority Inheritance temporarily elevates the lock-holder's priority to match that of the highest blocked waiting task.
30. **Producer-Consumer Setup:** Implemented via two counting semaphores (`empty = N`, `full = 0`) and one mutual exclusion lock.
31. **Race Conditions:** Occur when unsynchronized concurrent writes yield non-deterministic final state depending on thread interleaving.
32. **Linux Futex:** Fast User-space Mutex optimizes uncontended locks using user-space atomic CAS, sleeping in kernel space only on contention.

---

### Category E: Memory Management & Paging
33. **Internal vs. External Fragmentation:** Internal is unutilized space inside a fixed page frame; External is scattered unallocated memory unable to satisfy contiguous requests.
34. **Role of the MMU:** Hardware processor translating CPU virtual addresses to physical RAM addresses via page tables.
35. **Translation Lookaside Buffer (TLB):** Fully-associative hardware cache storing active virtual-to-physical address translations.
36. **Multi-Level Paging:** Hierarchical tree structure saving physical memory by instantiating page tables only for allocated virtual regions.
37. **Dirty Bit Invariant:** Indicates a memory page was modified and must be synchronized with backing storage before eviction.
38. **TLB Miss Handling:** Hardware or OS walks the multi-level page table in RAM, populates the TLB, and resumes translation.
39. **Huge Pages:** Sized at 2 MB or 1 GB to reduce TLB miss rates and page table memory consumption for large-memory workloads.

---

### Category F: Virtual Memory & Page Replacement
40. **Demand Paging:** Loads virtual pages from backing disk into physical RAM exclusively upon initial memory access.
41. **Page Fault Exception:** Hardware trap generated by the MMU when accessing a page entry whose Present bit is `0`.
42. **Belady's Anomaly:** The phenomenon where increasing physical frame allocation increases total page faults under FIFO replacement.
43. **LRU Anomaly Immunity:** Stack algorithms are immune to Belady's Anomaly because page sets for $N$ frames are strictly subsets of $N+1$ frames.
44. **Clock Replacement Algorithm:** Cycles a hand pointer across frames, clearing reference bits from 1 to 0 and evicting the first page with bit 0.
45. **Thrashing Invariant:** System spends more cycles servicing page faults than executing instructions, dropping CPU utilization to near zero.
46. **Working Set Model:** Defines memory demand as the distinct pages referenced within sliding window $\Delta$, ensuring $\sum |W_i| \le \text{Frames}$.

---

### Category G: Inter-Process Communication (IPC)
47. **Shared Memory Throughput:** Fastest single-host IPC mechanism, operating at direct hardware RAM bus speeds via `mmap`.
48. **UNIX Domain Sockets:** Provides high-speed single-host IPC by bypassing network protocol encapsulation and checksum overhead.
49. **File Descriptor Transfer:** `AF_UNIX` sockets pass open file descriptors across process boundaries via `sendmsg()` with `SCM_RIGHTS`.
50. **Zero-Copy I/O:** `sendfile()` streams data directly from kernel page cache to network interface buffers via DMA, bypassing user memory.

---

# 4. Critical Kernel & Concurrency Failure Modes

```
+---------------------------------------------------------------------------------------------------+
| #  | KERNEL SUBSYSTEM PITFALL               | ROOT CAUSE & THEORETICAL MITIGATION                 |
+---------------------------------------------------------------------------------------------------+
| 1  | Unreaped Zombie Accumulation           | Parent fails to call wait(); causes PID exhaustion. |
+---------------------------------------------------------------------------------------------------+
| 2  | Non-Reentrant Signal Handlers          | Calling malloc()/printf() inside handlers deadlocks.|
+---------------------------------------------------------------------------------------------------+
| 3  | Spinlocks on Uniprocessors             | Starves lock-holding thread; use sleeping mutexes.  |
+---------------------------------------------------------------------------------------------------+
| 4  | Priority Inversion Watchdog Resets     | Medium task preempts low task; apply PIP protocol.  |
+---------------------------------------------------------------------------------------------------+
| 5  | Condition Variable `if` Evaluations    | Spurious wakeups violate invariant; wrap in while.  |
+---------------------------------------------------------------------------------------------------+
| 6  | Thrashing under Over-Subscription      | Working sets exceed RAM; suspend tasks via PFF.     |
+---------------------------------------------------------------------------------------------------+
| 7  | Shared Memory Race Conditions          | Shared memory has zero sync; apply POSIX semaphores.|
+---------------------------------------------------------------------------------------------------+
```
