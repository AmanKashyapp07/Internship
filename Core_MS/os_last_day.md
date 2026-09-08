# Operating Systems — Last-Day Revision & Flashcards (Microsoft Prep)

> **Goal**: 15-minute high-yield cram sheet. Review these flashcards out loud, then review the core formulas, tables, and top traps.

---

## Part 1: Top 20 High-Yield OS Flashcards

### Card 1: Why is a thread context switch cheaper than a process context switch?
- **Answer**: Threads in the same process share the same virtual address space, page tables, and memory maps. A thread context switch only swaps registers, Program Counter, and Stack Pointer. A process context switch must also **swap page tables, flush the TLB**, and suffers severe **CPU cache misses (L1/L2/L3)**.

### Card 2: What are the segments of a process in memory (from High to Low)?
- **Answer**:
  1. Kernel Space (top)
  2. **Stack** (grows downwards $\downarrow$: local variables, return addresses)
  3. **Heap** (grows upwards $\uparrow$: dynamic allocations via `malloc`/`new`)
  4. **BSS** (uninitialized global/static variables, zero-initialized by OS)
  5. **Data** (initialized global/static variables)
  6. **Text** (compiled machine instructions, read-only)

### Card 3: What is the fundamental difference between a Mutex and a Semaphore?
- **Answer**:
  - **Mutex**: A locking mechanism with **strict thread ownership** (only the thread that acquired the lock can release it). Supports Priority Inheritance.
  - **Semaphore**: A **signaling mechanism** with **no ownership** (any thread can call `signal()` to unblock a waiting thread). Can be binary or counting.

### Card 4: When should you use a Spinlock instead of a Mutex?
- **Answer**: On **multicore machines** when the critical section is extremely short and the expected wait time is less than the cost of a thread context switch (~1–5 $\mu$s). Mutex puts the thread to sleep (context switch overhead); Spinlock busy-waits in a tight CPU loop.

### Card 5: What is Priority Inversion and how does an OS resolve it?
- **Answer**: A high-priority thread ($H$) is blocked waiting for a lock held by a low-priority thread ($L$), while a medium-priority thread ($M$) preempts $L$, indefinitely delaying $H$.
- **Fix**: **Priority Inheritance Protocol** — temporarily elevate the priority of $L$ to match $H$ until $L$ releases the lock.

### Card 6: What are the 4 necessary Coffman conditions for Deadlock?
- **Answer**:
  1. **Mutual Exclusion**: Resource can be held by only one process at a time.
  2. **Hold and Wait**: Process holds $\ge 1$ resource while waiting for another.
  3. **No Preemption**: Resources cannot be forcibly taken away.
  4. **Circular Wait**: $P_0$ waits for $P_1$, which waits for $P_2 \dots$ which waits for $P_0$.

### Card 7: How do you prevent Deadlock in production software?
- **Answer**: Break the **Circular Wait** condition by establishing a **global lock hierarchy**. All threads must acquire locks in the exact same predefined numerical or alphabetical order (e.g., always acquire Lock A before Lock B).

### Card 8: What is Deadlock vs. Livelock vs. Starvation?
- **Answer**:
  - **Deadlock**: Processes are permanently blocked waiting for each other (0% CPU, no state change).
  - **Livelock**: Processes actively change internal states in response to each other, but make zero forward progress (100% CPU burn).
  - **Starvation**: A runnable process is perpetually denied CPU time or resources due to unfair scheduler priority.

### Card 9: What happens step-by-step during a Page Fault?
- **Answer**:
  1. MMU encounters a page table entry with `Present bit == 0` ➔ triggers a **Page Fault trap** to kernel.
  2. OS validates memory address (if invalid $\to$ `SIGSEGV`).
  3. OS finds a free physical page frame (or runs page replacement algorithm to evict a page to disk).
  4. OS schedules disk I/O to read the missing page from swap into RAM.
  5. Page table updated (`Present bit = 1`), **TLB invalidated**, process resumed by re-executing the faulting instruction.

### Card 10: What is the TLB and what happens on a TLB miss?
- **Answer**: The **Translation Lookaside Buffer** is a high-speed hardware associative cache on the CPU that stores recent Virtual Page Number to Physical Frame Number translations. On a miss, the hardware page table walker (or OS) must read the page tables in physical RAM (adding memory latency).

### Card 11: What is Belady’s Anomaly?
- **Answer**: The counter-intuitive phenomenon in **FIFO page replacement** where increasing the number of physical page frames actually **increases** the number of page faults for certain reference strings. (LRU and Optimal are stack-based algorithms and immune to this).

### Card 12: What is Thrashing and how do you detect/fix it?
- **Answer**:
  - **Definition**: When the CPU spends nearly 100% of its time swapping pages in/out of disk rather than executing instructions.
  - **Cause**: Sum of all active processes' **Working Sets** exceeds total available physical RAM.
  - **Fix**: Suspend or kill low-priority processes (decrease multiprogramming degree) or add more physical RAM.

### Card 13: What is the difference between User Mode and Kernel Mode?
- **Answer**:
  - **User Mode (Ring 3)**: Unprivileged execution. Code cannot execute privileged instructions, access hardware directly, or read arbitrary memory addresses.
  - **Kernel Mode (Ring 0)**: Unrestricted execution with direct hardware access and full memory addressability. Switching modes requires a trap/syscall instruction.

### Card 14: What is the fastest IPC mechanism on a single host?
- **Answer**: **Shared Memory**. The kernel maps the same physical memory frames into the virtual address spaces of both processes. Communication is zero-copy at RAM speeds (processes must synchronize using semaphores/mutexes).

### Card 15: What is Internal Fragmentation vs. External Fragmentation?
- **Answer**:
  - **Internal**: Wasted space *inside* an allocated fixed-size block (common in **Paging** when a process doesn't use a full 4KB page).
  - **External**: Wasted free space *between* dynamic memory allocations where total free memory is sufficient but non-contiguous (common in **Segmentation**).

### Card 16: What is the tradeoff of Round Robin time quantum size?
- **Answer**:
  - Too large ➔ Degenerates into **FCFS** (poor response time for short interactive jobs, convoy effect).
  - Too small ➔ **Context switch overhead dominates**, reducing actual CPU throughput.

### Card 17: What is the concept behind Banker’s Algorithm?
- **Answer**: A **Deadlock Avoidance** algorithm. Before granting a resource request, the OS checks if the resulting allocation leaves the system in a **Safe State** (i.e. there exists at least one order in which all processes can finish). If not safe, the request is delayed.

### Card 18: What is a Zombie Process vs. an Orphan Process?
- **Answer**:
  - **Zombie**: A child process that has terminated, but its exit status has not yet been read by its parent via `wait()`. Remains in the process table holding its PID.
  - **Orphan**: A running child whose parent terminated. Automatically adopted by `init` / `systemd` (PID 1), which periodically calls `wait()` to reap it.

### Card 19: What is CAS (Compare-And-Swap)?
- **Answer**: An atomic hardware CPU instruction (`cmpxchg` on x86) that compares the contents of a memory location to a given value and, if they match, modifies the contents to a new value in a single atomic step. Foundation of lock-free data structures.

### Card 20: How do you solve the Producer-Consumer problem?
- **Answer**: Use **1 Mutex** (for mutual exclusion on the shared buffer) and **2 Semaphores**:
  - `empty` (initialized to buffer capacity $N$)
  - `full` (initialized to $0$)
  Producer waits on `empty`, signals `full`. Consumer waits on `full`, signals `empty`.

---

## Part 2: 1-Minute Memory Matrix & Key Points

### Process vs Thread Quick Checklist:
| Attribute | Process | Thread |
|---|---|---|
| Virtual Address Space | Separate (isolated) | Shared |
| Stack | Private | Private |
| Heap & Global Data | Private | Shared |
| File Descriptors | Private | Shared |
| Switch Cost | Heavy (TLB flush) | Light (registers only) |

### Virtual Address Translation Formula:
$$\text{Virtual Address} = \text{Virtual Page Number (VPN)} + \text{Page Offset}$$
$$\text{Page Size} = 2^{\text{Offset Bits}} \quad (\text{e.g., } 12 \text{ offset bits} = 4096 \text{ bytes} = 4\text{KB})$$

### Scheduling Tradeoffs:
- **FCFS**: Simple, non-preemptive, suffers from **Convoy Effect**.
- **SJF / SRTF**: Provably optimal minimum average waiting time; suffers from **Starvation** of long jobs.
- **Round Robin**: Fair, preemptive; performance hinges on time quantum $q$.
- **Priority Scheduling**: Starvation mitigated via **Aging** (gradually bumping waiting process priority).

---

## Part 3: Top 5 Microsoft Traps in OS

1. **Confusing Deadlock Prevention with Avoidance**:
   - Prevention statically breaks one of the 4 Coffman conditions (e.g. strict lock ordering). Avoidance dynamically analyzes requests at runtime using Banker's Algorithm.
2. **Thinking Mutex is just a Binary Semaphore**:
   - Mutex has **thread ownership** (unlock must come from locker) and supports Priority Inheritance. A binary semaphore is a signaling flag that any thread can post.
3. **Believing Zombie Processes consume RAM**:
   - Zombie processes have already freed their memory and file descriptors. They only consume an entry in the OS **Process Table** (PID allocation).
4. **Ignoring Indirect Context Switch Costs**:
   - The primary cost of a process context switch is not saving registers; it is the **TLB flush** and subsequent CPU cache misses while warming up the new process's memory.
5. **Thinking Threads don't have private stacks**:
   - Threads share heap and global variables, but **each thread MUST have its own independent stack** to track its own local variables and function call chain.
