# 90 High-Yield Microsoft Interview Questions — DBMS, OS, OOP in C++

This question sheet contains the real, high-frequency conceptual and scenario questions asked in Microsoft technical rounds (SWE / Intern / SDE). 

Each question here maps directly by number (1 to 30) to its corresponding solution in:
- DBMS Solutions: dbms_solutions.md
- OS Solutions: os_solutions.md
- OOP in C++ Solutions: oops_solutions.md

---

## Part 1: DBMS (30 Questions)

1. How does a database engine physically achieve the ACID properties under the hood?
2. What is Write-Ahead Logging (WAL), and why can't we write modified data pages directly to table files?
3. What is a Checkpoint in database crash recovery, and how does it prevent infinite WAL replay?
4. What is the structural and storage difference between a Clustered Index and a Non-Clustered Index?
5. Why do production databases use B+ Trees instead of B-Trees or Hash Tables for indexing?
6. What is a Covering Index, and how does it eliminate "Bookmark / Key Lookups"?
7. Explain the Leftmost Prefix Rule for composite indexes with a practical query example.
8. If an index exists on `(A, B)`, why can't a query with `WHERE A > 10 AND B = 5` utilize the index on column `B`?
9. Why shouldn't you index every column in a table? What are the write-time and memory penalties?
10. What is the difference between a Dirty Read, a Non-Repeatable (Fuzzy) Read, and a Phantom Read?
11. What are the four ANSI SQL isolation levels, and what concurrency anomalies does each prevent?
12. How does MySQL InnoDB prevent Phantom Reads in Repeatable Read isolation level?
13. What is Multi-Version Concurrency Control (MVCC), and why is it preferred over 2-Phase Locking for reads?
14. What is the difference between Pessimistic Locking (`SELECT ... FOR UPDATE`) and Optimistic Concurrency Control (OCC)?
15. What is Two-Phase Locking (2PL), and how does it fundamentally differ from Two-Phase Commit (2PC)?
16. What is Write Skew, and why doesn't Snapshot Isolation prevent it?
17. Explain 1NF, 2NF, and 3NF with a concrete table decomposition example.
18. What is Boyce-Codd Normal Form (BCNF), and when does a 3NF table fail to satisfy BCNF?
19. When and why would you deliberately Denormalize a database in a high-throughput production system?
20. Explain the three physical JOIN algorithms: Nested Loop Join, Hash Join, and Sort-Merge Join.
21. What is the difference between `DELETE`, `TRUNCATE`, and `DROP` in terms of DDL vs. DML, logging, and rollback?
22. What is the difference between a Primary Key and a Unique Key?
23. What is a View vs. a Materialized View, and when should you use a Materialized View?
24. What is the difference between Database Sharding and Read Replicas?
25. What is Replication Lag in a Primary-Replica architecture, and how do you handle it in application design?
26. How do you choose a good Shard Key, and what happens if you choose poorly (Shard Hotspotting)?
27. When would you choose a NoSQL database over a Relational SQL database?
28. What are the trade-offs between Key-Value, Document, Wide-Column, and Graph databases?
29. What is Eventual Consistency vs. Strong Consistency, and where does the CAP Theorem fit in?
30. How do you detect and resolve a Deadlock in a relational database?

---

## Part 2: Operating Systems (30 Questions)

1. What is the fundamental difference between a Process and a Thread in terms of memory and CPU scheduling?
2. What are the distinct memory segments of a running process from high to low memory addresses?
3. What happens step-by-step during a CPU context switch, and why is a process context switch much more expensive than a thread context switch?
4. What is the difference between User Mode (Ring 3) and Kernel Mode (Ring 0)?
5. How does a System Call work step-by-step from user space invocation to kernel execution and return?
6. What is the fundamental difference between a Mutex and a Counting Semaphore?
7. What is a Spinlock, and when would you choose a Spinlock over a Mutex on a multicore architecture?
8. What is Priority Inversion, and how does the OS resolve it using Priority Inheritance?
9. How do you implement the classic Producer-Consumer problem using Semaphores and a Mutex?
10. What are the four necessary Coffman conditions for Deadlock to occur?
11. How do you prevent Deadlock in production software (Deadlock Prevention vs. Deadlock Avoidance)?
12. What is the difference between Deadlock, Livelock, and Starvation?
13. How does Banker's Algorithm work conceptually to achieve Deadlock Avoidance?
14. How does the Memory Management Unit (MMU) translate a Virtual Address to a Physical Address?
15. What is the Translation Lookaside Buffer (TLB), and what happens on a TLB miss vs. a Page Fault?
16. Walk through the complete lifecycle of a Page Fault from hardware interrupt trap to instruction restart.
17. What is Belady's Anomaly, and why does FIFO page replacement suffer from it while LRU does not?
18. What is Thrashing, what is its root cause, and how can the OS resolve it?
19. What is the difference between Internal Fragmentation and External Fragmentation in memory management?
20. Why do modern operating systems use Multi-Level Page Tables instead of a single flat page table?
21. What are the algorithmic trade-offs between FCFS, Shortest Job First (SJF), and Round Robin CPU scheduling?
22. How does the size of the Round Robin time quantum affect CPU throughput and interactive responsiveness?
23. What is a Multi-Level Feedback Queue (MLFQ) scheduler and why do production OSes like Windows use it?
24. What are the different Inter-Process Communication (IPC) mechanisms, and which is the fastest?
25. What is a Zombie Process, what is an Orphan Process, and how is each handled by the OS?
26. What is a Hardware Interrupt vs. a Software Trap?
27. What is an atomic operation, and how does Compare-And-Swap (CAS) enable lock-free concurrency?
28. What is False Sharing in a multicore CPU cache architecture, and how do you prevent it?
29. What is the difference between a Preemptive Kernel and a Non-Preemptive Kernel?
30. What is Memory-Mapped I/O (`mmap`), and why is it faster than standard `read`/`write` system calls?

---

## Part 3: OOP in C++ (30 Questions)

1. What are the four pillars of OOP, and what is the exact difference between Encapsulation and Abstraction?
2. How is dynamic polymorphism implemented under the hood in C++ using `vtable` and `vptr`?
3. Why MUST a base class destructor be declared `virtual` when deleting an object through a base pointer?
4. Can a constructor be `virtual` in C++? Can a destructor be pure virtual?
5. What is C++ Name Hiding, and how do you bring base class overloads into derived scope?
6. Can you overload a function based on return type alone? Why or why not?
7. What is a Covariant Return Type in C++, and when is it useful?
8. What is the Diamond Problem in multiple inheritance, and how does C++ solve it using `virtual` inheritance?
9. How is a pure virtual interface created in C++, and what happens if a derived class does not override all pure virtual functions?
10. What is Object Slicing in C++, and how do you prevent it?
11. Explain the Liskov Substitution Principle (LSP) and why inheriting `Square` from `Rectangle` violates it.
12. What is the Single Responsibility Principle (SRP), and how do you identify a code smell violating it?
13. What is the Open/Closed Principle (OCP), and how do interfaces and polymorphism achieve it?
14. What is the Interface Segregation Principle (ISP) vs. Dependency Inversion Principle (DIP)?
15. How do you implement a thread-safe Singleton in modern C++ (Meyers' Singleton vs. Double-Checked Locking)?
16. What is the difference between the Factory Method pattern and the Abstract Factory pattern?
17. What is the Observer pattern, and how do you prevent memory leaks / dangling pointers using `std::weak_ptr`?
18. What is the Strategy pattern, and when would you use it over a large `switch-case` block?
19. What is the Decorator pattern, and how does it avoid subclass explosion?
20. What is the difference between Composition and Inheritance ("is-a" vs. "has-a"), and why is composition favored?
21. What is the difference between Public, Protected, and Private inheritance in C++?
22. What is the difference between a Shallow Copy and a Deep Copy, and why does a default shallow copy cause double-free crashes?
23. What is the Rule of 3, the Rule of 5, and the Rule of 0 in modern C++?
24. What is the Copy-and-Swap idiom in C++, and why does it provide strong exception safety?
25. What is RAII (Resource Acquisition Is Initialization), and how does it guarantee resource cleanup in the presence of exceptions?
26. What is the difference between `std::unique_ptr`, `std::shared_ptr`, and `std::weak_ptr`?
27. How does reference counting work in `std::shared_ptr`, and how do circular references cause memory leaks?
28. What is the difference between `delete` and `delete[]` in C++?
29. Can static member functions be `virtual` or access non-static member variables in C++?
30. What is the `friend` keyword in C++, and does it violate encapsulation?

---

## How to Practice
- Pick 5 questions at random every morning.
- Time yourself: explain your answer out loud in 60 to 90 seconds without looking at the solution files.
- Verify your answer against the corresponding solution file (`dbms_solutions.md`, `os_solutions.md`, `oops_solutions.md`).