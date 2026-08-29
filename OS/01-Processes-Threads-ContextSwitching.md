# Master Guide 01: Processes, Threads & Context Switching

> **Focus:** Process Memory Layout, Process vs Thread Resource Sharing, PCB vs TCB, State Transitions, Context Switch Direct vs Indirect Costs, TLB/ASID Mechanics, Fork-Exec-Wait, Copy-on-Write (COW), Zombie vs Orphan Reaping, and 1:1 vs M:1 vs M:N Threading Models.
> 
> *Targeted for Top-Tier Tech System & Backend Engineering Interviews.*

---

# Table of Contents
1. [Process Memory Layout & Architecture](#1-process-memory-layout--architecture)
2. [Process vs. Thread: The Resource Matrix](#2-process-vs-thread-the-resource-matrix)
3. [PCB vs. TCB (Task Control Blocks)](#3-pcb-vs-tcb-task-control-blocks)
4. [Process State Lifecycle & Transitions](#4-process-state-lifecycle--transitions)
5. [Context Switching: The Real Cost Breakdown](#5-context-switching-the-real-cost-breakdown)
6. [fork(), exec(), wait() & Copy-on-Write (COW)](#6-fork-exec-wait--copy-on-write-cow)
7. [Zombie vs. Orphan Processes](#7-zombie-vs-orphan-processes)
8. [Thread Models: 1:1 vs. M:1 vs. M:N](#8-thread-models-11-vs-m1-vs-mn)
9. [High-Frequency Interview Drill & Verbal Q&A](#9-high-frequency-interview-drill--verbal-qa)

---

# 1. Process Memory Layout & Architecture

```
+-------------------------------------------------------+ 0xFFFFFFFF (High Memory)
|                 Kernel Virtual Memory                 | (Identical mapping for all processes)
+-------------------------------------------------------+
|                    Stack (v Grows Down)               | Local variables, function call frames
|                           |                           |
|                           v                           |
|                                                       |
|                           ^                           |
|                           |                           |
|                    Heap (^ Grows Up)                  | Dynamic allocations (malloc, new)
+-------------------------------------------------------+
|                 Uninitialized Data (.bss)             | Global/static vars initialized to 0
+-------------------------------------------------------+
|                  Initialized Data (.data)             | Global/static vars initialized with non-zero
+-------------------------------------------------------+
|                     Text / Code (.text)               | Binary instructions (Read-Only)
+-------------------------------------------------------+ 0x00000000 (Low Memory)
```

- **One-Line Intuition:** A process is an isolated container of resources with its own private address space; code sits at the bottom, dynamic memory grows inward from both ends.
- **The Interview Trap:** Candidates often claim the stack and heap can collide into each other freely. In modern 64-bit OS with virtual memory, guard pages and `mmap` regions sit between them, triggering a `SIGSEGV` long before physical collision.
- **30-Second Verbal Answer:** **"A process virtual address space is partitioned into Text for instructions, Data and BSS for global state, Heap for runtime allocations growing upward, and Stack for call frames growing downward. The MMU maps these virtual pages to arbitrary physical frames."**
- **Follow-up:** *Where do static variables inside a function live?* **In the `.data` segment if initialized with a non-zero value, or `.bss` if uninitialized/zero, NOT on the stack.**

---

# 2. Process vs. Thread: The Resource Matrix

```
+---------------------------------------------------------------------------------------------------+
| RESOURCE CATEGORY        | SHARED ACROSS THREADS (Process-Level) | PRIVATE PER THREAD (Thread-Level)      |
+---------------------------------------------------------------------------------------------------+
| Memory Segments          | Code (.text), Data (.data, .bss), Heap| Stack (local call frames)              |
| Execution Context        | Global Variables, Process ID (PID)    | Program Counter (PC), CPU Registers    |
| OS Descriptors           | File Descriptors (Sockets, Files)     | Thread ID (TID), Signal Mask           |
| Security & IPC           | IPC Channels, Permissions, Signals    | Thread-Local Storage (TLS), Errno      |
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** A process is an entire house with furniture; threads are people living in the house sharing the rooms, but each carrying their own private notebook.
- **The Interview Trap:** Saying "threads do not have their own stack." Threads MUST have their own private stacks to track independent function execution call chains.
- **30-Second Verbal Answer:** **"A process is the fundamental unit of resource allocation with isolated memory and file descriptors. A thread is the fundamental unit of CPU execution within a process, sharing text, data, heap, and open files, but maintaining its own Program Counter, register set, and stack."**
- **Follow-up:** *Can Thread A access the stack variables of Thread B in the same process?* **Yes, because they share the same address space. If Thread B passes a pointer to its stack variable, Thread A can read/write it directly, though it is dangerous and error-prone.**

---

# 3. PCB vs. TCB (Task Control Blocks)

```
+------------------------------------+          +------------------------------------+
|     PROCESS CONTROL BLOCK (PCB)    |          |      THREAD CONTROL BLOCK (TCB)    |
+------------------------------------+          +------------------------------------+
| Process ID (PID), Parent PID (PPID)|          | Thread ID (TID)                    |
| Process State (Ready/Running/Wait) |          | Thread State (Ready/Running/Wait)  |
| Page Table Pointer (e.g. CR3 reg)  |--------->| Program Counter (PC)               |
| File Descriptor Table              |          | CPU Register State Save Area       |
| Signal Handlers & Pending Signals  |          | Stack Pointer (SP)                 |
| CPU Scheduling Priority & Stats    |          | Pointer to Parent PCB              |
+------------------------------------+          +------------------------------------+
```

- **One-Line Intuition:** The PCB is the master passport of the entire application; the TCB is the boarding pass for a single worker thread inside it.
- **The Interview Trap:** Thinking Linux maintains completely separate struct hierarchies for processes and threads.
- **30-Second Verbal Answer:** **"In Linux, both processes and threads are represented by the unified `struct task_struct`. Threads are simply lightweight processes created via `clone()` with shared memory (`CLONE_VM`), shared file system (`CLONE_FILES`), and shared signal handlers (`CLONE_SIGHAND`)."**

---

# 4. Process State Lifecycle & Transitions

```
                    [ Admit ]
                       |
                       v
                 +------------+     Scheduler Dispatch     +-------------+
  +------------->|   READY    |--------------------------->|   RUNNING   |
  |              +------------+                            +-------------+
  |                    ^                                          |
  |                    |              Timer / Preemption          |
  |                    +------------------------------------------+
  |                                                               |
  |              +------------+                                   |
  +--------------|  WAITING / |<----------------------------------+
    I/O or Event |   BLOCKED  |       I/O Request / Wait for Event
      Complete   +------------+
```

### 7-State Model (Including Swapped States):
- **Ready Suspended:** Process is in secondary storage (disk) but ready to run as soon as brought into RAM.
- **Blocked Suspended:** Process is blocked on an event and swapped out to disk.

### Critical State Transition Triggers:
- **Running -> Ready:** Interrupt (timer expiration in Round Robin), higher priority process arrives.
- **Running -> Waiting:** Blocking system call (`read()`, `sleep()`, waiting for mutex/semaphore).
- **Waiting -> Ready:** I/O completion interrupt, lock acquired, signal received.

- **One-Line Intuition:** You wait in line (Ready), you get served (Running), you step aside to sign a form (Waiting/Blocked), then you get back in line (Ready).
- **The Interview Trap:** Thinking a process transitions directly from "Waiting" to "Running". It NEVER does; it always moves to "Ready" first to wait for scheduler dispatch.
- **30-Second Verbal Answer:** **"A process transitions through New, Ready, Running, Waiting, and Terminated. When running, a timer interrupt preempts it to Ready, whereas a blocking I/O request transitions it to Waiting. Once I/O finishes, the kernel moves it back to Ready, not Running."**

---

# 5. Context Switching: The Real Cost Breakdown

```
+---------------------------------------------------------------------------------------------------+
| COST COMPONENT       | PROCESS CONTEXT SWITCH (Heavy)        | THREAD CONTEXT SWITCH (Light)      |
+---------------------------------------------------------------------------------------------------+
| Direct CPU Cost      | Save/restore general registers, SP, PC| Save/restore general registers, SP |
| Memory Management    | Switch Page Table pointer (CR3 / TTBR0)| Keep same Page Table pointer       |
| Hardware TLB Impact  | Flush TLB entries (or switch ASID)    | Zero TLB flush (address space same)|
| Cache Pollution      | Massive L1/L2 cache misses (Cold)     | Low to moderate cache misses (Warm)|
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** Switching threads is swapping workers in the same office; switching processes is moving the entire company to a new building.
- **The Interview Trap:** Claiming that register saving/restoring is the main bottleneck. The direct register save takes under 1-2 microseconds; the **indirect cost of cache and TLB cold misses** over the next several thousand cycles dominates total latency.
- **30-Second Verbal Answer:** **"Context switching cost has two components: direct cost from saving and restoring CPU registers and swapping page table base pointers in the kernel, and indirect cost from TLB invalidation and CPU L1/L2 cache pollution, causing a flurry of memory stalls immediately after the switch."**
- **Follow-up:** *How do modern CPUs avoid flushing the TLB on a process context switch?* **Using Address Space Identifiers (ASID) or PCID (Process Context Identifiers) on x86/ARM, tagging TLB entries with the process ID so entries from different processes can coexist in the TLB.**

---

# 6. fork(), exec(), wait() & Copy-on-Write (COW)

```
Parent Memory Pages [R/W]             fork() called             Parent & Child Share Pages [Read-Only]
+-----------------------+           ----------------->          +------------------------------------+
| Page 1 | Page 2| Page3|                                       | Page 1 (RO) | Page 2 (RO)| Page 3  |
+-----------------------+                                       +------------------------------------+
                                                                                 ^
                                                                                 | Write to Page 2
                                                                                 v
                                                                 Duplicate ONLY Page 2 for Child [R/W]
                                                                +------------------------------------+
                                                                | Page 2' (Child Private Copy)       |
                                                                +------------------------------------+
```

### System Call Responsibilities:
- `fork()`: Creates an exact duplicate child process. Returns `0` to child, child's `PID` to parent, and `-1` on error.
- `exec()`: Overwrites current process address space with a new executable binary. Does NOT create a new PID.
- `wait()` / `waitpid()`: Blocks parent until a child terminates, collects its exit code, and frees the child's PCB entry.

### Copy-on-Write (COW) Mechanics:
1. `fork()` duplicates only page table references, marking all physical pages **Read-Only**.
2. If either process attempts a write, the MMU triggers a Page Fault interrupt.
3. The kernel intercepts the fault, allocates a new physical frame, copies only that specific 4KB page, marks it Read-Write, and resumes execution.

- **One-Line Intuition:** Don't photocopy the entire 500-page book on `fork()`; share the book, and photocopy a single page only when someone writes on it.
- **The Interview Trap:** Assuming `fork()` doubles memory usage instantly. Thanks to COW, `fork()` is $O(1)$ in memory allocation, copying only page tables.
- **30-Second Verbal Answer:** **"`fork()` creates a child sharing the parent's physical pages marked read-only via Copy-on-Write. When either process writes to a page, a hardware page fault triggers the kernel to duplicate only that specific page. `exec()` replaces the memory image with a new program, and `wait()` reaps the child's termination status."**

---

# 7. Zombie vs. Orphan Processes

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | ZOMBIE PROCESS                        | ORPHAN PROCESS                     |
+---------------------------------------------------------------------------------------------------+
| Definition           | Child has terminated, but parent has  | Parent has terminated before child |
|                      | NOT called `wait()` to read exit code | has finished execution             |
| Resource Consumption | Consumes NO CPU or RAM; holds 1 entry | Runs normally; consumes CPU & RAM  |
|                      | in Kernel Process Table (PID leak)    | as an active executing process     |
| Resolution / Reaping | Parent calls `wait()`, or kill parent | Automatically adopted by `init`    |
|                      | so `init` (PID 1) reaps it            | (PID 1 / `systemd`), which calls `wait()`|
| Command to Identify  | `ps aux | grep 'Z'` (State 'Z' / defunct)| `ps -ef` (Shows PPID = 1)          |
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** A Zombie is a dead body waiting for a burial certificate (`wait()`); an Orphan is a living child whose parents left, adopted by the state (`init`).
- **The Interview Trap:** Trying to kill a zombie process with `kill -9 <zombie_pid>`. A zombie is ALREADY dead; `SIGKILL` cannot kill a dead process. You must kill its parent or make the parent call `wait()`.
- **30-Second Verbal Answer:** **"A Zombie is a terminated process whose parent has not yet read its exit status via `wait()`, holding a slot in the process table. An Orphan is an active process whose parent exited first, causing it to be adopted by `init` (PID 1), which periodically calls `wait()` to prevent it from becoming a permanent zombie."**
- **Follow-up:** *What danger do too many zombie processes present?* **PID exhaustion. The kernel has a finite PID limit (e.g. 32,768 or `sysctl kernel.pid_max`); if exhausted, no new processes can be spawned.**

---

# 8. Thread Models: 1:1 vs. M:1 vs. M:N

```
+---------------------------------------------------------------------------------------------------+
| MODEL                | ARCHITECTURE                          | PROS                 | CONS                |
+---------------------------------------------------------------------------------------------------+
| 1:1 (Kernel-Level)   | 1 User Thread = 1 Kernel Thread       | True multi-core      | High creation and   |
| (Linux NPTL, Java)   | (Managed directly by OS scheduler)    | parallelism; 1 block | context-switch cost |
|                      |                                       | doesn't stall others | in kernel space     |
+---------------------------------------------------------------------------------------------------+
| M:1 (User-Level)     | M User Threads = 1 Kernel Thread      | Ultra-fast context   | No true multicore   |
| (Green Threads)      | (Managed by user runtime library)     | switch in user space | parallelism; 1 block|
|                      |                                       | (Zero syscalls)      | blocks ALL threads  |
+---------------------------------------------------------------------------------------------------+
| M:N (Hybrid)         | M User Threads multiplexed over       | Best of both worlds; | Complex scheduler   |
| (Go Goroutines)      | N Kernel Threads (Work-stealing engine)| lightweight + true   | implementation and  |
|                      |                                       | multicore execution  | runtime overhead    |
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** 1:1 = 1 passenger per taxi; M:1 = 50 passengers on 1 bus; M:N = 50 passengers dynamically dispatched across 4 minivans.
- **The Interview Trap:** Believing User-Level Threads (M:1) can utilize multiple CPU cores in parallel. Because the OS kernel only sees 1 execution context, it schedules it on only 1 core at a time.
- **30-Second Verbal Answer:** **"1:1 maps each user thread to a kernel thread, enabling true multi-core parallel execution at the expense of higher context switch overhead. M:1 runs entirely in user-space with fast switching but blocks all threads on a single blocking I/O call. M:N multiplexes M user threads onto N kernel threads, as seen in Go's work-stealing scheduler."**

---

# 9. High-Frequency Interview Drill & Verbal Q&A

### Q1: What happens under the hood when a process makes a system call?
> **Answer:** The CPU switches from **User Mode (Ring 3) to Kernel Mode (Ring 0)** via a software interrupt or `syscall` instruction. The hardware saves the User PC and stack pointer onto the Kernel Stack, indexes the **System Call Descriptor Table**, executes the handler in kernel space, and restores registers via `sysret`/`iret`.

### Q2: Why is `fork()` followed immediately by `exec()` inefficient without Copy-on-Write?
> **Answer:** Because without COW, `fork()` would duplicate the entire physical address space only for `exec()` to immediately deallocate and overwrite it. COW ensures `fork()` only copies page table references in $O(1)$ physical allocation time.

### Q3: What is the difference between a mode switch and a context switch?
> **Answer:** A **mode switch** transitions the current thread between User Mode and Kernel Mode (same process context, same page table). A **context switch** saves the execution state of one thread/process and loads that of a completely different thread/process, changing register sets and potentially swapping page tables.

### Q4: Can two processes share the same physical memory frame?
> **Answer:** **Yes.** Shared memory regions (`shmget`, `mmap`), shared dynamic libraries (`.so` / `.dll` text segments), and Copy-on-Write pages map different virtual addresses across distinct process page tables to the exact same physical frame.

### Q5: How does the kernel know which process to wake up when an I/O event completes?
> **Answer:** When a process blocks on I/O, its PCB is moved from the Ready Queue to the device's **Wait Queue**. When the device finishes, a hardware interrupt occurs, and the device driver identifies the pending I/O request, extracts the PCB from the wait queue, and moves it to the **Ready Queue**.
