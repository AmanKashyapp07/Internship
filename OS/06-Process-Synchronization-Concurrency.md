# Master Guide 06: Process Synchronization & Concurrency

> **Focus:** Race Conditions, Critical Section 3 Rules, Peterson's Algorithm, Hardware Atomics (TAS & CAS), Mutex vs Semaphore vs Spinlock Decision Matrix, Spurious Wakeups & Mesa Monitors, Priority Inversion, and Classic Concurrency Problems (Producer-Consumer, Readers-Writers, Dining Philosophers).
> 
> *Targeted for Top-Tier Tech System & Backend Engineering Interviews.*

---

# Table of Contents
1. [Race Conditions & The 3 Critical Section Rules](#1-race-conditions--the-3-critical-section-rules)
2. [Software & Hardware Synchronization Primitives](#2-software--hardware-synchronization-primitives)
3. [The Master Synchronization Decision Matrix](#3-the-master-synchronization-decision-matrix)
4. [Condition Variables, Monitors & Spurious Wakeups](#4-condition-variables-monitors--spurious-wakeups)
5. [Priority Inversion & Priority Inheritance Protocol](#5-priority-inversion--priority-inheritance-protocol)
6. [Classic Concurrency Problems & Compact Code Solutions](#6-classic-concurrency-problems--compact-code-solutions)
7. [High-Frequency Interview Drill & Verbal Q&A](#7-high-frequency-interview-drill--verbal-qa)

---

# 1. Race Conditions & The 3 Critical Section Rules

```
+---------------------------------------------------------------------------------------------------+
| 3 MANDATORY CRITICAL SECTION CRITERIA                                                             |
+---------------------------------------------------------------------------------------------------+
| 1. Mutual Exclusion  | If process Pi is executing in its critical section, no other processes     |
|                      | can execute in their critical sections simultaneously.                     |
+---------------------------------------------------------------------------------------------------+
| 2. Progress          | If no process is in its critical section and some wish to enter, only      |
|                      | processes not in their remainder sections can participate in the decision. |
+---------------------------------------------------------------------------------------------------+
| 3. Bounded Waiting   | There must be a bound on the number of times other processes are allowed   |
|                      | to enter their critical sections after a process has made a request.       |
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** Race condition is two people simultaneously editing the same Google Doc without operational transform—the last save blindly overwrites the first.
- **The Interview Trap:** Thinking Mutual Exclusion alone is sufficient. Without **Progress**, a system can deadlock; without **Bounded Waiting**, a process can starve indefinitely.
- **30-Second Verbal Answer:** **"A race condition occurs when multiple threads concurrently access and mutate shared data, making the final outcome dependent on non-deterministic execution order. A valid synchronization solution must guarantee three properties: Mutual Exclusion, Progress (no deadlock on entry), and Bounded Waiting (no indefinite starvation)."**

---

# 2. Software & Hardware Synchronization Primitives

### 1. Peterson's Algorithm (2 Processes: $P_i$ and $P_j$)
```c
bool flag[2] = {false, false}; // flag[i] = true means Pi wants to enter
int turn;                      // Whos turn it is to enter

// Process Pi Entry Code:
flag[i] = true;
turn = j; // Yield turn generously to the other process
while (flag[j] && turn == j); // Busy wait

// --- CRITICAL SECTION ---

// Process Pi Exit Code:
flag[i] = false;
```
- **Why it works:** `turn` breaks ties deterministically. Even if both set `flag = true` at the exact same instant, `turn` can only hold one value ($i$ or $j$), granting entry to exactly one process while guaranteeing mutual exclusion, progress, and bounded waiting.

### 2. Hardware Atomic Primitives: Test-and-Set vs. Compare-and-Swap (CAS)
```c
// Compare-And-Swap (CAS) Atomic Semantic:
int compare_and_swap(int *ptr, int expected, int new_val) {
    int old_val = *ptr;
    if (old_val == expected) {
        *ptr = new_val;
    }
    return old_val; // Returns previous value; if old_val == expected, CAS succeeded!
}
```
- **Why Hardware Atomics Matter:** Modern multi-core CPUs use out-of-order execution and memory store buffers. Lock-free algorithms rely on hardware atomic CAS (`lock cmpxchg` on x86) and Memory Barriers (`mfence`) to prevent CPU instruction reordering.

---

# 3. The Master Synchronization Decision Matrix

```
+---------------------------------------------------------------------------------------------------+
| PRIMITIVE            | OWNERSHIP | BLOCKING BEHAVIOR        | IDEAL USE CASE                      |
+---------------------------------------------------------------------------------------------------+
| Mutex (Lock)         | YES       | Sleep / Context Switch   | Protecting exclusive access to data |
|                      | (Lock/Unlock by SAME thread) | (Thread moves to Wait Queue)| structures in user-space |
+---------------------------------------------------------------------------------------------------+
| Counting Semaphore   | NO        | Sleep / Context Switch   | Resource pool management            |
| (Signaling Token)    | (Thread A can signal Thread B) | (Tracks integer resource count)| (e.g. max 10 DB connections) |
+---------------------------------------------------------------------------------------------------+
| Binary Semaphore     | NO        | Sleep / Context Switch   | Signaling events between threads    |
|                      | (No ownership required)   | (Can be unlocked by anyone)  | (Task completion notifications)     |
+---------------------------------------------------------------------------------------------------+
| Spinlock             | YES       | Busy-Wait (Consumes 100% | Critical sections held for < 1-2 us |
|                      |           | CPU in a tight loop)     | inside OS kernel on MULTI-CORE CPUs |
+---------------------------------------------------------------------------------------------------+
| Condition Variable   | With Mutex| Sleep until signaled     | Waiting for complex boolean state   |
| (cv / monitor)       |           | (Releases mutex on wait) | transitions (Buffer full/empty)     |
+---------------------------------------------------------------------------------------------------+
```

### Spinlock Golden Rule:
- **Multi-Core:** Use spinlocks **ONLY if the critical section is shorter than the context-switch overhead** (~2-5 microseconds).
- **Single-Core:** **NEVER use a spinlock on a single CPU core** without preemption! The spinning thread starves the thread holding the lock from ever running to release it.

---

# 4. Condition Variables, Monitors & Spurious Wakeups

```c
// The Canonical Monitor / Condition Variable Pattern:
std::unique_lock<std::mutex> lock(mtx);

// MUST use while loop, NEVER an if statement!
while (!condition_is_met()) {
    cv.wait(lock); // Atomically unlocks mtx and puts thread to sleep; re-acquires mtx on wakeup
}

// Perform work with shared state safely...
```

### Why `while` is Mandatory over `if` (Spurious Wakeups & Mesa Semantics):
1. **Spurious Wakeup:** An OS thread can wake up from `cv.wait()` even if no signal was sent (due to OS kernel signal interruptions or race conditions in `futex`).
2. **Mesa Semantics:** When `cv.notify_one()` is called, the waking thread is moved to the Ready Queue. By the time it actually acquires the mutex and runs, another thread may have entered and invalidated the condition.
3. **The Rule:** Always re-evaluate the predicate in a `while` loop upon waking.

---

# 5. Priority Inversion & Priority Inheritance Protocol

```
Scenario:
Task H (High Priority)      --- Blocked waiting for Lock held by L ---> (STARVED!)
Task M (Medium Priority)    =========================================> (Preempts L and hogs CPU!)
Task L (Low Priority)       --- Holds Lock, but preempted by M -------> (Cannot finish to release Lock)
```

- **The Mars Pathfinder Case (1997):** A high-priority attitude control thread starved because a medium-priority communications thread preempted a low-priority meteorological logging task holding a shared resource mutex, triggering repeated system watchdog resets.
- **The Solution (Priority Inheritance Protocol - PIP):** When Task $H$ blocks on a mutex held by Task $L$, Task $L$ **temporarily inherits $H$'s high priority** until it releases the mutex, preventing Task $M$ from preempting it.

---

# 6. Classic Concurrency Problems & Compact Code Solutions

### A. Producer-Consumer (Bounded Buffer) Problem
- **Core Trick:** 2 Counting Semaphores (`emptySlots = N`, `fullSlots = 0`) + 1 Mutex for buffer push/pop.

```cpp
// Shared Synchronization Primitives:
Semaphore emptySlots(N); // Tracks available buffer capacity
Semaphore fullSlots(0);  // Tracks produced items ready for consumption
Mutex mtx;               // Protects buffer integrity

void producer() {
    while (true) {
        Item item = produce();
        emptySlots.wait();  // Wait for space
        mtx.lock();
        buffer.push(item);
        mtx.unlock();
        fullSlots.signal(); // Signal available item
    }
}

void consumer() {
    while (true) {
        fullSlots.wait();   // Wait for item
        mtx.lock();
        Item item = buffer.pop();
        mtx.unlock();
        emptySlots.signal(); // Signal freed space
        consume(item);
    }
}
```

---

### B. Readers-Writers Problem (Reader Preference)
- **Core Trick:** The first reader locks the resource for all readers (`writeLock.wait()`); the last reader leaves and unlocks it (`writeLock.signal()`).

```cpp
int readCount = 0;
Mutex readCountMtx;
Semaphore writeLock(1);

void reader() {
    readCountMtx.lock();
    readCount++;
    if (readCount == 1) writeLock.wait(); // First reader blocks writers
    readCountMtx.unlock();

    // --- READING HAPPENS CONCURRENTLY ---

    readCountMtx.lock();
    readCount--;
    if (readCount == 0) writeLock.signal(); // Last reader unblocks writers
    readCountMtx.unlock();
}

void writer() {
    writeLock.wait();
    // --- WRITING HAPPENS EXCLUSIVELY ---
    writeLock.signal();
}
```

---

### C. Dining Philosophers Problem
- **Core Trick:** Enforce an **asymmetric resource hierarchy** (e.g. odd philosophers pick Left then Right; even philosophers pick Right then Left) to break Circular Wait.

```cpp
Mutex chopstick[5];

void philosopher(int i) {
    int first = min(i, (i + 1) % 5);
    int second = max(i, (i + 1) % 5);

    while (true) {
        think();
        chopstick[first].lock();  // Pick lower ID chopstick first
        chopstick[second].lock(); // Pick higher ID chopstick second
        
        eat();
        
        chopstick[second].unlock();
        chopstick[first].unlock();
    }
}
```

---

# 7. High-Frequency Interview Drill & Verbal Q&A

### Q1: What is the fundamental difference between a Mutex and a Binary Semaphore?
> **Answer:** **Ownership.** A Mutex has an owner: only the exact thread that called `lock()` is permitted to call `unlock()`. A Binary Semaphore has no ownership: any thread can call `signal()` to wake up a thread waiting in `wait()`, making semaphores ideal for inter-thread event signaling.

### Q2: Why is `notify_all()` (broadcast) sometimes preferred over `notify_one()`?
> **Answer:** When multiple waiting threads have **different condition predicates** or when a state change allows multiple threads to proceed simultaneously (e.g. a Read-Write lock releasing a writer, unblocking multiple waiting readers).

### Q3: What is a Futex in the Linux Kernel?
> **Answer:** Fast User-space Mutex. It attempts to acquire the lock in **user-space using an atomic CAS without making a system call**. It traps into the kernel only if there is contention and the thread must be put to sleep, making uncontended lock acquisition near-zero overhead.

### Q4: Can a deadlock occur with only 1 thread?
> **Answer:** **Yes (Self-Deadlock / Recursive Deadlock).** If a single thread attempts to acquire a non-recursive mutex that it already holds, it blocks waiting for itself to release the lock, causing a permanent deadlock.

### Q5: What is the ABA problem in lock-free concurrency and how is it solved?
> **Answer:** A thread reads value $A$, is preempted, another thread changes $A \to B \to A$, and the original thread's CAS succeeds incorrectly assuming nothing changed. It is solved using **versioned / tagged pointers** (e.g. atomic `(pointer, version)` pairs or double-word CAS `cmpxchg16b`).
