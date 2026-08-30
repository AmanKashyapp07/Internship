# Process Synchronization & Concurrency Control Theory

> **Scope:** Race Conditions, Critical Section Problem Criteria (Mutual Exclusion, Progress, Bounded Waiting), Peterson's Algorithm, Hardware Atomic Primitives (TAS, CAS, Memory Fences), Mutex vs. Semaphore vs. Spinlock Decision Framework, Linux Futexes, Condition Variables & Monitors (Spurious Wakeups, Mesa Semantics), Priority Inversion & PIP, Classical Synchronization Problems, and The ABA Problem.

---

# Table of Contents
1. [Race Conditions & Critical Section Problem Formulation](#1-race-conditions--critical-section-problem-formulation)
2. [Software Synchronization: Peterson's Algorithm](#2-software-synchronization-petersons-algorithm)
3. [Hardware Synchronization Primitives (TAS, CAS, Memory Barriers)](#3-hardware-synchronization-primitives-tas-cas-memory-barriers)
4. [Synchronization Primitives Classification Matrix](#4-synchronization-primitives-classification-matrix)
5. [Condition Variables, Monitors & Spurious Wakeups](#5-condition-variables-monitors--spurious-wakeups)
6. [Priority Inversion & Priority Inheritance Protocol (PIP)](#6-priority-inversion--priority-inheritance-protocol-pip)
7. [Classical Concurrency Problem Formulations](#7-classical-concurrency-problem-formulations)
8. [The ABA Problem & Tagged Pointers](#8-the-aba-problem--tagged-pointers)
9. [Core Theoretical Summary Principles](#9-core-theoretical-summary-principles)

---

# 1. Race Conditions & Critical Section Problem Formulation

A **Race Condition** occurs when multiple concurrent threads access and manipulate shared data, causing the final execution state to depend on non-deterministic thread scheduling and interleaving.

### The 3 Mandatory Critical Section Properties:
```
+---------------------------------------------------------------------------------------------------+
| PROPERTY             | FORMAL THEORETICAL DEFINITION                                              |
+---------------------------------------------------------------------------------------------------+
| 1. Mutual Exclusion  | If process Pi is executing in its critical section, no other process Pj can|
|                      | execute in its critical section simultaneously.                            |
+---------------------------------------------------------------------------------------------------+
| 2. Progress          | If no process is executing in its critical section and some processes wish |
|                      | to enter, only processes not in remainder sections decide entry.           |
+---------------------------------------------------------------------------------------------------+
| 3. Bounded Waiting   | A bound exists on the number of times other processes enter their critical  |
|                      | sections after a process has requested entry, before that request is granted|
+---------------------------------------------------------------------------------------------------+
```

---

# 2. Software Synchronization: Peterson's Algorithm

Designed for two concurrent processes ($P_0$ and $P_1$) executing on sequentially consistent memory models:

```c
bool flag[2] = {false, false}; // flag[i] indicates Pi wants to enter critical section
int turn;                      // Resolves simultaneous entry contention

// Process Pi Entry Protocol:
flag[i] = true;
turn = j; // Yield turn to the alternate process
while (flag[j] && turn == j); // Busy wait

// --- CRITICAL SECTION ---

// Process Pi Exit Protocol:
flag[i] = false;
```

### Correctness Proof:
1. **Mutual Exclusion:** For both $P_0$ and $P_1$ to enter concurrently, `flag[0] == true` and `flag[1] == true`, but `turn` can hold only one scalar value (either 0 or 1), forcing at least one process to stall in its `while` loop.
2. **Progress & Bounded Waiting:** The process that sets `turn` last allows the other process to bypass the condition and enter immediately.

---

# 3. Hardware Synchronization Primitives (TAS, CAS, Memory Barriers)

Modern multi-core architectures feature out-of-order execution, store buffers, and relaxed memory consistency models, requiring hardware-level atomic instructions:

### 1. Compare-And-Swap (CAS) Atomic Semantic
Executes atomically in hardware via cache-line locking (`lock cmpxchg` on x86):
```c
int compare_and_swap(int *ptr, int expected, int new_val) {
    int old_val = *ptr;
    if (old_val == expected) {
        *ptr = new_val;
    }
    return old_val;
}
```

### 2. Memory Barriers (Fences)
Hardware instructions (e.g. `mfence`, `dmb`) enforcing strict ordering constraints across memory load and store operations, preventing compiler and CPU reordering across synchronization boundaries.

---

# 4. Synchronization Primitives Classification Matrix

```
+---------------------------------------------------------------------------------------------------+
| PRIMITIVE            | OWNERSHIP ENFORCEMENT | BLOCKING BEHAVIOR        | PRIMARY APPLICATION     |
+---------------------------------------------------------------------------------------------------+
| Mutex                | YES (Only locking     | Thread sleeps in kernel  | Mutual exclusion on     |
|                      | thread can unlock)    | wait queue (Context sw)  | data structures         |
+---------------------------------------------------------------------------------------------------+
| Counting Semaphore   | NO (Any thread can    | Thread sleeps if count   | Multi-instance resource |
|                      | signal / post)        | drops below zero         | pool management         |
+---------------------------------------------------------------------------------------------------+
| Binary Semaphore     | NO (Signaling token)  | Thread sleeps on wait(S) | Inter-thread event      |
|                      |                       |                          | notification signaling  |
+---------------------------------------------------------------------------------------------------+
| Spinlock             | YES                   | Busy-waits (100% CPU     | Critical sections held  |
|                      |                       | polling in tight loop)   | for < 1-2 us in kernel  |
+---------------------------------------------------------------------------------------------------+
| Futex (Linux)        | Hybrid User/Kernel    | Fast atomic CAS in user  | High-performance lock   |
|                      |                       | space; sleeps on conflict| implementations         |
+---------------------------------------------------------------------------------------------------+
```

### Spinlock Deployment Constraints:
- **Multi-Core Architectures:** Appropriate **only if the expected critical section hold duration is shorter than a context switch overhead** (~2-5 $\mu$s).
- **Single-Core Architectures:** **Never deploy spinlocks without preemption**. The spinning thread monopolizes the uniprocessor core, preventing the lock-holding thread from executing to release the lock.

---

# 5. Condition Variables, Monitors & Spurious Wakeups

A **Condition Variable** provides thread synchronization based on arbitrary application state predicates within a **Monitor**:

```cpp
std::unique_lock<std::mutex> lock(mtx);

// The predicate MUST be evaluated in a while loop:
while (!resource_is_available()) {
    cv.wait(lock); // Atomically releases mtx and blocks; re-acquires mtx on wake
}

// Access shared resource under lock...
```

### Spurious Wakeups & Mesa Monitor Semantics:
1. **Spurious Wakeup:** An OS thread can wake from a blocking wait state in the absence of an explicit signal due to internal kernel signal interruptions or race conditions.
2. **Mesa Monitor Semantics:** Calling `notify()` moves a sleeping thread to the Ready Queue rather than transferring immediate control (Hoare semantics). By the time the thread is dispatched and re-acquires the mutex, another thread may have acquired the resource.
3. **The Invariant:** Predicates associated with condition variables must **always be evaluated inside a `while` loop**.

---

# 6. Priority Inversion & Priority Inheritance Protocol (PIP)

```
Scenario:
Task H (High Priority)      --- Blocked waiting for Mutex held by L ---> (Starved)
Task M (Medium Priority)    ===========================================> (Preempts L)
Task L (Low Priority)       --- Holds Mutex, but preempted by M -------> (Cannot release Mutex)
```

### Priority Inheritance Protocol (PIP):
- When high-priority task $H$ blocks on a synchronization resource held by low-priority task $L$, $L$ **temporarily inherits $H$'s elevated priority level**.
- $L$ executes with $H$'s priority, preventing intermediate task $M$ from preempting $L$.
- Upon releasing the mutex, $L$'s priority drops back to its original baseline, and $H$ immediately preempts and acquires the mutex.

---

# 7. Classical Concurrency Problem Formulations

### A. Producer-Consumer (Bounded Buffer Problem)
- **Primitives:** Counting Semaphore `empty = N`, Counting Semaphore `full = 0`, Mutex `mutex = 1`.

```cpp
void producer() {
    while (true) {
        Item item = produce_item();
        empty.wait();
        mutex.lock();
        buffer.insert(item);
        mutex.unlock();
        full.signal();
    }
}

void consumer() {
    while (true) {
        full.wait();
        mutex.lock();
        Item item = buffer.remove();
        mutex.unlock();
        empty.signal();
        consume_item(item);
    }
}
```

---

### B. Readers-Writers Problem (Reader Preference)
- **Primitives:** Integer `readCount = 0`, Mutex `countMutex = 1`, Semaphore `writeLock = 1`.

```cpp
void reader() {
    countMutex.lock();
    readCount++;
    if (readCount == 1) writeLock.wait(); // First reader acquires write lock
    countMutex.unlock();

    // Concurrent reading occurs...

    countMutex.lock();
    readCount--;
    if (readCount == 0) writeLock.signal(); // Last reader releases write lock
    countMutex.unlock();
}

void writer() {
    writeLock.wait();
    // Exclusive writing occurs...
    writeLock.signal();
}
```

---

### C. Dining Philosophers Problem (Asymmetric Resource Ordering)
- Prevents circular wait by establishing total ordering across resources:

```cpp
void philosopher(int i) {
    int first = std::min(i, (i + 1) % 5);
    int second = std::max(i, (i + 1) % 5);

    while (true) {
        think();
        chopstick[first].lock();  // Acquire lowest indexed chopstick first
        chopstick[second].lock(); // Acquire highest indexed chopstick second
        
        eat();
        
        chopstick[second].unlock();
        chopstick[first].unlock();
    }
}
```

---

# 8. The ABA Problem & Tagged Pointers

The **ABA Problem** occurs in lock-free data structures when a memory location is read as value $A$, modified to $B$, and restored back to $A$. A concurrent CAS operation comparing against $A$ succeeds, failing to observe intermediate structural mutations.

### Solution: Tagged Pointers / Version Counters
Structures combine the pointer address with a monotonically increasing integer version tag into a double-word value:
$$\text{TaggedPointer} = \langle \text{PointerAddress}, \text{VersionTag} \rangle$$
Atomic updates utilize double-width CAS (`cmpxchg16b` on x86_64) to ensure CAS fails if the version tag has incremented.

---

# 9. Core Theoretical Summary Principles

1. **Critical Section Invariants:** Synchronization solutions must strictly satisfy Mutual Exclusion, Progress, and Bounded Waiting.
2. **Mutex vs. Semaphore Ownership:** Mutexes enforce strict thread ownership; Semaphores operate as arbitrary signaling counters.
3. **Mesa Monitor Invariant:** Condition variable predicate checks must be enclosed within `while` loops to guard against spurious wakeups.
4. **Deadlock Elimination in Dining Philosophers:** Enforcing asymmetric lock ordering ($F(R_i) < F(R_{i+1})$) eliminates circular wait dependencies.
