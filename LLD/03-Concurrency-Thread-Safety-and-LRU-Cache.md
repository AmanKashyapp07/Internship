# 03. Concurrency, Thread-Safety & In-Memory Mechanics

> **Interview Mindset**: Writing a single-threaded class is trivial. An interviewer asking a Low-Level Design (LLD) problem almost always asks: *"How would you make this thread-safe?"* or *"What happens if 100 concurrent threads invoke `bookTicket()` simultaneously?"* 
> Understanding C++ synchronization primitives (`std::mutex`, `std::unique_lock`, `std::condition_variable`, `std::shared_mutex`), race conditions, and thread-safe data structures is what elevates an intern from a junior coder to a hireable backend engineer.

---

## 1. Concurrency Core Concepts in OOP

```
┌─────────────────────────────────────────────────────────────┐
│               Shared BankAccount State: $100                │
└──────────────┬───────────────────────────────┬──────────────┘
               │                               │
     Reads 100 │                               │ Reads 100
               ▼                               ▼
   ┌───────────────────────┐       ┌───────────────────────┐
   │  Thread 1 (Withdrawal)│       │  Thread 2 (Withdrawal)│
   │  Calculates: 100 - 40 │       │  Calculates: 100 - 30 │
   └───────────┬───────────┘       └───────────┬───────────┘
               │                               │
               │ Writes $60                    │ Writes $70
               ▼                               ▼
┌─────────────────────────────────────────────────────────────┐
│ 💥 Race Condition: Final balance becomes $60 or $70 instead │
│                   of the correct $30!                       │
└─────────────────────────────────────────────────────────────┘
```

### The 3 Big Concurrency Hazards

| Hazard | Real-World Scenario | Defensive Solution in C++ |
| :--- | :--- | :--- |
| **Race Condition** | Two threads read `count = 5` simultaneously, increment to `6`, and write back. Total increment is 1 instead of 2. | `std::atomic<int>`, `std::mutex`, or `std::lock_guard` |
| **Deadlock** | Thread A locks Mutex 1 and waits for Mutex 2. Thread B locks Mutex 2 and waits for Mutex 1. Both freeze forever. | Strict **Lock Ordering** or `std::lock(m1, m2)` / `std::scoped_lock` |
| **Starvation** | Low-priority thread continuously yields to high-priority threads and never gets CPU time. | Fair scheduling / FIFO queue mechanisms |

---

## 2. Synchronization Mechanisms in C++ Compared

```
                ┌── 1. std::lock_guard (Simple RAII scoped lock)
C++ Locks       ┼── 2. std::unique_lock (Flexible: supports deferred locking, timeouts, condition_variable)
Mechanisms      ├── 3. std::shared_mutex (C++17: Multiple readers via std::shared_lock, single writer)
                └── 4. std::atomic<T> (Lock-free hardware atomic instructions via CAS)
```

### 1. `std::lock_guard` vs `std::unique_lock`
- `std::lock_guard`: Lightweight, strictly scoped RAII lock. Acquires on construction, releases on destruction. Cannot be manually unlocked.
- `std::unique_lock`: Supports deferred locking, early manual unlock, and works with `std::condition_variable`.

### 2. `std::shared_mutex` (Crucial for In-Memory Caches)
- In read-heavy systems (90% reads, 10% writes), normal mutexes bottleneck throughput because readers unnecessarily block other readers.
- **Rule**:
  - Multiple threads acquire a read lock via `std::shared_lock<std::shared_mutex>`.
  - Only one thread acquires an exclusive write lock via `std::unique_lock<std::shared_mutex>` (blocks all readers and writers).

---

## 3. The Producer-Consumer Pattern (Bounded Buffer)

One of the most frequently asked concurrency building blocks:

```
┌───────────────────────┐      push()      ┌─────────────────────────────┐      pop()       ┌───────────────────────┐
│   Producer Thread 1   ├─────────────────►│                             ├─────────────────►│   Consumer Thread 1   │
└───────────────────────┘                  │    Bounded BlockingQueue    │                  └───────────────────────┘
                                           │       (Max 5 Items)         │
┌───────────────────────┐      push()      │                             │      pop()       ┌───────────────────────┐
│   Producer Thread 2   ├─────────────────►│                             ├─────────────────►│   Consumer Thread 2   │
└───────────────────────┘                  └─────────────────────────────┘                  └───────────────────────┘
```

```cpp
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class BlockingQueue {
private:
    std::queue<T> queue;
    size_t capacity;
    std::mutex mtx;
    std::condition_variable notFull;
    std::condition_variable notEmpty;

public:
    explicit BlockingQueue(size_t cap) : capacity(cap) {}

    // Producer pushes item
    void push(T item) {
        std::unique_lock<std::mutex> lock(mtx);
        notFull.wait(lock, [this]() { return queue.size() < capacity; });

        queue.push(std::move(item));
        notEmpty.notify_one(); // Wake up one waiting consumer
    }

    // Consumer pops item
    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        notEmpty.wait(lock, [this]() { return !queue.empty(); });

        T item = std::move(queue.front());
        queue.pop();
        notFull.notify_one(); // Wake up one waiting producer
        return item;
    }
};
```

---

## 4. The #1 LLD Interview Problem: Thread-Safe LRU Cache

> [!IMPORTANT]
> **Interview Requirement**: Design a cache of capacity $C$ that supports:
> 1. `get(key)`: Returns value in $O(1)$ time and marks key as most recently used.
> 2. `put(key, value)`: Updates or inserts in $O(1)$. If capacity is exceeded, evicts the **Least Recently Used (LRU)** item.
> 3. **Thread-Safe**: Must handle concurrent access safely using modern C++.

### The Architecture: HashMap + Doubly Linked List

```
┌──────────────────┐       ┌──────────────────┐       ┌──────────────────┐       ┌──────────────────┐
│    Dummy HEAD    │◄─────►│   Node (Key: 1)  │◄─────►│   Node (Key: 2)  │◄─────►│    Dummy TAIL    │
│  (Most Recent)   │       │   Value: "Apple" │       │   Value: "Mango" │       │  (Least Recent)  │
└──────────────────┘       └──────────────────┘       └──────────────────┘       └──────────────────┘
                                                                                          │
                                                                                          ▼
                                                                                   Evicted on Put()
                                                                                    when cache full
```

- **`std::unordered_map<K, Node*>`**: Provides $O(1)$ lookup for existing nodes.
- **Doubly Linked List**:
  - Adding to Head: $O(1)$.
  - Removing any given Node: $O(1)$ (each node has `prev` and `next` pointers).
  - Removing from Tail (Eviction): $O(1)$.

---

### Complete Production-Ready C++ Implementation

```cpp
#include <iostream>
#include <unordered_map>
#include <mutex>

template <typename K, typename V>
class ThreadSafeLRUCache {
private:
    struct Node {
        K key;
        V value;
        Node* prev{nullptr};
        Node* next{nullptr};
        Node(K k, V v) : key(k), value(v) {}
    };

    size_t capacity;
    std::unordered_map<K, Node*> cacheMap;
    Node* head; // Dummy head
    Node* tail; // Dummy tail
    mutable std::mutex mtx; // Protects list mutations & map

    // Helper: Add node right after head (most recently used)
    void addToHead(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    // Helper: Unlink node from its current position
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // Helper: Promote existing node to most recently used
    void moveToHead(Node* node) {
        removeNode(node);
        addToHead(node);
    }

    // Helper: Evict least recently used node from tail
    Node* removeTail() {
        Node* res = tail->prev;
        removeNode(res);
        return res;
    }

public:
    explicit ThreadSafeLRUCache(size_t cap) : capacity(cap) {
        head = new Node(K{}, V{});
        tail = new Node(K{}, V{});
        head->next = tail;
        tail->prev = head;
    }

    ~ThreadSafeLRUCache() {
        std::lock_guard<std::mutex> lock(mtx);
        Node* curr = head;
        while (curr) {
            Node* nxt = curr->next;
            delete curr;
            curr = nxt;
        }
    }

    // Disallow copy operations
    ThreadSafeLRUCache(const ThreadSafeLRUCache&) = delete;
    ThreadSafeLRUCache& operator=(const ThreadSafeLRUCache&) = delete;

    // O(1) Get Operation
    bool get(const K& key, V& valueOut) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = cacheMap.find(key);
        if (it == cacheMap.end()) {
            return false;
        }

        Node* node = it->second;
        moveToHead(node);
        valueOut = node->value;
        return true;
    }

    // O(1) Put Operation
    void put(const K& key, const V& value) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = cacheMap.find(key);

        if (it != cacheMap.end()) {
            Node* node = it->second;
            node->value = value;
            moveToHead(node);
        } else {
            Node* newNode = new Node(key, value);
            cacheMap[key] = newNode;
            addToHead(newNode);

            if (cacheMap.size() > capacity) {
                Node* lru = removeTail();
                cacheMap.erase(lru->key);
                delete lru; // Free memory!
            }
        }
    }
};
```

---

## 5. Concurrency Interview Cheat Sheet (C++ Edition)

| Question | Senior Answer |
| :--- | :--- |
| *"Why does `get()` in an LRU Cache require a lock?"* | *"Even though `get()` is a read operation for the value, in an LRU cache it mutates the internal Doubly Linked List pointers to promote the node to the head. Concurrent pointer mutations without locks cause race conditions and dangling pointers."* |
| *"What is the difference between `volatile` in C++ vs Java?"* | *"In Java, `volatile` provides memory visibility guarantees for multithreading. In C++, `volatile` only tells the compiler not to optimize away reads/writes (used for hardware/memory-mapped I/O); it does NOT provide thread safety or memory barriers! In C++, you must use `std::atomic`."* |
| *"How do you prevent deadlocks when acquiring two locks?"* | *"Use `std::scoped_lock lock(mtx1, mtx2)` (C++17), which applies a deadlock-avoidance algorithm (similar to lock ordering) to acquire all mutexes atomically without deadlocking."* |
