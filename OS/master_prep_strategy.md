# OS Master Prep Strategy — Top Internship Interviews

> **Goal**: Crack OS theory rounds at top IT companies (Google, Microsoft, Amazon, Flipkart, Uber, etc.)
> **Source**: Gap analysis between InterviewBit OS PDF (43 Qs) and what top companies actually ask.

---

## 📋 What the InterviewBit PDF Covers

The PDF covers 43 questions at a **definitional/surface level only**. Good for vocabulary, not for reasoning.

| # | Topic | Questions |
|---|-------|-----------|
| ✅ | OS types, GUI, bootstrap, RTOS | Q1–Q5, Q10 |
| ✅ | IPC mechanisms (pipes, sockets, shared memory) | Q6, Q12, Q27 |
| ✅ | Zombie/orphan, cascading termination | Q28, Q29 |
| ✅ | Semaphore — wait()/signal() only | Q7, Q31 |
| ✅ | Process states (5 states) | Q18 |
| ✅ | Kernel types (monolithic, microkernel) | Q32, Q33, Q34 |
| ✅ | FCFS definition | Q19 |
| ✅ | Scheduling algorithm names only | Q21 |
| ✅ | Deadlock — 4 conditions named only | Q41 |
| ✅ | Belady's Anomaly — definition only | Q42 |
| ✅ | Demand paging, thrashing — surface definitions | Q9, Q23 |
| ✅ | Paging vs. Segmentation — comparison table | Q22 |
| ✅ | RAID levels 0–6 named | Q4 |
| ✅ | Thread vs. Process — comparison table | Q17, Q39 |
| ✅ | Context switching — surface definition | Q37 |
| ✅ | Starvation and Aging | Q30 |
| ✅ | Spooling | Q43 |
| ✅ | Virtual memory — surface definition | Q16 |

**Assessment**: InterviewBit gets you to "can define OS terms" level.
Your notes get you to "can reason about OS trade-offs" level — where top companies actually test you.

---

## 🔴 Critical Gaps — What the PDF Misses (Interview Killers)

These are the topics where saying "I know the definition" will get you **rejected** at top companies.

---

### Gap 1 — Critical Section: The 3 Requirements
**File**: [l4.md](l4.md) — Section 3

The PDF mentions semaphores but **never explains the 3 requirements** for a valid critical section solution. Interviewers at Flipkart, Microsoft, and Uber drill into this.

**What you must know**:
1. **Mutual Exclusion** — At most one process in the critical section at any time.
2. **Progress** — If no process is in CS, the decision of which enters next cannot be postponed indefinitely (deadlock freedom).
3. **Bounded Waiting** — There must be a bound on how many times others can enter CS before a waiting process is allowed in (starvation freedom).

> **Typical Q**: "Peterson's Solution satisfies all 3 requirements. Why doesn't it work on modern hardware?" → Instruction reordering / memory barriers.

---

### Gap 2 — Mutex vs. Semaphore: Ownership
**File**: [l4.md](l4.md) — Section 6 comparison table

The PDF says "mutex = object, semaphore = integer." That is incomplete and misleading for top interviews.

**What you must know**:

| | Mutex | Binary Semaphore | Counting Semaphore |
|-|-------|------------------|--------------------|
| **Ownership** | ✅ Yes — only the locker can unlock | ❌ No | ❌ No |
| **Use** | Protect shared data | Signaling between threads | Manage N resource pool |
| **Can other thread unlock?** | No (undefined behavior / error) | Yes | Yes |

> **Typical Q**: "Can Thread B unlock a mutex that Thread A locked?" → No. "Can Thread B call signal() on a semaphore that Thread A decremented?" → Yes. This is the key distinction.

---

### Gap 3 — Classic Concurrency Problems
**File**: [l4.md](l4.md) — Section 8

The PDF never mentions these. They are the **most commonly asked** concurrency theory questions at top companies.

**What you must know**:

#### Producer-Consumer (Bounded Buffer)
- Producer adds to shared buffer; Consumer removes from it. Buffer has max capacity N.
- Solution uses 3 semaphores: `empty = N`, `full = 0`, `mutex = 1`.
- **Key rule**: `wait(empty)` before `wait(mutex)` in producer. If reversed → deadlock.

#### Readers-Writers
- Multiple readers can read simultaneously; a writer needs exclusive access.
- **Readers-preference**: `readCount` tracks active readers. First reader locks `rw`; last reader unlocks `rw`.
- **Weakness**: Writers can starve if readers keep arriving.

#### Dining Philosophers
- 5 philosophers, 5 chopsticks. Need both left and right to eat.
- **Naive solution → deadlock**: all pick left simultaneously → circular wait.
- **Solutions**: (a) Allow at most 4 philosophers to sit. (b) **Odd-Even rule**: odd picks left then right; even picks right then left. (c) Atomic dual-lock (pick both or neither).

> **Typical Q**: "Why does the naive Dining Philosophers solution deadlock? How do you fix it?" → Circular wait. Fix by breaking symmetric pickup order.

---

### Gap 4 — Deadlock: Prevention, Avoidance, Detection, Recovery
**File**: [l5.md](l5.md) — Sections 3–5

The PDF only names the 4 conditions. It completely skips **how to handle** deadlocks. Banker's Algorithm is standard at FAANG/product companies.

**What you must know**:

#### Prevention (Break a Coffman Condition)
| Condition to Break | How |
|--------------------|-----|
| Mutual Exclusion | Make resources sharable (not always possible) |
| Hold and Wait | Process must request all resources at once before starting |
| No Preemption | Allow OS to forcibly take resources from waiting processes |
| **Circular Wait** | **Impose a global ordering on resource types** (most practical in industry) |

#### Avoidance — Banker's Algorithm
- **Safe State**: A state in which the OS can find a sequence to execute all processes to completion.
- **Unsafe State ≠ Deadlock**, but deadlock *might* occur.
- Banker's checks: `Need[i] = Max[i] - Allocation[i]`. Grants request only if system stays in safe state.

#### Detection & Recovery
- **Detection**: Use Resource Allocation Graph (RAG). A cycle in the RAG = potential deadlock. Run detection algorithm periodically.
- **Recovery**: Terminate one process at a time until deadlock cycle is broken, OR preempt resources from a victim process.

> **Typical Q**: "Is an unsafe state the same as a deadlock?" → No. Unsafe state means deadlock *may* occur; safe state guarantees it won't.

---

### Gap 5 — Page Fault: Step-by-Step Hardware/OS Flow
**File**: [l7.md](l7.md) — Section 2

The PDF says "page fault trap occurs → schedule disk read → restart." Zero mechanism depth.

**What you must know** (trace the full flow):
```
1. CPU references virtual address → MMU checks page table entry (PTE)
2. PTE valid bit = 0 → MMU raises hardware trap (Page Fault Exception)
3. OS page fault handler takes control (runs in kernel mode)
4. Is the address a valid virtual address? If not → segmentation fault (SIGSEGV), kill process
5. Find a free frame in physical memory (or evict a victim page using replacement algorithm)
6. If victim page is dirty (modified) → write it back to disk first
7. Schedule disk I/O to load the required page into the free frame
8. Update page table: set PTE valid bit = 1, frame number = new frame
9. Update TLB with new mapping
10. Restart the faulting instruction (not the next one — the same one)
```

> **Typical Q**: "What happens step-by-step when a page fault occurs?" Trace all 10 steps.

---

### Gap 6 — TLB, Address Translation, and EAT Formula
**File**: [l6.md](l6.md) — Sections 5–6

The PDF never mentions TLB. This is asked at **every** product company that touches systems.

**What you must know**:

#### Address Translation Flow
```
CPU generates virtual address → split into (page number, offset)
→ Check TLB first (hardware cache)
  TLB Hit  → physical frame number found in ~1 cycle → access memory
  TLB Miss → walk page table in RAM → load into TLB → access memory
```

#### Effective Access Time (EAT) Formula
```
EAT = h × (t_tlb + t_mem) + (1 - h) × (t_tlb + t_page_table + t_mem)

where:
  h     = TLB hit rate (e.g. 0.90)
  t_tlb = TLB access time (~1–10 ns)
  t_mem = memory access time (~100 ns)

Example: h=90%, t_tlb=10ns, t_mem=100ns
  EAT = 0.90 × (10+100) + 0.10 × (10+100+100)
      = 0.90 × 110 + 0.10 × 210
      = 99 + 21 = 120 ns
```

#### Page Table Size Formula
```
Page Table Size = (Virtual Address Space / Page Size) × PTE size
                = 2^(m-k) × e bytes
  where m = address bits, k = page size bits, e = PTE size in bytes

Example: 32-bit address, 4KB pages (k=12), 4-byte PTE
  = 2^(32-12) × 4 = 2^20 × 4 = 4 MB page table per process
```

> **Typical Q**: "Why do we need multi-level paging?" → A single-level page table for a 32-bit process = 4 MB in memory per process. With 100 processes = 400 MB just for page tables. Multi-level paging allocates only the portions actually used.

---

### Gap 7 — Context Switch: Exactly What Gets Saved
**File**: [l2.md](l2.md) — Section 4

The PDF says "saves context of one, loads another." Interviewers want specifics.

**What you must know**:

#### What is saved in PCB during Process Context Switch:
- Program Counter (PC) — next instruction to execute
- CPU registers (general-purpose, status flags)
- Stack Pointer
- Memory Management info — **page table base register (CR3 on x86)** → updating this **flushes the entire TLB**
- I/O status (open file descriptors, pending I/O)
- Scheduling info (priority, CPU time used)

#### Why Thread Switch is Faster than Process Switch:
| Step | Process Switch | Thread Switch |
|------|----------------|---------------|
| Save/restore registers | ✅ Yes | ✅ Yes |
| Switch page table (CR3) | ✅ Yes → **TLB flush** | ❌ No — threads share same address space |
| CPU cache warm-up | ❌ Cache goes cold | ✅ Mostly stays warm (shared memory) |
| **Overall cost** | **High** (TLB flush + cache miss storm) | **Low** |

> **Typical Q**: "Why is process context switching more expensive than thread context switching?" → The TLB flush when the page table base register is updated. After a process switch, every memory access is a TLB miss until the TLB is repopulated.

---

### Gap 8 — Page Replacement: Worked Examples
**File**: [l7.md](l7.md) — Sections 4.1–4.4

The PDF never shows an actual page fault trace. OAs will give you a reference string and ask you to solve FIFO/LRU/OPT and count faults.

**What you must be able to solve**:

Reference string: `7 0 1 2 0 3 0 4 2 3 0 3`, 3 frames

| Algorithm | Page Faults | Notes |
|-----------|-------------|-------|
| FIFO | 9 | Subject to Belady's Anomaly |
| OPT | 6 | Best possible (theoretical benchmark) |
| LRU | 8 | No Belady's Anomaly. Near-optimal in practice |

> **Key facts to memorize**:
> - FIFO can suffer Belady's Anomaly (more frames → MORE faults).
> - LRU and OPT are "stack algorithms" — immune to Belady's Anomaly.
> - OPT is unimplementable (requires future knowledge) — used only as a benchmark.

---

## ⏱️ Time-Optimized Reading Plan

**Prerequisite**: Finish InterviewBit PDF first (covers vocabulary and definitions).

| Time | What to Read | File & Section | Why |
|------|-------------|----------------|-----|
| **15 min** | Critical Section (3 requirements) + Mutex vs. Semaphore (ownership) + Classic Problems | [l4.md](l4.md) Sec 3, 6, 8 | Asked in every top company |
| **15 min** | Deadlock Prevention + Avoidance (safe/unsafe state) + Banker's Algorithm | [l5.md](l5.md) Sec 3–5 | Standard at FAANG |
| **15 min** | Page Fault flow (10 steps) + Page Replacement with worked examples + Thrashing depth | [l7.md](l7.md) Sec 2, 4, 6 | Asked at product companies |
| **10 min** | TLB + Address Translation + EAT formula + Multi-level paging | [l6.md](l6.md) Sec 5–6 | OAs have numerical questions |
| **5 min** | Context switch depth (what gets saved, why process > thread) | [l2.md](l2.md) Sec 4 | Classic interview question |
| **10 min** | Read all comparison tables + Top 42 Qs list | [final.md](final.md) | Right before the interview |

**Total extra time**: ~70 minutes on top of InterviewBit PDF.

---

## 🏆 Priority Order: Files to Read

| Priority | File | Read If You Have |
|----------|------|-----------------|
| 🔴 Must | [l4.md](l4.md) — Synchronization | 15 mins |
| 🔴 Must | [l5.md](l5.md) — Deadlocks | 15 mins |
| 🔴 Must | [l7.md](l7.md) — Virtual Memory | 15 mins |
| 🟡 High | [l6.md](l6.md) — Memory Management | 10 mins |
| 🟡 High | [l2.md](l2.md) — Processes & Threads | 5 mins |
| 🟢 Bonus | [l3.md](l3.md) — Scheduling | 10 mins |
| 🟢 Bonus | [l8.md](l8.md) — File Systems & RAID | 10 mins |
| ⚡ Always | [final.md](final.md) — Cheatsheet | 10 mins (before interview) |

---

## 🚀 15-Minute Emergency Cheat Code

If you have literally **15 minutes** before the interview starts:

1. Open [final.md](final.md)
2. Read **Quick Comparison Tables** — process/thread, mutex/semaphore, scheduling, page replacement
3. Read **Critical Definitions** table — know every term cold
4. Read the **Confused Pairs** section at the bottom — these are exact interview trick questions
5. Mentally answer the **Top 42 Questions** — skip any you can answer instantly, slow down on ones that feel fuzzy

---

## ❌ What to Skip Entirely (Low ROI for Top Internships)

| Topic | Why Skip |
|-------|----------|
| GUI definition | Never asked in technical rounds |
| Overlays in OS | Legacy concept, not asked |
| Top 10 OS examples | Trivia, not technical |
| RTOS types (Hard/Firm/Soft) | Only for embedded systems roles |
| Asymmetric clustering | Too niche for general SDE interviews |
| SMP (Symmetric Multiprocessing) | Rarely asked at internship level |
| Reentrancy | Very rarely asked |
