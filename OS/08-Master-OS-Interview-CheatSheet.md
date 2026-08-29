# Master Guide 08: OS Interview Cheat Sheet & Rapid-Fire Q&A

> **Focus:** 50 High-Yield Rapid-Fire Verbal Questions & Bold Answers, Complete Unified OS Formula Card, and the Top 10 Red Flag Misconceptions to Avoid.
> 
> *The 10-Minute Pre-Interview Revision Document for FAANG/Tier-1 Tech Interviews.*

---

# Table of Contents
1. [The Unified OS Formula Card](#1-the-unified-os-formula-card)
2. [50 Rapid-Fire Interview Questions & Bold Answers](#2-50-rapid-fire-interview-questions--bold-answers)
3. [Top 10 Red Flag Mistakes That Sound Junior](#3-top-10-red-flag-mistakes-that-sound-junior)

---

# 1. The Unified OS Formula Card

```
+---------------------------------------------------------------------------------------------------+
| 1. CPU SCHEDULING FORMULAS                                                                        |
|    * Turnaround Time (TAT) = Completion Time (CT) - Arrival Time (AT)                             |
|    * Waiting Time (WT)    = Turnaround Time (TAT) - Burst Time (BT)                               |
|    * Response Time (RT)   = First Time Process gets CPU - Arrival Time (AT)                       |
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
| 4. DEADLOCK (BANKER'S ALGORITHM)                                                                  |
|    * Need Matrix Calculation: Need[i][j] = Max[i][j] - Allocation[i][j]                           |
|    * Safety Condition: Find sequence where Need_i <= Available, then Available += Allocation_i    |
|    * Safe State => NO DEADLOCK GUARANTEED                                                         |
+---------------------------------------------------------------------------------------------------+
| 5. VIRTUAL MEMORY & WORKING SET                                                                   |
|    * Thrashing Invariant: Sum of all Process Working Sets > Total Physical Memory Frames          |
|    * Belady's Anomaly Counterexample (FIFO): 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5 (3 frames=9, 4=10)  |
+---------------------------------------------------------------------------------------------------+
```

---

# 2. 50 Rapid-Fire Interview Questions & Bold Answers

### Topic 1: Processes, Threads & Context Switching
1. **What is the fundamental difference between a process and a thread?**
   - **A process is an isolated container of resources with its own private address space; a thread is the unit of CPU execution sharing memory with other threads in the same process.**
2. **What memory regions are private to each thread?**
   - **Stack, CPU Registers, Program Counter (PC), and Thread-Local Storage (TLS).**
3. **What memory regions are shared across all threads of a process?**
   - **Code (.text), Initialized Data (.data), Uninitialized Data (.bss), Heap, and open File Descriptors.**
4. **Why is a process context switch heavier than a thread context switch?**
   - **Because a process context switch requires changing the page table pointer (CR3) and invalidating/tagging TLB entries, causing severe L1/L2 cache misses.**
5. **How does the CPU switch between User Mode and Kernel Mode?**
   - **Via hardware traps, interrupts, or `syscall` instructions, transitioning the CPU from Ring 3 to Ring 0 and swapping to the kernel stack.**
6. **What is a Zombie Process and how do you fix it?**
   - **A terminated child whose exit status has not been read by its parent via `wait()`; fixed by having the parent call `wait()` or killing the parent so `init` (PID 1) reaps it.**
7. **What is an Orphan Process?**
   - **An executing process whose parent died; it is automatically adopted and reaped by PID 1 (`init` / `systemd`).**
8. **What does `fork()` return?**
   - **Returns `0` to the child process, the child's `PID` to the parent process, and `-1` on failure.**
9. **How does Copy-on-Write (COW) optimize `fork()`?**
   - **`fork()` copies only page table references marked read-only; physical pages are duplicated only when a write operation triggers a page fault.**
10. **What is the difference between User-Level Threads (M:1) and Kernel-Level Threads (1:1)?**
    - **User-level threads switch in user-space without syscalls but cannot run across multiple CPU cores; kernel-level threads allow true multi-core parallel execution.**

---

### Topic 2: CPU Scheduling
11. **Which CPU scheduling algorithm gives the minimum average waiting time?**
    - **Shortest Job First (SJF) / Shortest Remaining Time First (SRTF).**
12. **Why isn't SJF used in general-purpose desktop operating systems?**
    - **Because the exact future CPU burst time of arbitrary user programs cannot be known in advance.**
13. **What is the Convoy Effect?**
    - **A performance bottleneck in FCFS where short I/O-bound processes are blocked waiting behind a massive CPU-bound process.**
14. **How do you choose the Time Quantum $q$ in Round Robin?**
    - **Size $q$ such that $80\%$ of CPU bursts are shorter than $q$ (typically 10-50ms) to balance response time against context-switch overhead.**
15. **What happens if Round Robin's quantum $q$ is extremely large?**
    - **It degenerates into First-Come, First-Served (FCFS).**
16. **What happens if Round Robin's quantum $q$ is extremely small?**
    - **Excessive context-switching overhead stalls the CPU, reducing net system throughput toward zero.**
17. **What is Starvation and how is it solved?**
    - **When low-priority jobs never run due to a steady stream of high-priority jobs; solved by Aging (gradually increasing waiting jobs' priority).**
18. **How does the Linux Completely Fair Scheduler (CFS) select the next task to run?**
    - **It picks the task with the smallest `vruntime` in $O(1)$ time from the leftmost node of a Red-Black Tree.**
19. **What does a negative `nice` value mean in Linux?**
    - **Higher priority (nice values range from -20 highest to +19 lowest).**

---

### Topic 3: Deadlocks & Banker's Algorithm
20. **What are the 4 Coffman conditions for deadlock?**
    - **Mutual Exclusion, Hold and Wait, No Preemption, and Circular Wait (Acronym: MHNC).**
21. **Does a cycle in a Resource Allocation Graph always mean deadlock?**
    - **Only for single-instance resources. For multi-instance resources, a cycle is a necessary condition, but not sufficient.**
22. **What is the most practical Deadlock Prevention technique?**
    - **Enforcing a strict Global Linear Resource Ordering to eliminate Circular Wait.**
23. **What is the difference between Deadlock Prevention and Deadlock Avoidance?**
    - **Prevention constrains requests statically to make deadlocks impossible; Avoidance dynamically checks for a Safe State before granting requests.**
24. **What is a Safe State in Banker's Algorithm?**
    - **A state where at least one execution sequence exists that allows all processes to finish without deadlock.**
25. **What is the difference between Deadlock and Livelock?**
    - **Deadlocked processes are asleep/blocked waiting for resources; livelocked processes are actively running/spinning, changing states with zero forward progress.**

---

### Topic 4: Process Synchronization & Concurrency
26. **What are the 3 requirements for solving the Critical Section problem?**
    - **Mutual Exclusion, Progress (no deadlock on entry), and Bounded Waiting (no starvation).**
27. **What is the difference between a Mutex and a Binary Semaphore?**
    - **A Mutex has ownership (only the locking thread can unlock); a Semaphore has no ownership and is used for event signaling.**
28. **When should you use a Spinlock instead of a Mutex?**
    - **On multi-core systems when the critical section is extremely short ($< 1\text{--}2\mu\text{s}$), avoiding the cost of a full thread sleep/wake context switch.**
29. **Why must Condition Variable wait be wrapped in a `while` loop instead of `if`?**
    - **To guard against Spurious Wakeups and race conditions where another thread claims the condition before the waking thread acquires the lock (Mesa semantics).**
30. **What is Priority Inversion and how is it fixed?**
    - **When a medium-priority task preempts a low-priority task holding a lock needed by a high-priority task; fixed via Priority Inheritance Protocol (PIP).**
31. **What is the Producer-Consumer problem's synchronization setup?**
    - **Two counting semaphores (`emptySlots = N`, `fullSlots = 0`) and one mutex for buffer access.**
32. **What is a Race Condition?**
    - **A concurrent bug where the final state of shared data depends on the unpredictable execution order of multiple threads.**
33. **What is a Futex in Linux?**
    - **Fast User-space Mutex: acquires locks in user-space via atomic CAS, trapping into the kernel only when lock contention occurs.**

---

### Topic 5: Memory Management & Paging
34. **What is the difference between Internal and External Fragmentation?**
    - **Internal is wasted space inside a fixed allocated block (Paging); External is scattered free space between blocks that cannot fit a contiguous request (Segmentation).**
35. **What is the role of the MMU?**
    - **Hardware chip that dynamically translates virtual addresses from the CPU into physical RAM addresses using page tables.**
36. **What is the Translation Lookaside Buffer (TLB)?**
    - **An on-chip associative hardware cache inside the MMU that stores recent virtual-to-physical address translations.**
37. **Why do 64-bit operating systems use Multi-Level Paging?**
    - **To save memory by creating a sparse tree structure where page tables are allocated only for active virtual address ranges.**
38. **What does the Dirty bit in a Page Table Entry mean?**
    - **It indicates the page was modified in RAM and must be written back to disk before eviction.**
39. **What happens during a TLB Miss?**
    - **The MMU (or OS) walks the multi-level page table in RAM to find the physical frame, loads it into the TLB, and restarts the translation.**
40. **What are Huge Pages and why are they used?**
    - **Pages sized 2 MB or 1 GB that reduce page table walks and maximize TLB coverage for large-memory applications like databases.**

---

### Topic 6: Virtual Memory & Page Replacement
41. **What is Demand Paging?**
    - **A virtual memory scheme where pages are loaded from disk into RAM only when referenced.**
42. **What is a Page Fault?**
    - **A hardware trap triggered when the CPU accesses a virtual page whose Present bit in the PTE is 0.**
43. **What is Belady's Anomaly?**
    - **The counter-intuitive phenomenon where allocating more physical frames causes more page faults (occurs in FIFO, not in LRU).**
44. **Why is LRU immune to Belady's Anomaly?**
    - **Because LRU is a Stack Algorithm: the set of pages resident in $N$ frames is always a strict subset of pages in $N+1$ frames.**
45. **How does the Clock (Second Chance) Page Replacement algorithm work?**
    - **A circular pointer inspects reference bits; if 1, clears to 0 and advances; if 0, evicts that frame immediately.**
46. **What is Thrashing?**
    - **A state where the system spends more time swapping pages in/out of disk than executing instructions, dropping CPU utilization to near zero.**
47. **What is the Working Set Model?**
    - **Peter Denning's model tracking the set of pages referenced in the last $\Delta$ time units to ensure a process is allocated enough frames to prevent thrashing.**

---

### Topic 7: Inter-Process Communication (IPC)
48. **What is the fastest IPC mechanism on a single Linux machine?**
    - **Shared Memory (`shm_open` + `mmap`), because reads and writes bypass the kernel entirely.**
49. **Why are UNIX Domain Sockets faster than TCP sockets on localhost?**
    - **Because they bypass the entire TCP/IP network stack, packet framing, and checksum calculations.**
50. **What is Zero-Copy I/O and which syscall enables it?**
    - **Direct data transfer from page cache to network card via DMA without copying to user-space, enabled by `sendfile()`.**

---

# 3. Top 10 Red Flag Mistakes That Sound Junior

```
+---------------------------------------------------------------------------------------------------+
| #  | THE JUNIOR MISTAKE                     | THE SENIOR CORRECTION / INTERVIEW PIVOT             |
+---------------------------------------------------------------------------------------------------+
| 1  | "Threads don't have their own stack."  | Threads ALWAYS have their own stack to execute      |
|    |                                        | independent function calls.                         |
+---------------------------------------------------------------------------------------------------+
| 2  | "You can kill a Zombie with kill -9."  | Zombies are ALREADY dead. You must make the parent  |
|    |                                        | call wait() or kill the parent so init reaps it.    |
+---------------------------------------------------------------------------------------------------+
| 3  | "SJF is used in standard desktop OS."  | SJF requires knowing future burst lengths; real OSs |
|    |                                        | use Round Robin, MLFQ, or CFS.                      |
+---------------------------------------------------------------------------------------------------+
| 4  | "A cycle in a RAG always means         | A cycle indicates deadlock ONLY for single-instance |
|    | deadlock."                             | resources; for multi-instance it's not sufficient.  |
+---------------------------------------------------------------------------------------------------+
| 5  | "A Mutex and a Binary Semaphore are    | A Mutex enforces strict OWNERSHIP (lock/unlock by   |
|    | identical."                            | same thread); a Semaphore is for signaling.         |
+---------------------------------------------------------------------------------------------------+
| 6  | "Use an `if` statement to check a      | ALWAYS use a `while` loop to guard against spurious |
|    | Condition Variable."                   | wakeups and race conditions in Mesa monitors.       |
+---------------------------------------------------------------------------------------------------+
| 7  | "Paging causes external fragmentation."| Paging ELIMINATES external fragmentation; it suffers|
|    |                                        | only from internal fragmentation.                   |
+---------------------------------------------------------------------------------------------------+
| 8  | "More RAM frames always reduces page   | FIFO can suffer from Belady's Anomaly where more    |
|    | faults."                               | frames cause MORE page faults.                      |
+---------------------------------------------------------------------------------------------------+
| 9  | "Spinlocks are always faster than      | Spinlocks are catastrophic on single-core CPUs and  |
|    | Mutexes."                              | waste 100% CPU on long critical sections.           |
+---------------------------------------------------------------------------------------------------+
| 10 | "Shared memory is thread-safe."        | Shared memory provides ZERO built-in synchronization|
|    |                                        | and requires explicit semaphores or mutexes.        |
+---------------------------------------------------------------------------------------------------+
```
