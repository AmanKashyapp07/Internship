# OS — GS OA MCQ Revision Notes

---

## 1. Processes vs. Threads

| Feature | Process | Thread |
| :--- | :--- | :--- |
| **Definition** | An executing program with its own address space. | Lightweight unit of execution within a process. |
| **Memory** | Has independent memory space (Code, Data, Heap, Stack). | Shares Code, Data, and Heap with peer threads. Has its **own Stack and Registers**. |
| **Overhead** | High context-switching and creation overhead. | Low context-switching overhead. |
| **Communication**| IPC (Inter-Process Communication) like Pipes, Sockets, Shared Memory. | Direct access to shared memory (requires synchronization). |

---

## 2. Process Scheduling Algorithms

*   **First Come First Served (FCFS):** Non-preemptive. Can cause the **Convoy Effect** (short processes wait behind a long process).
*   **Shortest Job First (SJF):** Non-preemptive. Gives the **optimal average waiting time**. Can cause **starvation** for long processes.
*   **Shortest Remaining Time First (SRTF):** Preemptive version of SJF.
*   **Round Robin (RR):** Preemptive. Uses a fixed **Time Quantum**. Prevents starvation. Highly dependent on quantum size:
    *   *Too large* -> behaves like FCFS.
    *   *Too small* -> causes high context-switching overhead.
*   **Priority Scheduling:** Can cause starvation of low-priority processes. Resolved via **Aging** (gradually increasing priority of waiting processes).

---

## 3. Deadlocks (Coffman Conditions)

A deadlock occurs when processes are blocked waiting for resources held by each other. **All 4 conditions must hold simultaneously:**

1.  **Mutual Exclusion:** Only one process can use a resource at a time.
2.  **Hold and Wait:** A process holding allocated resources can request additional resources.
3.  **No Preemption:** Resources cannot be forcibly taken from a process.
4.  **Circular Wait:** A closed loop of processes exists where each waits for a resource held by the next.

### Deadlock Handling
*   **Prevention:** Design the system to violate at least one Coffman condition.
*   **Avoidance:** Dynamically checks resource allocation safety. Uses the **Banker's Algorithm** (determines if allocating a resource leaves the system in a **Safe State**).
*   **Detection & Recovery:** Detect cycles in the **Resource Allocation Graph (RAG)** and recover by terminating processes or preempting resources.

---

## 4. Process Synchronization

*   **Critical Section:** The segment of code where shared resources are accessed. Only one process should execute in it at a time.
*   **Mutex vs. Semaphore:**

| Parameter | Mutex (Mutual Exclusion) | Semaphore |
| :--- | :--- | :--- |
| **Mechanism** | Locking mechanism. | Signaling mechanism (using an integer value). |
| **Ownership** | **Has ownership.** Only the thread that locked the Mutex can unlock it. | **No ownership.** Any thread can signal (increment/decrement) it. |
| **Types** | Binary. | Binary (0 or 1) or Counting (range of integer values). |

---

## 5. Memory Management

*   **Paging:** Divides virtual memory into fixed-size **pages** and physical memory into **frames**.
    *   Eliminates **External Fragmentation**.
    *   Suffers from **Internal Fragmentation** (unused memory inside a allocated page).
*   **Segmentation:** Divides memory into variable-size logical segments (e.g., code, stack).
    *   Eliminates **Internal Fragmentation**.
    *   Suffers from **External Fragmentation** (total free memory is enough, but contiguous blocks are not available).
*   **Virtual Memory:** Allows execution of processes that are not completely in RAM using demand paging.
*   **Page Fault:** Occurs when a program tries to access a page that is mapped in virtual address space but not loaded in physical RAM.

### Page Replacement Algorithms
*   **FIFO (First-In, First-Out):** Replaces the oldest page.
    *   **Belady's Anomaly:** **(Very common MCQ)** An anomaly where increasing the number of physical page frames results in an *increase* in the number of page faults. Occurs in FIFO, but never in Stack-based algorithms like LRU.
*   **LRU (Least Recently Used):** Replaces the page that has not been accessed for the longest time.
*   **Optimal Page Replacement:** Replaces the page that will not be used for the longest period in the future (theoretical baseline, requires future knowledge).

---

## 6. CPU Execution Modes
*   **User Mode:** Restricted mode. User programs run here. Cannot execute privileged instructions directly.
*   **Kernel Mode:** Privileged mode. OS kernel runs here. Complete access to hardware.
*   **System Call:** Interface used by user applications to request services from the kernel (causes context switch from User Mode to Kernel Mode).
