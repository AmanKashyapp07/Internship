# Memory Management, Paging & Virtual Memory Architecture

> **Scope:** Logical vs. Physical Address Space, Memory Management Unit (MMU), Contiguous Allocation & Fragmentation, Paging Hardware Architecture, Page Table Entry (PTE) Flags, Page Table Sizing Mathematics, Multi-Level & Inverted Page Tables, Translation Lookaside Buffer (TLB), Effective Memory Access Time (EMAT), and Segmentation vs. Paging Comparison.

---

# Table of Contents
1. [Logical vs. Physical Address Translation](#1-logical-vs-physical-address-translation)
2. [Contiguous Allocation & Memory Fragmentation](#2-contiguous-allocation--memory-fragmentation)
3. [Paging Hardware Architecture & Page Table Entries](#3-paging-hardware-architecture--page-table-entries)
4. [Page Table Sizing & Mathematical Formulation](#4-page-table-sizing--mathematical-formulation)
5. [Multi-Level Hierarchical & Inverted Page Tables](#5-multi-level-hierarchical--inverted-page-tables)
6. [Translation Lookaside Buffer (TLB) & EMAT Formulations](#6-translation-lookaside-buffer-tlb--emat-formulations)
7. [Segmentation vs. Paging Architectural Analysis](#7-segmentation-vs-paging-architectural-analysis)
8. [Core Theoretical Summary Principles](#8-core-theoretical-summary-principles)

---

# 1. Logical vs. Physical Address Translation

In modern operating systems, programs execute within an isolated **Logical (Virtual) Address Space**. The hardware **Memory Management Unit (MMU)** translates every CPU-generated virtual address into a physical hardware RAM address at runtime.

```
+------------------+         Logical Address (32/64-bit)         +--------------------+         Physical Address         +---------------------+
|    CPU Core      | ------------------------------------------> |    Memory Mgt Unit | -------------------------------> |   Physical RAM      |
| (Executes Code)  |                                             |     (MMU / TLB)    |                                  |   (DRAM Frames)     |
+------------------+                                             +--------------------+                                  +---------------------+
```

### Address Binding Stages:
1. **Compile-Time:** Generates absolute physical addresses. Requires program to always load at an invariant physical location.
2. **Load-Time:** Generates relocatable code with base-relative offsets; addresses are rebound by the loader upon memory allocation.
3. **Execution-Time:** Dynamic address binding via the MMU using base/limit registers and page tables. Permits processes to move across physical frames during execution.

---

# 2. Contiguous Allocation & Memory Fragmentation

```
Internal Fragmentation (Within Fixed Block)         External Fragmentation (Scattered Disjoint Gaps)
+--------------------------------------------+      +--------+----------------+--------+----------------+
| Allocated Page Frame: 4096 Bytes           |      | Used   | Free (100 KB)  | Used   | Free (150 KB)  |
| [ Process Data: 2500 Bytes ] [ Wasted: 1596]|      +--------+----------------+--------+----------------+
+--------------------------------------------+      Total Free = 250 KB; Cannot satisfy contiguous 200 KB request!
```

```
+---------------------------------------------------------------------------------------------------+
| FRAGMENTATION TYPE       | CAUSE                                      | MITIGATION STRATEGY       |
+---------------------------------------------------------------------------------------------------+
| Internal Fragmentation   | Fixed-size allocation (Paging). Payload is | Finer page granulariies   |
|                          | smaller than allocated page boundary.      | (e.g. 4KB vs HugePages)   |
+---------------------------------------------------------------------------------------------------+
| External Fragmentation   | Variable-size allocation (Segmentation).   | Dynamic compaction or     |
|                          | Free memory scattered in disjoint blocks.  | Non-contiguous Paging     |
+---------------------------------------------------------------------------------------------------+
```

### Contiguous Dynamic Placement Strategies:
- **First Fit:** Allocates the first free memory block satisfying size requirements (Fastest search time).
- **Best Fit:** Allocates the smallest free block satisfying size requirements (Minimizes leftover gap size, produces unusable fragments).
- **Worst Fit:** Allocates the largest free block available (Leaves the largest contiguous remaining fragment).

---

# 3. Paging Hardware Architecture & Page Table Entries

Paging divides logical memory into fixed-sized blocks called **Pages** and physical memory into matching blocks called **Frames**.

```
Logical Address: [ Page Number (p) : 20 bits ] [ Offset (d) : 12 bits ] (4 KB Page Size)
                              |                                |
                              v                                |
                 +--------------------------+                  |
                 | Page Table (Indexed by p)|                  |
                 |--------------------------|                  |
                 | ...                      |                  |
                 | Entry p: Frame Number (f)|                  |
                 +--------------------------+                  |
                              |                                |
                              v                                v
Physical Address: [ Frame Number (f) : 20 bits ] [ Offset (d) : 12 bits ]
```

### Page Table Entry (PTE) Flags & Layout:
```
+-------+--------+-----+-----+-------+--------+------------+------------------------+
| Dirty | Access | U/S | R/W | P/V   | Cache  | Global (G) | Physical Frame Number  |
|  (D)  |  (A)   |     |     |Present|Disable | (No Flush) |      (PFN / PPN)       |
+-------+--------+-----+-----+-------+--------+------------+------------------------+
```
- **Present / Valid Bit (P):** `1` = frame is resident in physical DRAM; `0` = frame is on backing storage (Page Fault trigger).
- **Read/Write Bit (R/W):** `0` = Read-only access (`.text`); `1` = Read-write access (`.data`, heap, stack).
- **User/Supervisor Bit (U/S):** `0` = Ring 0 kernel mode only; `1` = Ring 3 user mode accessible.
- **Dirty Bit (D):** Asserted by hardware on write access. Dictates whether the page must be written to disk on eviction.
- **Accessed / Referenced Bit (A):** Asserted by hardware on read/write access. Used by page replacement algorithms.

---

# 4. Page Table Sizing & Mathematical Formulation

### Core Paging Formulas:
$$\text{Page Size} = 2^d \text{ bytes} \implies d \text{ offset bits}$$
$$\text{Number of Pages} = \frac{\text{Virtual Address Space Size}}{\text{Page Size}} = \frac{2^V}{2^d} = 2^{V - d}$$
$$\text{Page Table Size} = (\text{Number of Pages}) \times (\text{PTE Size in Bytes})$$

### Single-Level Mathematical Evaluation:
- **Given:** 32-bit Virtual Address Space ($V = 32$), 4 KB Page Size ($2^{12}$ bytes), 4-byte Page Table Entry (PTE).
- **Calculations:**
  1. Offset bits $d = \log_2(4 \text{ KB}) = 12 \text{ bits}$.
  2. Page Number bits $p = 32 - 12 = 20 \text{ bits}$.
  3. Total Pages $= 2^{20} = 1,048,576 \text{ pages}$.
  4. Linear Page Table Size $= 2^{20} \times 4 \text{ bytes} = 4 \text{ MB per process}$.
- **Scaling Limit:** In a 64-bit address space, a flat single-level page table requires $2^{52} \times 8 \text{ bytes} \approx 36 \text{ Petabytes}$ of overhead per process, necessitating hierarchical multi-level paging.

---

# 5. Multi-Level Hierarchical & Inverted Page Tables

```
Two-Level Hierarchical Paging:
Logical Address: [ Outer Page (p1) : 10 bits ] [ Inner Page (p2) : 10 bits ] [ Offset (d) : 12 bits ]
                            |                                |                              |
                            v                                v                              |
                   +------------------+             +------------------+                    |
                   | Outer Page Table | ----------> | Inner Page Table |                    |
                   | (Points to inner)|             | (Points to Frame)|                    |
                   +------------------+             +------------------+                    |
                                                              |                             |
                                                              v                             v
                                            Physical Address: [ Frame Number (f) ] [ Offset (d) ]
```

### 1. Multi-Level Page Tables (Radix Tree Structure)
- **Sparse Allocation Advantage:** Unallocated or sparse virtual memory regions omit inner page tables entirely. A process utilizing 2 MB of heap/stack allocates only the top-level directory and one inner table (8 KB total vs. 4 MB flat).
- **Latency Tradeoff:** A $k$-level page table incurs $k$ sequential physical memory accesses per address translation on a TLB miss.

### 2. Inverted Page Tables
- Contains **one entry per physical memory frame** rather than one entry per virtual page.
- Each entry stores `(Process ID, Virtual Page Number)`.
- **Advantage:** Table size is bounded strictly by physical DRAM size, independent of active process counts.
- **Disadvantage:** Lookups require associative searches or hash tables, complicating page table traversals.

---

# 6. Translation Lookaside Buffer (TLB) & EMAT Formulations

The **Translation Lookaside Buffer (TLB)** is an on-chip, fully-associative hardware cache storing recent `Virtual Page Number -> Physical Frame Number` mappings.

```
Logical Address ---> [ Hardware TLB Lookup (~1ns) ]
                           |
            +--------------+--------------+
            |                             |
      (TLB HIT: ~95-99%)           (TLB MISS: ~1-5%)
            |                             |
            v                             v
   [ Return Frame directly ]     [ Walk Page Table in RAM (100ns) ]
            |                             |
            |                             v
            |                   [ Load Entry into TLB ]
            |                             |
            +--------------+--------------+
                           |
                           v
              [ Access Physical Memory Frame ]
```

### Effective Memory Access Time (EMAT) Equation:
For hit ratio $h$, TLB search latency $t_{\text{tlb}}$, physical DRAM access latency $t_{\text{mem}}$, and $k$-level page tables:

$$\text{EMAT} = h \times (t_{\text{tlb}} + t_{\text{mem}}) + (1 - h) \times (t_{\text{tlb}} + (k \times t_{\text{mem}}) + t_{\text{mem}})$$

### Mathematical Evaluation:
- **Given:** $h = 95\%$, $t_{\text{tlb}} = 1 \text{ns}$, $t_{\text{mem}} = 100 \text{ns}$, 2-Level Paging ($k = 2$).
- **EMAT Calculation:**
  $$\text{EMAT} = 0.95 \times (1 + 100) + 0.05 \times (1 + 200 + 100) = 0.95 \times 101 + 0.05 \times 301 = 95.95 + 15.05 = \mathbf{111.0 \text{ns}}$$

---

# 7. Segmentation vs. Paging Architectural Analysis

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | PAGING                                | SEGMENTATION                       |
+---------------------------------------------------------------------------------------------------+
| Allocation Unit      | Fixed size (e.g. 4 KB, 2 MB, 1 GB)   | Variable size (Logical code blocks)|
| Hardware / OS Role   | Hardware-enforced MMU translation     | Compiler/programmer defined bounds |
| Address Division     | Page Number + Offset $[p : d]$        | Segment Number + Offset $[s : d]$  |
| Fragmentation        | Internal fragmentation only           | External fragmentation only        |
| Translation Engine   | Page Table (Indexed by Page #)        | Segment Table (Base + Limit check) |
| Protection Model     | Page-level permission bits (R/W/X)    | Module-level access rights         |
+---------------------------------------------------------------------------------------------------+
```

---

# 8. Core Theoretical Summary Principles

1. **Address Translation Invariant:** The offset component ($d$) is passed through directly without translation; only the page number ($p$) is mapped to a frame number ($f$).
2. **Internal vs. External Fragmentation:** Paging completely eliminates external fragmentation by utilizing uniform fixed frames, incurring minimal internal fragmentation on trailing page boundaries.
3. **Multi-Level Paging Optimization:** Radix-tree multi-level page tables eliminate memory overhead for unallocated address regions at the cost of multiple memory lookups per TLB miss.
4. **TLB Performance Impact:** Effective memory access latency is dominated by the TLB hit ratio ($h$), reducing multi-level translation latency to near on-chip cache speeds.
