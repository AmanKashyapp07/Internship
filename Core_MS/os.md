# Operating Systems — High ROI Revision (Microsoft Interview Prep)

> **Mindset**: Microsoft interviewers test **real systems mechanics**: memory layout, virtual address translation, locking internals (Spinlock vs Mutex, Priority Inversion), and page fault lifecycles. Skip high-level hand-waving and focus on low-level correctness.

---

## 1. Process vs. Thread & Process Memory Layout

| Dimension | Process | Thread |
|---|---|---|
| **Definition** | An isolated program execution instance | The smallest unit of CPU scheduling within a process |
| **Address Space** | Private virtual memory space (isolated) | Shares parent's address space (code, data, heap, open files) |
| **Private State** | Page tables, file descriptors, security tokens | Registers, Program Counter (PC), **Stack** (local variables) |
| **Creation Overhead**| High (`fork()` copies page tables, handles) | Low (allocates a new stack and register state) |
| **Context Switch Cost**| Heavy (flushes TLB, swaps page tables, CPU cache misses) | Light (swaps registers & stack pointer; page tables unchanged) |
| **Crash Blast Radius**| Crash is contained to itself | Unhandled exception in one thread crashes the entire process |

```
High Memory (0xFFFFFFFF)
┌────────────────────────────────────────┐
│             Kernel Space               │
├────────────────────────────────────────┤
│        Stack (grows downwards ▼)       │ Local variables, function call frames
├────────────────────────────────────────┤
│                   ▼                    │
│                                        │
│                   ▲                    │
│        Heap (grows upwards ▲)          │ Dynamic memory (`malloc`, `new`)
├────────────────────────────────────────┤
│     BSS (Block Started by Symbol)      │ Uninitialized global & static variables
├────────────────────────────────────────┤
│              Data Segment              │ Initialized global & static variables
├────────────────────────────────────────┤
│              Text Segment              │ Compiled executable machine instructions (Read-Only)
└────────────────────────────────────────┘
Low Memory (0x00000000)
```

> **User vs. Kernel Threads (1:1 Model)**:
> Modern OSes (Linux, Windows) use a **1:1 threading model** where each user-space thread maps directly to a kernel-scheduled thread. Lightweight primitives (Go Goroutines, Java 21 Virtual Threads) use an **M:N model** (green threads multiplexed over fewer OS threads) to support millions of concurrent tasks with small stacks (KB vs MB).

---

## 2. Inter-Process Communication (IPC)

Processes have isolated memory spaces and must use explicit OS mechanisms to communicate:

| IPC Mechanism | Speed | Scope | Key Characteristic |
|---|---|---|---|
| **Shared Memory** | **Fastest** (Zero-copy) | Same machine | Kernel sets up shared physical frames; processes must synchronize access themselves (via Mutex/Semaphore). |
| **Pipes / Anonymous Pipes** | Fast | Parent-child only | Byte stream, unidirectional, half-duplex. |
| **Named Pipes (FIFO)** | Fast | Unrelated processes | Appears as a file in filesystem; bidirectional. |
| **Message Queues** | Medium | Same machine | OS-managed queue of structured messages (avoids manual framing). |
| **Sockets** | Slower (network stack overhead)| Local or distributed | Standard interface for cross-machine network communication (TCP/UDP or Unix Domain Sockets). |

---

## 3. Concurrency, Locks & Synchronization Mechanics

### Mutex vs. Semaphore (The Core Nuances)
- **Mutex (Mutual Exclusion)**:
  - **Ownership concept**: The thread that locks the mutex **must** be the one that unlocks it.
  - Supports **Priority Inheritance** (prevents priority inversion).
  - Can be recursive/reentrant.
- **Semaphore (Signaling Mechanism)**:
  - **No ownership**: Any thread can call `signal()` / `release()` to unblock a waiting thread.
  - **Counting Semaphore**: Initialized with integer $N$ (tracks $N$ available shared resources).
  - **Binary Semaphore**: Can behave like a lock, but lacks ownership checks.

### Spinlock vs. Mutex
- **Spinlock**: Busy-waits in a CPU loop (`while (test_and_set(&lock))`).
  - *When to use*: On **multicore systems** when the lock hold time is guaranteed to be shorter than the cost of a context switch (~1–5 microseconds). Used heavily inside OS kernels.
- **Mutex**: Puts the waiting thread to **sleep**; CPU schedules another thread.
  - *When to use*: When the lock is held across I/O operations or lengthy computations.

### Critical Concurrency Pitfalls:
1. **Race Condition**: Program output depends on the unpredictable execution order/interleaving of threads.
2. **Deadlock vs. Livelock vs. Starvation**:
   - **Deadlock**: Threads blocked permanently waiting for resources held by each other (0% CPU utilization).
   - **Livelock**: Threads actively change states in response to each other, but make zero forward progress (100% CPU utilization, e.g., two people dodging each other in a hallway).
   - **Starvation**: A runnable thread is perpetually denied resources/CPU time due to scheduling bias.
3. **Priority Inversion**:
   - High-priority thread $H$ waits for a lock held by low-priority thread $L$. Medium-priority thread $M$ preempts $L$. Thus, $M$ indirectly blocks $H$!
   - *Fix*: **Priority Inheritance Protocol** — boost $L$'s priority temporarily to match $H$ until it releases the lock.

### Producer-Consumer Pattern (Classic Interview Implementation)
Solved using **1 Mutex + 2 Semaphores**:
```cpp
Semaphore empty(BUFFER_SIZE); // Counts empty slots
Semaphore full(0);            // Counts filled items
Mutex mtx;                    // Protects critical buffer access

void producer() {
    while (true) {
        item = produceItem();
        empty.wait();         // Decrement empty slots (blocks if full)
        mtx.lock();
        buffer.push(item);
        mtx.unlock();
        full.signal();        // Increment full slots (wakes consumer)
    }
}

void consumer() {
    while (true) {
        full.wait();          // Decrement full slots (blocks if empty)
        mtx.lock();
        item = buffer.pop();
        mtx.unlock();
        empty.signal();       // Increment empty slots (wakes producer)
        consumeItem(item);
    }
}
```

---

## 4. Deadlock — Conditions, Prevention & Detection

### The 4 Necessary Coffman Conditions:
1. **Mutual Exclusion**: Non-shareable resource.
2. **Hold and Wait**: Process holds ≥1 resource while waiting for another.
3. **No Preemption**: Resources cannot be forcibly revoked.
4. **Circular Wait**: $P_0 \to P_1 \to \dots \to P_n \to P_0$.

### Handling Strategies:
- **Prevention (Break 1 condition)**:
  - *Best approach in practice*: **Break Circular Wait** by enforcing a strict global ordering of all resource locks (always acquire Lock A before Lock B).
- **Avoidance (Banker's Algorithm)**:
  - Dynamically inspects resource requests. Grants requests only if the remaining state leaves at least one safe execution sequence where all processes can finish.
- **Detection & Recovery**:
  - Maintain a **Resource Allocation Graph (RAG)**; detect cycles periodically. Recovery: Kill offending process or rollback state.

---

## 5. Virtual Memory, Paging & Page Fault Lifecycle

### Virtual-to-Physical Address Translation
Every memory access translates a virtual address to a physical RAM address via the **MMU (Memory Management Unit)**:
```
Virtual Address: [ Virtual Page Number (VPN) | Page Offset ]
                          │
                   Check TLB (Hardware Cache)
                   ┌──────┴──────┐
             Hit: Fast       Miss: Look up Page Table in RAM
                                 │
                   Present bit = 1? ➔ Get Physical Frame Number (PFN)
                   Present bit = 0? ➔ Trigger PAGE FAULT Interrupt!
```

### Complete Page Fault Lifecycle (Step-by-Step)
1. CPU executes an instruction referencing an unmapped virtual address (`Present bit == 0`).
2. MMU generates a **Page Fault interrupt (Trap to kernel)**.
3. OS saves registers and program counter of the faulting process.
4. OS validates the memory access:
   - If invalid address ➔ send `SIGSEGV` (Segmentation Fault).
   - If valid ➔ locate page on backing disk (swap space).
5. OS selects a free physical frame in RAM. If RAM is full, runs a **Page Replacement Algorithm** to evict a "victim" page (writing it to disk if dirty).
6. OS issues disk I/O read to load requested page into the frame (process moves to Waiting state).
7. Disk I/O completes ➔ OS updates the Page Table entry (`Present bit = 1`, frame number set).
8. OS invalidates the stale **TLB** entry.
9. Process is scheduled back to Ready; the faulting instruction is **re-executed seamlessly**.

### Page Replacement Algorithms & Belady's Anomaly
- **FIFO**: Simple queue. Suffers from **Belady's Anomaly** (allocating *more* physical page frames can actually cause *more* page faults!).
- **LRU (Least Recently Used)**: Evicts page untouched for the longest time. Optimal in practice; stack-based (immune to Belady's Anomaly).
- **Clock / Second-Chance**: Approximates LRU in real kernels using a single hardware access bit per page.

### Thrashing (The System Meltdown)
- **Definition**: When the CPU spends virtually all its time swapping pages in/out of disk rather than executing instructions.
- **Cause**: The sum of all active processes' **Working Sets** exceeds total available physical RAM.
- **Remedy**: Decrease degree of multiprogramming (suspend/kill lower-priority processes) or add physical RAM.

---

## 6. System Calls, Kernel Mode & Context Switching

### Dual-Mode CPU Operation:
- **User Mode (Ring 3)**: Unprivileged instructions. Direct hardware/memory access is blocked to ensure stability.
- **Kernel Mode (Ring 0)**: Unrestricted execution privileges; direct hardware and control register access.

### How a System Call Works:
1. User application calls standard library wrapper (`read()`).
2. Wrapper sets up arguments in registers and executes a software interrupt / trap instruction (`syscall` / `sysenter`).
3. Hardware elevates CPU privilege level to Ring 0 and jumps to the **Syscall Dispatch Table** in kernel memory.
4. Kernel executes requested service, writes result into return register.
5. Kernel executes `sysret` instruction, dropping privilege back to Ring 3 and resuming user program.

### What Makes a Context Switch Expensive?
1. **Direct Cost**: Saving and restoring hardware registers, Program Counter, and Stack Pointer.
2. **Indirect Cost (The Real Penalty)**:
   - **TLB Invalidation**: Swapping page tables flushes the TLB; subsequent memory lookups suffer severe cache misses.
   - **CPU L1/L2/L3 Cache Pollution**: The new process executes different code/data, causing rampant cache line misses until the working set warms up.

---

## Rapid-Fire Interview Drill (Answer in 30 Seconds)
1. *Why does a thread context switch cost less than a process context switch?* ➔ Threads share the same address space and page tables; no TLB flush or memory map swap is required.
2. *Can a mutex be unlocked by a different thread than the one that locked it?* ➔ No, a mutex has strict thread ownership. A semaphore can be signaled by any thread.
3. *What is Belady's Anomaly?* ➔ A phenomenon in FIFO page replacement where increasing physical memory frames increases the total number of page faults.
4. *What is the difference between internal and external fragmentation?* ➔ Internal is wasted space *inside* allocated fixed blocks (paging); external is wasted non-contiguous free space *between* dynamic blocks (segmentation).
5. *How does the OS resolve Priority Inversion?* ➔ Through Priority Inheritance: temporarily boosting the low-priority lock-holder's priority to match the blocked high-priority thread.