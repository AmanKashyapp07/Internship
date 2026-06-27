# OS Master Cheatsheet — Internship Interview Ready

> This is a rapid-review reference covering every critical OS concept for top-tier internship interviews. Use this for last-minute revision.

## 📚 Full Notes Index
| File | Topics |
|------|--------|
| [l1.md](l1.md) | OS types, kernel types, system calls, boot process, 32/64-bit |
| [l2.md](l2.md) | Memory layout, PCB, state diagram, context switching, zombie/orphan, IPC |
| [l3.md](l3.md) | FCFS, SJF, SRTF, Priority, Round Robin, MLQ, MLFQ, Gantt charts |
| [l4.md](l4.md) | Race conditions, mutex, semaphore, CV, classic problems |
| [l5.md](l5.md) | Coffman conditions, prevention/avoidance/detection, Banker's Algorithm |
| [l6.md](l6.md) | Paging, TLB, fragmentation, multi-level paging, segmentation |
| [l7.md](l7.md) | Demand paging, page fault, FIFO/OPT/LRU, Belady's, thrashing |
| [l8.md](l8.md) | Inodes, file allocation, journaling, DMA, disk scheduling algorithms |

---

## ⚡ Quick Comparison Tables

### Process vs. Thread
| | **Process** | **Thread** |
|-|-------------|------------|
| Memory | Own address space | Shared (heap, data, text) |
| Context Switch | Expensive (TLB flush, cache cold) | Cheap (no TLB flush) |
| Communication | IPC (pipes, sockets, shared mem) | Direct shared variables |
| Failure isolation | High (crash in one ≠ crash in others) | Low (crash in one = all crash) |
| Creation time | Slow | Fast |

### Mutex vs. Binary Semaphore vs. Counting Semaphore
| | **Mutex** | **Binary Semaphore** | **Counting Semaphore** |
|-|-----------|---------------------|----------------------|
| Values | 0 (locked) / 1 (unlocked) | 0 / 1 | 0 to N |
| Ownership | Yes (only locker unlocks) | No | No |
| Signaling between threads | No | Yes | Yes |
| Use case | Protect shared data | Mutual exclusion OR signaling | N resource pool (e.g. connection pool) |

### Scheduling Algorithms
| Algorithm | Preemptive | Starvation | Convoy Effect | Best for |
|-----------|-----------|------------|---------------|----------|
| FCFS | No | No | Yes | Simple batch |
| SJF | No | Yes | No | Min avg WT (non-preemptive) |
| SRTF | Yes | Yes | No | Min avg WT overall |
| Priority | Both | Yes | No | Priority systems |
| Round Robin | Yes | No | No | Interactive / time-sharing |
| MLFQ | Yes | No (aging) | No | General purpose OS |
| Linux CFS | Yes | No (fair share) | No | Modern Linux OS (Red-Black tree) |

### Page Replacement Algorithms
| Algorithm | Rule | Belady's Anomaly | Optimal? |
|-----------|------|-----------------|---------|
| FIFO | Evict oldest | ⚠️ Yes | No |
| OPT | Evict furthest future use | No | Yes (benchmark) |
| LRU | Evict least recently used | No | Near-optimal |
| LFU | Evict least frequently used | No | OK |

### Paging vs. Segmentation
| | **Paging** | **Segmentation** |
|-|-----------|-----------------|
| Division | Fixed-size pages | Variable-size segments |
| External Fragmentation | None | Yes |
| Internal Fragmentation | Yes (last page) | None |
| User view | Physical (transparent) | Logical (matches program structure) |

### IPC Mechanisms
| Mechanism | Speed | Direction | Cross-machine? | Use case |
|-----------|-------|-----------|----------------|---------|
| Shared Memory | Fastest | Bidirectional | No | High-throughput, same machine |
| Pipe | Fast | Unidirectional | No | Shell piping, parent-child |
| Message Queue | Medium | Bidirectional | No | Loosely coupled producers/consumers |
| Socket | Slower | Bidirectional | Yes | Distributed systems, microservices |
| Signal | Fast | Unidirectional | No | Events (`SIGKILL`, `SIGTERM`) |

---

## 📌 Critical Definitions

| Term | Definition |
|------|-----------|
| **PCB** | Process Control Block — data structure storing all process information |
| **TCB** | Thread Control Block — data structure storing thread state |
| **Context Switch** | Saving current process/thread state and restoring another's |
| **Dispatcher** | OS component that actually performs the CPU switch |
| **Convoy Effect** | Short jobs stuck behind a long job in FCFS |
| **Starvation** | Low-priority process never gets CPU because high-priority always arrives |
| **Aging** | Gradually increasing priority of waiting processes to prevent starvation |
| **Deadlock** | Circular wait — processes blocked forever waiting for each other's resources |
| **Livelock** | Processes actively change state but make no progress (actively stuck) |
| **Race Condition** | Result of concurrent execution depends on non-deterministic timing |
| **Critical Section** | Code segment that accesses shared resources exclusively |
| **Page Fault** | Access to a page not currently in RAM → OS loads it from disk |
| **TLB** | Translation Lookaside Buffer — hardware cache for page table entries |
| **Thrashing** | System spends more time paging than executing (too many page faults) |
| **Working Set** | Set of pages actively used by a process in a recent time window |
| **Zombie** | Process finished, PCB not yet cleaned up (parent hasn't called wait()) |
| **Orphan** | Process whose parent exited; adopted by init (PID 1) |
| **Belady's Anomaly** | More frames → more page faults (in FIFO only) |
| **Swapping** | Moving entire process to/from disk |
| **Demand Paging** | Load only needed pages, not entire process |
| **Copy-on-Write** | Parent and child share pages; copy only on write (makes fork fast) |
| **TLB ASID** | Address Space ID tag — avoids full TLB flush on context switch |
| **MMU** | Memory Management Unit — hardware that translates logical to physical addresses |
| **Linux CFS** | Completely Fair Scheduler — schedules processes using virtual runtime (`vruntime`) and a Red-Black tree |
| **ASLR** | Address Space Layout Randomization — randomizes memory locations to prevent buffer overflows |
| **NX Bit** | No-Execute — marks memory pages (like stack/heap) as non-executable to prevent code injection |
| **RAID** | Redundant Array of Independent Disks — combines multiple physical drives for speed/redundancy |

---

## 🔢 Important Formulas

```
TAT (Turnaround Time)  = Completion Time - Arrival Time
WT  (Waiting Time)     = TAT - Burst Time
RT  (Response Time)    = First CPU Start - Arrival Time

Page Table Size = (Logical Address Space / Page Size) × PTE size
                = 2^(m-k) × e   [m=addr bits, k=page size bits, e=PTE bytes]

Effective Access Time (EAT) = h × (t_tlb + t_mem) + (1-h) × (t_tlb + t_page + t_mem)
  where h = TLB hit rate, t_tlb ≈ 1 cycle, t_mem = 100 ns

Need[i] = Max[i] - Allocation[i]    [Banker's Algorithm]
```

---

## 🧩 The 4 Coffman Deadlock Conditions

> All four must hold simultaneously for a deadlock:

1. **Mutual Exclusion** — Resources are non-sharable
2. **Hold and Wait** — Processes hold resources while waiting for more
3. **No Preemption** — Resources can't be forcibly taken
4. **Circular Wait** — P1 waits for P2, P2 waits for P3, ..., Pn waits for P1

---

## 🎯 Top 30 OS Interview Questions

### Introduction & Architecture
1. What are the two core roles of an OS?
2. What is the difference between Multiprogramming and Multitasking?
3. What is the difference between a Monolithic and Microkernel?
4. What happens when a process calls `open("file.txt")`? (Walk through system call flow)
5. What is the difference between BIOS and UEFI?
6. Why does a 32-bit OS support only 4 GB RAM?

### Processes & Threads
7. Describe the memory layout of a process.
8. What is stored in a PCB? In a TCB?
9. Draw and explain the process state transition diagram.
10. Why is thread context switching faster than process context switching?
11. What is a zombie process? How is it cleaned up?
12. What is an orphan process? Who adopts it?
13. Compare all IPC mechanisms (shared memory, pipes, sockets, signals).

### CPU Scheduling
14. What is the Convoy Effect? Which algorithm causes it?
15. What is the difference between SJF and SRTF?
16. What is starvation? Which scheduling algorithms suffer from it?
17. What is aging? Which algorithm uses it?
18. What is the MLFQ? How is it different from MLQ?
19. **Numerical**: Given processes with arrival times and burst times, calculate avg TAT and WT for Round Robin (Q=2).

### Synchronization
20. What is a race condition? Give an example with code.
21. What are the three requirements for a correct critical section solution?
22. What is the difference between a mutex and a semaphore?
23. Can a thread unlock another thread's mutex? What about a semaphore?
24. What is a condition variable? Why use `while` loop (not `if`) when checking with `cv.wait`?
25. How do you solve the Dining Philosophers without deadlock?

### Deadlocks
26. State the four Coffman conditions.
27. What is a safe state? What is an unsafe state?
28. Walk through the Banker's Algorithm step-by-step.
29. What is the difference between deadlock and livelock?

### Memory Management
30. What is the difference between internal and external fragmentation?
31. How does paging eliminate external fragmentation?
32. Calculate page table size: 32-bit address space, 4 KB pages, 4-byte PTE.
33. What is a TLB? What happens on a TLB miss?
34. Calculate EAT: TLB hit rate = 90%, TLB access = 10 ns, memory access = 100 ns.

### Virtual Memory & Thrashing
35. Walk through step-by-step handling of a page fault.
36. What is Belady's Anomaly? Which algorithms are immune to it?
37. What is thrashing? What causes it?
38. What is the Working-Set Model?
39. What is Copy-on-Write? How does it make `fork()` efficient?
40. What is ASLR and how does it secure process memory layout?
41. Explain the NX bit and the W^X (Write XOR Execute) principle.
42. What role do R/W and U/S bits play in a Page Table Entry?

---

## 🏆 Frequently Confused Pairs — Final Clarifications

| Confused Pair | Key Distinction |
|---------------|----------------|
| Swapping vs. Demand Paging | Swapping = whole process to disk; Demand Paging = individual pages only on access |
| TLB hit vs. miss | Hit = 1 memory access; Miss = 2+ memory accesses (page table in RAM) |
| Deadlock vs. Starvation | Deadlock = circular wait (all stuck); Starvation = unfair scheduling (one stuck, others proceed) |
| Mutex vs. Binary Semaphore | Mutex has ownership (only locker unlocks); Binary semaphore doesn't (any thread can signal) |
| SJF vs. SRTF | SJF is non-preemptive; SRTF preempts on every new arrival |
| Internal vs. External Fragmentation | Internal = wasted space inside allocation; External = wasted space between allocations |
| Zombie vs. Orphan | Zombie = dead but PCB alive (parent hasn't waited); Orphan = alive but parentless |
| Page vs. Frame | Page = logical; Frame = physical; Same size |
| LRU vs. LFU | LRU = based on time (recency); LFU = based on count (frequency) |
| Hard Link vs. Soft Link | Hard = same inode (survives delete); Soft = path string (breaks on delete) |
| Journaling vs. fsck | Journaling = log before write (fast recovery); fsck = full disk scan (slow) |
| SCAN vs. C-SCAN | SCAN reverses at end; C-SCAN jumps back to start (more uniform wait times) |
| Polling vs. Interrupt-driven I/O | Polling = CPU busy-waits; Interrupt = CPU freed, device signals when done |
| DMA vs. Programmed I/O | DMA = device transfers directly to RAM; PIO = CPU copies every byte |

---

## ⚡ File Systems Quick Reference

### Inode Structure
```
Inode stores: permissions, owner, timestamps, size, ref count
Block pointers: 12 direct + 1 single indirect + 1 double + 1 triple
Max file size (4KB blocks): ~4 TB

Path resolution: / → inode 2 → directory entry → next inode → ...
```

### File Allocation Methods
| Method | External Frag | Random Access | Used In |
|--------|--------------|---------------|---------|
| Contiguous | Yes | ✅ Fast | CDs, some DBs |
| Linked (FAT) | No | ❌ Slow | USB drives |
| Indexed (inode) | No | ✅ Moderate | ext4, APFS |
| **RAID 0 / 1 / 5 / 10** | — | — | Speed (RAID 0), Redundancy (RAID 1), Distributed Parity (RAID 5), Mirrored Stripes (RAID 10) |

### Disk Scheduling (Head at 53, requests: 98 183 37 122 14 124 65 67)
| Algorithm | Total Movement | Starvation? |
|-----------|---------------|-------------|
| FCFS | 640 | No |
| SSTF | 236 | ⚠️ Yes |
| SCAN | 331 | No |
| C-SCAN | 382 | No |
| C-LOOK | Best | No |

> **Use C-LOOK** in practice for best balance of performance and fairness.
