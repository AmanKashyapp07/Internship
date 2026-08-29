# Master Guide 05: Virtual Memory, Page Replacement & Thrashing

> **Focus:** Demand Paging, Page Fault 6-Step Interrupt Cycle, Head-to-Head Algorithm Race (FIFO vs Optimal vs LRU vs Clock), Belady's Anomaly Mechanics, Second-Chance / Clock Algorithm, Thrashing Cause-Detection-Fix, and the Working Set Model.
> 
> *Targeted for Top-Tier Tech System & Backend Engineering Interviews.*

---

# Table of Contents
1. [Demand Paging & The Page Fault 6-Step Lifecycle](#1-demand-paging--the-page-fault-6-step-lifecycle)
2. [The 4-Algorithm Head-to-Head Race](#2-the-4-algorithm-head-to-head-race)
3. [Belady's Anomaly: The #1 Interview Gotcha](#3-beladys-anomaly-the-1-interview-gotcha)
4. [Clock & Enhanced Second-Chance Algorithms](#4-clock--enhanced-second-chance-algorithms)
5. [Thrashing: Cause, Detection & Fix](#5-thrashing-cause-detection--fix)
6. [Working Set Model & Page Fault Frequency (PFF)](#6-working-set-model--page-fault-frequency-pff)
7. [High-Frequency Interview Drill & Verbal Q&A](#7-high-frequency-interview-drill--verbal-qa)

---

# 1. Demand Paging & The Page Fault 6-Step Lifecycle

```
[ Step 1: CPU Memory Reference ] ---> [ MMU Checks PTE ] ---> (Present Bit = 0)
                                                                    |
+-------------------------------------------------------------------+
|
v
[ Step 2: Trap to Kernel ] (Hardware Page Fault Exception, saves faulting address to CR2)
|
v
[ Step 3: Check Validity ] (Kernel verifies access is legitimate, not a SIGSEGV)
|
v
[ Step 4: Disk I/O Fetch ] (Allocate free frame, issue async disk read to swap/file)
|                          (Faulting process moved to Blocked state; CPU switches tasks)
v
[ Step 5: Update Page Table ] (Write frame # into PTE, set Present Bit = 1, set Dirty = 0)
|
v
[ Step 6: Restart Instruction ] (Move process to Ready Queue; CPU re-executes exact instruction)
```

- **One-Line Intuition:** Demand paging is streaming a movie scene-by-scene on demand instead of waiting for the entire 50 GB file to download before hitting play.
- **The Interview Trap:** Forgetting Step 6: the CPU does NOT resume from the next instruction; it **restarts the exact faulting instruction** that triggered the page fault.
- **30-Second Verbal Answer:** **"Demand paging loads virtual pages into RAM only upon first access. When a non-resident page is referenced, the MMU generates a Page Fault trap. The kernel validates the reference, fetches the page from backing store into a free physical frame, updates the Page Table Entry's valid bit, and restarts the faulting instruction."**

---

# 2. The 4-Algorithm Head-to-Head Race

**Reference String:** `7, 0, 1, 2, 0, 3, 0, 4, 2, 3` with **3 Physical Frames**.

```
+----------------------------------------------------------------------------------------------------+
| REF STRING | 7   0   1   2   0   3   0   4   2   3  | TOTAL PAGE FAULTS | HIT RATIO                |
+----------------------------------------------------------------------------------------------------+
| 1. FIFO    | F1: 7   7   7   2   2   2   2   4   4   4  |                   |                          |
|            | F2: -   0   0   0   0   3   3   3   2   2  |    8 FAULTS       | 2 / 10 = 20%             |
|            | F3: -   -   1   1   1   1   0   0   0   3  |                   |                          |
+----------------------------------------------------------------------------------------------------+
| 2. OPTIMAL | F1: 7   7   7   2   2   2   2   2   2   2  |                   |                          |
| (Belady)   | F2: -   0   0   0   0   0   0   4   4   3  |    6 FAULTS       | 4 / 10 = 40% (Optimal)   |
|            | F3: -   -   1   1   1   3   3   3   3   3  |                   |                          |
+----------------------------------------------------------------------------------------------------+
| 3. LRU     | F1: 7   7   7   2   2   2   2   4   4   4  |                   |                          |
|            | F2: -   0   0   0   0   0   0   0   2   2  |    7 FAULTS       | 3 / 10 = 30%             |
|            | F3: -   -   1   1   1   3   3   3   3   3  |                   |                          |
+----------------------------------------------------------------------------------------------------+
| 4. CLOCK   | Tracks (Page, Reference Bit); clears 1 -> 0    |    7 FAULTS       | 3 / 10 = 30%             |
| (2nd Chance)| on sweep, evicts first 0 found. Approximates LRU|                   | (Near-LRU, O(1) HW cost) |
+----------------------------------------------------------------------------------------------------+
```

### Algorithm Comparison Matrix:
- **Optimal (OPT):** Replace page not used for the longest time in the future. **Theoretical minimum faults**, impossible in practice (requires future knowledge).
- **LRU (Least Recently Used):** Replace page not used for the longest time in the past. High implementation cost (needs timestamp or doubly linked list + hash map).
- **FIFO (First-In, First-Out):** Replace oldest loaded page. Vulnerable to Belady's Anomaly.
- **Clock (Second Chance):** Uses single hardware reference bit per frame. Gives pages a second chance if accessed recently; standard algorithm in production kernels.

---

# 3. Belady's Anomaly: The #1 Interview Gotcha

```
+---------------------------------------------------------------------------------------------------+
| BELADY'S ANOMALY DEFINITION:                                                                      |
| Allocating MORE physical frames causes an INCREASE in the total number of page faults!            |
| Occurs primarily in FIFO. Does NOT occur in Stack Algorithms (LRU, Optimal, LFU).                 |
+---------------------------------------------------------------------------------------------------+
```

### Classic Proof Reference String:
**String:** `1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5`

```
+-----------------------------------+-----------------------------------+
| WITH 3 FRAMES (9 Page Faults)     | WITH 4 FRAMES (10 Page Faults!)   |
+-----------------------------------+-----------------------------------+
| 1: [1, -, -] (Fault)              | 1: [1, -, -, -] (Fault)           |
| 2: [1, 2, -] (Fault)              | 2: [1, 2, -, -] (Fault)           |
| 3: [1, 2, 3] (Fault)              | 3: [1, 2, 3, -] (Fault)           |
| 4: [4, 2, 3] (Fault - evicts 1)   | 4: [1, 2, 3, 4] (Fault)           |
| 1: [4, 1, 3] (Fault - evicts 2)   | 1: [1, 2, 3, 4] (Hit!)            |
| 2: [4, 1, 2] (Fault - evicts 3)   | 2: [1, 2, 3, 4] (Hit!)            |
| 5: [5, 1, 2] (Fault - evicts 4)   | 5: [5, 2, 3, 4] (Fault - evicts 1)|
| 1: [5, 1, 2] (Hit!)               | 1: [5, 1, 3, 4] (Fault - evicts 2)|
| 2: [5, 1, 2] (Hit!)               | 2: [5, 1, 2, 4] (Fault - evicts 3)|
| 3: [3, 1, 2] (Fault - evicts 5)   | 3: [5, 1, 2, 3] (Fault - evicts 4)|
| 4: [3, 4, 2] (Fault - evicts 1)   | 4: [4, 1, 2, 3] (Fault - evicts 5)|
| 5: [3, 4, 5] (Fault - evicts 2)   | 5: [4, 5, 2, 3] (Fault - evicts 1)|
+-----------------------------------+-----------------------------------+
| Total Faults: 9                   | Total Faults: 10 (ANOMALY!)       |
+-----------------------------------+-----------------------------------+
```

- **Why LRU is Immune (Stack Property):** A **Stack Algorithm** guarantees that the set of pages in memory for $N$ frames is always a strict subset of the pages in memory for $N + 1$ frames ($M(N) \subseteq M(N+1)$). FIFO violates this property.

---

# 4. Clock & Enhanced Second-Chance Algorithms

```
                          Clock Hand Pointer
                                  |
                                  v
                        +---> [ Frame 0: (P7, bit=0) ]
                        |     [ Frame 1: (P0, bit=1) ] ---> (Bit=1: Reset to 0, advance hand)
                        |     [ Frame 2: (P3, bit=0) ] ---> (Bit=0: EVICT THIS FRAME!)
                        +------------------------------+
```

### Enhanced Second-Chance Priority Order $(R, M)$:
1. **Class 0 $(0, 0)$:** Neither recently referenced nor modified — **Best page to replace** (Zero disk I/O).
2. **Class 1 $(0, 1)$:** Not recently referenced, but modified — Must write to disk, but not recently needed.
3. **Class 2 $(1, 0)$:** Recently referenced, but clean — Likely to be needed again soon.
4. **Class 3 $(1, 1)$:** Recently referenced and modified — **Worst page to replace**.

---

# 5. Thrashing: Cause, Detection & Fix

```
[ Processes spawned / RAM depleted ]
                 |
                 v
[ Sum of Working Sets > Physical RAM ]
                 |
                 v
[ Page Fault rate skyrockets ] ---------------------------> [ CPU sits idle waiting for Disk I/O ]
                 |                                                        |
                 v                                                        v
[ Pager constantly evicts active pages ] <--------------- [ OS spawns MORE processes thinking CPU is idle! ]
```

### Thrashing in 3 Lines:
- **Cause:** Degree of multiprogramming is too high; $\sum \text{Working Sets} > \text{Total Physical Frames}$, causing processes to spend more time swapping pages than executing instructions.
- **Detection:** **CPU utilization plummets** toward 0% while **disk swap I/O utilization approaches 100%**.
- **Fix:** Decrease the degree of multiprogramming (suspend/swap out entire processes to disk) and enforce the **Working Set Model**.

---

# 6. Working Set Model & Page Fault Frequency (PFF)

```
Process Access History: ... 2 6 1 5 7 7 7 7 5 1 [ Current Time t ]
                               |<--- Working Set Window Delta --->|
                               Working Set W(t, Delta=6) = { 1, 5, 7 } (Size = 3 Frames)
```

### 1. Working Set Model (Peter Denning):
- Define **Working Set Window $\Delta$** (fixed number of past page references).
- $\text{Working Set } W(t, \Delta) = \text{Set of distinct pages referenced in }(t - \Delta, t)$.
- Total demand $D = \sum |W_i|$.
- **Kernel Policy:** If $D > \text{Total Available Frames}$, **suspend an entire process** to prevent thrashing.

### 2. Page Fault Frequency (PFF) Strategy:
```
Page Fault Rate
       ^
       |   [ Upper Threshold ] ---> High Fault Rate: ALLOCATE MORE FRAMES TO PROCESS
       |   -------------------
       |
       |   -------------------
       |   [ Lower Threshold ] ---> Low Fault Rate: RECLAIM REDUNDANT FRAMES FROM PROCESS
       +-----------------------> Time
```

---

# 7. High-Frequency Interview Drill & Verbal Q&A

### Q1: What is the difference between Spatial Locality and Temporal Locality?
> **Answer:** **Temporal Locality** means recently accessed memory locations are likely to be accessed again soon (e.g. loops, stack variables). **Spatial Locality** means locations near recently accessed addresses are likely to be accessed soon (e.g. array traversals, sequential code execution).

### Q2: Why is the Optimal Page Replacement algorithm impossible to implement in practice?
> **Answer:** Because it requires **future knowledge of the exact page reference sequence**. It is used purely as an offline benchmark to evaluate the efficiency of real-world algorithms like LRU and Clock.

### Q3: What is Belady's Anomaly and which algorithms are immune to it?
> **Answer:** Belady's Anomaly is the phenomenon where **increasing the number of physical frames increases total page faults**. It occurs in FIFO, but **Stack Algorithms** (such as LRU and Optimal) are mathematically immune because the set of pages in $N$ frames is always a subset of pages in $N+1$ frames.

### Q4: How does Linux handle page replacement under memory pressure?
> **Answer:** Linux uses an active/inactive **Dual-List Clock Algorithm (variant of 2Q/LRU)**. Pages start in an Inactive List; if referenced again while in the inactive list, they are promoted to the Active List. Memory reclamation (via `kswapd`) scans the inactive list to evict clean pages or flush dirty pages.

### Q5: How does the OS distinguish between a fatal Segmentation Fault and a valid Page Fault?
> **Answer:** On a page fault, the kernel checks the faulting virtual address (stored in register `CR2` on x86) against the process's **Virtual Memory Areas (VMAs)** in the PCB (`mm_struct`). If the address is within a valid allocated VMA with proper read/write permissions, it is a **Demand Page Fault**; otherwise, the kernel emits a **`SIGSEGV` (Segmentation Fault)**.
