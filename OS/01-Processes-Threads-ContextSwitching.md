# Process Architecture, Threading Models & Context Switching Theory

> **Scope:** Process Virtual Memory Layout, Process vs. Thread Resource Boundary, Task Control Blocks (PCB / TCB), Context Switching Costs (Direct CPU vs. Indirect TLB / Cache Invalidation), Process State Lifecycle, Fork-Exec-Wait Semantics, Copy-on-Write (COW), Zombie & Orphan Management, and Multithreading Architectures (1:1, M:1, M:N).

---

# Table of Contents
1. [Process Virtual Memory Layout](#1-process-virtual-memory-layout)
2. [Process vs. Thread Resource Separation](#2-process-vs-thread-resource-separation)
3. [Kernel Control Blocks: PCB vs. TCB](#3-kernel-control-blocks-pcb-vs-tcb)
4. [Process State Lifecycle & Scheduling Transitions](#4-process-state-lifecycle--scheduling-transitions)
5. [Context Switching Mechanics & Performance Cost Analysis](#5-context-switching-mechanics--performance-cost-analysis)
6. [Process Creation: fork(), exec(), wait() & Copy-on-Write (COW)](#6-process-creation-fork-exec-wait--copy-on-write-cow)
7. [Zombie and Orphan Process Lifecycles](#7-zombie-and-orphan-process-lifecycles)
8. [Thread Execution Models (1:1 vs. M:1 vs. M:N)](#8-thread-execution-models-11-vs-m1-vs-mn)
9. [Core Theoretical Summary Principles](#9-core-theoretical-summary-principles)

---

# 1. Process Virtual Memory Layout

A process represents an executing program instance isolated within its own private virtual address space. In modern 32-bit/64-bit architectures, the operating system kernel and the Memory Management Unit (MMU) organize a process address space into standardized virtual memory segments:

```
+-------------------------------------------------------+ 0xFFFFFFFF (High Memory)
|                 Kernel Virtual Memory                 | (Directly mapped for kernel operations)
+-------------------------------------------------------+
|                    Stack (v Grows Downward)           | Local variables, function frames, return ptrs
|                           |                           |
|                           v                           |
|                                                       |
|                           ^                           |
|                           |                           |
|                    Heap (^ Grows Upward)              | Dynamic memory allocations (brk / mmap)
+-------------------------------------------------------+
|                 Uninitialized Data (.bss)             | Global/static variables initialized to 0
+-------------------------------------------------------+
|                  Initialized Data (.data)             | Global/static variables initialized non-zero
+-------------------------------------------------------+
|                     Text / Code (.text)               | Compiled binary instructions (Read-Only)
+-------------------------------------------------------+ 0x00000000 (Low Memory)
```

### Segment Characteristics:
1. **Text Segment (`.text`):** Contains machine code instructions. Marked read-only to prevent self-modifying code; shared among multiple instances executing the same binary.
2. **Initialized Data Segment (`.data`):** Contains global and static variables explicitly initialized with non-zero values before compile time.
3. **Uninitialized Data Segment (`.bss`):** Contains global and static variables initialized to zero or uninitialized. Initialized to zero by the OS loader without consuming disk binary space.
4. **Heap Segment:** Dynamically allocated memory managed at runtime via `brk()`, `sbrk()`, and `mmap()` system calls. Expands upward toward higher memory addresses.
5. **Stack Segment:** LIFO memory structure holding stack frames for active functions (local variables, parameters, saved frame pointer, return instruction pointer). Grows downward toward lower memory addresses.

---

# 2. Process vs. Thread Resource Separation

A **Process** is the fundamental unit of resource ownership (address space, open files, security context). A **Thread** (Lightweight Process) is the fundamental unit of CPU execution scheduling.

```
+---------------------------------------------------------------------------------------------------+
| RESOURCE CATEGORY        | SHARED ACROSS THREADS (Process-Level) | PRIVATE PER THREAD (Thread-Level)      |
+---------------------------------------------------------------------------------------------------+
| Memory Address Space     | Code (.text), Data (.data, .bss), Heap| Stack Segment (Call Frames)            |
| Execution State          | Global Variables, Process ID (PID)    | Program Counter (PC), Register State   |
| OS Descriptors           | File Descriptors (Sockets, Pipes)     | Thread ID (TID), Signal Mask           |
| IPC & Permissions        | IPC Channels, Credentials, Signals    | Thread-Local Storage (TLS), Errno      |
+---------------------------------------------------------------------------------------------------+
```

---

# 3. Kernel Control Blocks: PCB vs. TCB

Operating system kernels manage processes and threads through internal kernel data structures.

```
+------------------------------------+          +------------------------------------+
|     PROCESS CONTROL BLOCK (PCB)    |          |      THREAD CONTROL BLOCK (TCB)    |
+------------------------------------+          +------------------------------------+
| Process ID (PID), Parent PID (PPID)|          | Thread ID (TID)                    |
| Process State (Ready/Running/Wait) |          | Thread State (Ready/Running/Wait)  |
| Page Table Base Pointer (CR3/TTBR0)|--------->| Program Counter (PC)               |
| File Descriptor Table              |          | CPU Register Context Save Area     |
| Signal Handlers & Signal Masks     |          | Stack Pointer (SP)                 |
| CPU Scheduling Priority & Stats    |          | Pointer to Parent PCB              |
+------------------------------------+          +------------------------------------+
```

### Linux Kernel Unified Task Representation:
In Linux, processes and threads are both represented internally by `struct task_struct`. The `clone()` system call creates threads by specifying sharing flags:
- `CLONE_VM`: Share virtual memory address space.
- `CLONE_FILES`: Share open file descriptor tables.
- `CLONE_SIGHAND`: Share signal handlers.

---

# 4. Process State Lifecycle & Scheduling Transitions

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

### Transition Semantics:
1. **Admit $\to$ Ready:** Process image is loaded into memory, PCB allocated, and placed into the Ready Queue.
2. **Ready $\to$ Running:** CPU Scheduler selects the process and dispatches execution via context switch.
3. **Running $\to$ Ready:** Preemption occurs due to timer interrupt expiration (e.g. Round Robin quantum) or arrival of a higher-priority task.
4. **Running $\to$ Waiting (Blocked):** Process issues a blocking system call (`read()`, `sleep()`, lock acquisition).
5. **Waiting $\to$ Ready:** Asynchronous event completion (hardware I/O interrupt, signal delivery) moves the PCB to the Ready Queue. A waiting process **never transitions directly to Running**.

---

# 5. Context Switching Mechanics & Performance Cost Analysis

A **Context Switch** is the kernel procedure of saving the execution state of the currently running task and loading the saved state of the next selected task.

```
+---------------------------------------------------------------------------------------------------+
| COST COMPONENT       | PROCESS CONTEXT SWITCH                | THREAD CONTEXT SWITCH              |
+---------------------------------------------------------------------------------------------------+
| Register State       | Save/restore general registers, SP, PC| Save/restore general registers, SP |
| Memory Management    | Swap Page Table base pointer (CR3)    | Retain identical Page Table base   |
| Hardware TLB Impact  | Flush TLB entries (or swap ASID)      | Zero TLB flush (Same address space)|
| Cache Locality       | High L1/L2/L3 cache misses (Cold)     | Low to moderate cache misses (Warm)|
+---------------------------------------------------------------------------------------------------+
```

### Performance Cost Breakdown:
1. **Direct Cost (1-2 $\mu$s):** Saving general-purpose registers, program counter, stack pointer to kernel stack, switching kernel stack pointers, and loading new register state.
2. **Indirect Cost (Dominant Latency Source):** 
   - **TLB Invalidation:** Swapping the page table pointer clears the Translation Lookaside Buffer (TLB), forcing subsequent memory references to incur multi-level page table walks.
   - **Cache Pollution:** The new process accesses distinct memory lines, evicting the previous process's working set from L1/L2/L3 hardware caches.
3. **Hardware Mitigation (ASID / PCID):** Modern CPU architectures (ARM ASID, x86 PCID) tag TLB entries with an Address Space Identifier, permitting TLB entries from distinct address spaces to co-exist without flushing across process switches.

---

# 6. Process Creation: fork(), exec(), wait() & Copy-on-Write (COW)

```
Parent Memory Pages [R/W]             fork() Invocation         Parent & Child Share Pages [Read-Only]
+-----------------------+           ------------------->        +------------------------------------+
| Page 1 | Page 2| Page3|                                       | Page 1 (RO) | Page 2 (RO)| Page 3  |
+-----------------------+                                       +------------------------------------+
                                                                                 ^
                                                                                 | Write to Page 2
                                                                                 v
                                                                 Allocate Physical Frame for Child [R/W]
                                                                +------------------------------------+
                                                                | Page 2' (Child Private Copy)       |
                                                                +------------------------------------+
```

### System Call Execution Roles:
- **`fork()`:** Clones the calling process. Returns child PID to parent, `0` to child, and `-1` on error.
- **`execve()` / `exec()`:** Replaces the current process address space, heap, stack, and text with a new executable binary image. The PID remains unchanged.
- **`wait()` / `waitpid()`:** Suspends parent execution until a child state transition occurs, collecting the child termination status and enabling kernel PCB reclamation.

### Copy-on-Write (COW) Optimization:
- Traditional `fork()` duplicated the entire physical memory of the parent ($O(N)$ allocation overhead).
- **COW Implementation:** `fork()` duplicates only the parent's page table entries and marks all shared physical frames as **Read-Only**.
- When either process executes a write instruction to a shared page, the MMU raises a **Page Fault Exception**.
- The kernel handles the fault by allocating a new physical frame, copying the 4KB page content, configuring the faulting process's page table with Read-Write permissions, and resuming execution.

---

# 7. Zombie and Orphan Process Lifecycles

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | ZOMBIE PROCESS                        | ORPHAN PROCESS                     |
+---------------------------------------------------------------------------------------------------+
| Definition           | Child terminated; parent has not      | Parent terminated while child is   |
|                      | invoked `wait()` to collect exit code | actively running                   |
| Resource Consumption | Zero CPU and RAM; holds 1 entry in    | Consumes standard CPU and RAM      |
|                      | Kernel Process Table (PID entry)      | resources as an active process     |
| Resolution Mechanism | Parent invokes `wait()`, or parent    | Re-parented to `init` (PID 1 /     |
|                      | terminates so `init` reaps child      | `systemd`), which reaps it         |
| System Danger        | PID Exhaustion (Exhausting kernel PID | None (Executes normally until      |
|                      | allocation table limits)              | natural termination)               |
+---------------------------------------------------------------------------------------------------+
```

---

# 8. Thread Execution Models (1:1 vs. M:1 vs. M:N)

```
+---------------------------------------------------------------------------------------------------+
| MODEL                | ARCHITECTURE                          | ADVANTAGES           | DISADVANTAGES       |
+---------------------------------------------------------------------------------------------------+
| 1:1 (Kernel-Level)   | 1 User Thread = 1 Kernel Thread       | True multi-core      | Higher memory and   |
| (Linux NPTL, Java)   | (Managed directly by OS scheduler)    | parallelism; blocking| context switch cost |
|                      |                                       | thread does not stall| in kernel space     |
+---------------------------------------------------------------------------------------------------+
| M:1 (User-Level)     | M User Threads = 1 Kernel Thread      | Extremely fast user- | Zero multi-core     |
| (Green Threads)      | (Managed entirely in user runtime)    | space context switch | parallelism; 1 block|
|                      |                                       | (No system calls)    | blocks entire group |
+---------------------------------------------------------------------------------------------------+
| M:N (Hybrid Model)   | M User Threads multiplexed over       | Scalable lightweight | Complex runtime     |
| (Go Goroutines)      | N Kernel Threads (Work-Stealing)      | user threads + true  | scheduler and thread|
|                      |                                       | multi-core execution | preemption logic    |
+---------------------------------------------------------------------------------------------------+
```

---

# 9. Core Theoretical Summary Principles

1. **Memory Separation:** A process maintains private virtual segments (Text, Data, BSS, Heap, Stack) mapped through page tables to physical frames.
2. **Context Switching Costs:** Direct register saves require microseconds; dominant performance degradation stems from TLB invalidation and CPU cache misses.
3. **Copy-on-Write:** Defers physical frame duplication until write access occurs, allowing `fork()` to execute in $O(1)$ time with shared read-only pages.
4. **Process Cleanup:** Terminated child processes require parent `wait()` invocation to free kernel PCB entries; un-reaped processes become zombies, risking PID exhaustion.
