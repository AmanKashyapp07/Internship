# DBMS Master Prep Strategy — Top IT Internship Interviews
> **Goal**: Balance DBMS preparation with Data Structures & Algorithms (DSA). Learn what to study now for Online Assessments (OAs), and what to save for face-to-face technical interviews.

---

## ⚡ High-Level Summary: Pre-OA vs. Post-OA

```mermaid
graph TD
    A[DBMS Prep Strategy] --> B[🔴 Phase 1: Pre-OA (Focus: Coding & Math MCQs)]
    A --> C[🔵 Phase 2: Post-OA (Focus: Theory & System Internals)]
    B --> D(SQL Queries, Normalization Keys/FDs, Transactions Serializability)
    C --> E(Keys & ER Models, Indexing & B+ Trees, Concurrency & Recovery)
```

---

## 🔴 Phase 1: Pre-OA Study Plan (High Priority — Study Now)
* **Objective**: Clear MCQ-based tests, database math questions, and SQL coding challenges.
* **DSA Balance**: Spend $75\%$ of your time on DSA, and use these targeted files to cover $90\%$ of OA-relevant DBMS questions in the remaining $25\%$ of your time.

### 1. SQL Coding Preparation (Highest ROI)
OAs frequently include 1 or 2 hands-on SQL coding questions.
* **Files to read**:
  - **[l10.md (SQL Problem Patterns)](file:///Users/amankashyap/Documents/internship/DBMS/l10.md)**: Practice high-frequency query templates (Nth highest salary, running totals, consecutive logins, Top-N per group).
  - **[l8.md (Advanced SQL)](file:///Users/amankashyap/Documents/internship/DBMS/l8.md)**: Study window functions (`ROW_NUMBER`, `RANK`, `DENSE_RANK`), CTEs, and row pagination.
  - **[cheatsheet.md (SQL Cheatsheet)](file:///Users/amankashyap/Documents/internship/DBMS/cheatsheet.md)**: Review logical query execution order (e.g. `FROM` runs before `WHERE` which runs before `SELECT`).

### 2. Functional Dependencies & Normalization (MCQs)
Normalization problems are standard in written tests. You will be given Functional Dependencies (FDs) and asked to find keys or normal forms.
* **File to read**:
  - **[l4.md (Normalization)](file:///Users/amankashyap/Documents/internship/DBMS/l4.md)**: Learn the **Attribute Closure** algorithm (used to find Candidate Keys), and memorize rules for **1NF, 2NF, 3NF, and BCNF**.

### 3. Transactions & Serializability (MCQs)
Written tests often ask you to determine if a transaction schedule is conflict serializable.
* **File to read**:
  - **[l5.md (Transactions & ACID)](file:///Users/amankashyap/Documents/internship/DBMS/l5.md)**: Practice drawing **Precedence Graphs** to check for conflict serializability (cycle in graph = not serializable).

---

## ⏱️ Phase 2: Post-OA / 1–2 Days Before Interview (Low Priority — Read Later)
* **Objective**: Revise definitions, system internals, structures, and explain trade-offs to a human interviewer.
* **DSA Balance**: Since OAs are cleared, you can shift some focus to revision here.

### 1. Indexing & B+ Trees (First Day of Revision)
Interviewers love asking *why* databases use B+ Trees.
* **File to read**:
  - **[l7.md (Indexing & Query Processing)](file:///Users/amankashyap/Documents/internship/DBMS/l7.md)**: Revise B+ Tree vs. B-Tree structure, dense vs. sparse indexes, and clustered vs. non-clustered indexes.

### 2. Concurrency Anomalies & Locking (First Day of Revision)
* **File to read**:
  - **[l6.md (Concurrency Control)](file:///Users/amankashyap/Documents/internship/DBMS/l6.md)**: Revise Concurrency anomalies (Dirty reads, Non-repeatable reads, Phantom reads), and the **Two-Phase Locking (2PL)** protocol.

### 3. Recovery, WAL & Buffer Pool (Second Day of Revision)
* **File to read**:
  - **[l9.md (Recovery & Internals)](file:///Users/amankashyap/Documents/internship/DBMS/l9.md)**: Revise Write-Ahead Logging (WAL) rules, buffer pool cache eviction, and the three-schema architecture.

### 4. Keys, ER Models, & General Vocabulary (Quick Skim)
* **Files to read**:
  - **[l1.md (Relational Model & Keys)](file:///Users/amankashyap/Documents/internship/DBMS/l1.md)**: Skim the definitions of candidate/super keys and NULL semantics.
  - **[l2.md (ER Modeling)](file:///Users/amankashyap/Documents/internship/DBMS/l2.md)**: Skim weak entity sets and cardinality mapping constraints.

### 5. Final Cheat-Sheet Review (Last 2 Hours)
* **File to read**:
  - **[final.md (Master Summary)](file:///Users/amankashyap/Documents/internship/DBMS/final.md)**: Review comparison tables, transaction states, indexing rules, and the "Common Traps" section right before the interview begins.
