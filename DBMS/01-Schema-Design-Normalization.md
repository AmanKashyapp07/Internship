# Master Guide 01: Schema Design & Normalization

> **Focus:** Relational Database Keys Matrix, ER Modeling & Cardinality, 1NF to BCNF Normalization Ladder (Single Running Example), Functional Dependencies, and Denormalization Trade-Offs.
> 
> *Targeted for Top-Tier Tech System & Backend Engineering Interviews.*

---

# Table of Contents
1. [The Database Keys Comparison Matrix](#1-the-database-keys-comparison-matrix)
2. [Entity-Relationship (ER) Modeling & Cardinality](#2-entity-relationship-er-modeling--cardinality)
3. [The Normalization Ladder (1NF to BCNF with One Running Example)](#3-the-normalization-ladder-1nf-to-bcnf-with-one-running-example)
4. [Denormalization: When & Why to Break the Rules](#4-denormalization-when--why-to-break-the-rules)
5. [High-Frequency Interview Drill & Verbal Q&A](#5-high-frequency-interview-drill--verbal-qa)

---

# 1. The Database Keys Comparison Matrix

```
+---------------------------------------------------------------------------------------------------+
| KEY TYPE             | DEFINITION & PROPERTIES                     | DATABASE EXAMPLE / ROLE      |
+---------------------------------------------------------------------------------------------------+
| Super Key            | Any set of columns that uniquely identifies | `{email, name}`, `{ssn, age}`|
|                      | a row (may contain redundant attributes)    | (Superset of Candidate Key)  |
+---------------------------------------------------------------------------------------------------+
| Candidate Key        | A MINIMAL Super Key with zero redundant     | `{id}`, `{email}`, `{ssn}`   |
|                      | attributes                                  | (Eligible to become Primary) |
+---------------------------------------------------------------------------------------------------+
| Primary Key (PK)     | The chosen candidate key; uniquely identifies| `user_id`                    |
|                      | rows; strictly `UNIQUE` and `NOT NULL`      | (Forms Clustered Index)      |
+---------------------------------------------------------------------------------------------------+
| Foreign Key (FK)     | Column referencing a Primary Key in another | `user_id` in `orders` table  |
|                      | table; enforces Referential Integrity       | (Requires index for joins)   |
+---------------------------------------------------------------------------------------------------+
| Composite Key        | Primary Key composed of 2+ columns          | `{student_id, course_id}`    |
|                      | (Used in Many-to-Many junction tables)      | in `enrollments` table       |
+---------------------------------------------------------------------------------------------------+
| Surrogate Key        | System-generated artificial identifier with | Auto-increment `BIGINT` or   |
|                      | no real-world business meaning              | `UUIDv7` (time-ordered)      |
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** A Candidate Key is an applicant qualified for the job; the Primary Key is the single candidate you hire.
- **The Interview Trap:** Using random UUIDv4 as a Primary Key in B+ Tree storage engines (InnoDB). UUIDv4 random inserts cause severe **page splits, low leaf fill factors (~50%), and massive disk I/O**. Always prefer time-ordered `BIGINT AUTO_INCREMENT` or `UUIDv7`.
- **30-Second Verbal Answer:** **"A Super Key is any attribute set uniquely identifying a tuple; a Candidate Key is a minimal super key with no redundant columns. The Primary Key is the chosen candidate key that enforces uniqueness and non-nullability, often defining the clustered index physical storage layout. Foreign Keys maintain referential integrity across relational boundaries."**

---

# 2. Entity-Relationship (ER) Modeling & Cardinality

```
1:1 (One-to-One):          [ User ] ---------------------------- (1:1) -------------------------> [ UserProfile ]
                           (Store FK in UserProfile with UNIQUE constraint, or merge into 1 table)

1:N (One-to-Many):         [ Customer ] ------------------------ (1:N) -------------------------> [ Orders ]
                           (Store customer_id as Foreign Key in Orders table)

M:N (Many-to-Many):        [ Students ] <---> [ StudentCourses (Junction Table) ] <---> [ Courses ]
                           (Requires Junction/Bridge table with Composite PK {student_id, course_id})
```

### Weak Entities & Identifying Relationships:
- **Weak Entity:** An entity that cannot be uniquely identified by its own attributes alone and depends on the existence of an owner entity (e.g. `Dependent` table dependent on `Employee`).
- **Identifying Relationship:** The primary key of the parent entity is included as part of the composite primary key of the weak entity.

---

# 3. The Normalization Ladder (1NF to BCNF with One Running Example)

### The Un-Normalized Initial State (0NF / Table with Anomalies):
```
Table: CourseEnrollments (UN-NORMALIZED)
+------------+-------------+----------------------+--------------------+-------------+
| student_id | student_name| courses_enrolled     | instructor_name    | dept_office |
+------------+-------------+----------------------+--------------------+-------------+
| 101        | Alice       | CS101, MATH201       | Dr. Smith, Dr. Fox | Hall-204    |
| 102        | Bob         | CS101                | Dr. Smith          | Hall-204    |
+------------+-------------+----------------------+--------------------+-------------+
Anomalies: Multi-valued arrays, duplicate instructor locations, update anomalies if Dr. Smith moves office.
```

---

### Step 1: First Normal Form (1NF) — Atomic Values
> **Rule:** Every column must contain atomic (indivisible) scalar values. No repeating groups or arrays.

```
Table: Enrollments_1NF (Composite PK: {student_id, course_id})
+------------+-------------+-----------+-----------------+-------------+
| student_id | student_name| course_id | instructor_name | dept_office |
+------------+-------------+-----------+-----------------+-------------+
| 101        | Alice       | CS101     | Dr. Smith       | Hall-204    |
| 101        | Alice       | MATH201   | Dr. Fox         | Science-101 |
| 102        | Bob         | CS101     | Dr. Smith       | Hall-204    |
+------------+-------------+-----------+-----------------+-------------+
```
- **What it fixed:** Removed comma-separated arrays.
- **Remaining Flaw (Partial Dependency):** `student_name` depends *only* on `student_id`, not on the full composite key `{student_id, course_id}`.

---

### Step 2: Second Normal Form (2NF) — Eliminate Partial Dependencies
> **Rule:** Must be in 1NF **AND** every non-prime attribute must depend on the **WHOLE primary key**, not a proper subset of a composite primary key.

```
Table 1: Students (PK: student_id)
+------------+-------------+
| student_id | student_name|
+------------+-------------+
| 101        | Alice       |
| 102        | Bob         |
+------------+-------------+

Table 2: CourseOfferings (Composite PK: {student_id, course_id})
+------------+-----------+-----------------+-------------+
| student_id | course_id | instructor_name | dept_office |
+------------+-----------+-----------------+-------------+
| 101        | CS101     | Dr. Smith       | Hall-204    |
| 101        | MATH201   | Dr. Fox         | Science-101 |
| 102        | CS101     | Dr. Smith       | Hall-204    |
+------------+-----------+-----------------+-------------+
```
- **What it fixed:** Separated `Students` to eliminate partial dependencies.
- **Remaining Flaw (Transitive Dependency):** In `CourseOfferings`, `course_id -> instructor_name` and `instructor_name -> dept_office`. Thus, `course_id -> dept_office` via a transitive non-prime dependency.

---

### Step 3: Third Normal Form (3NF) — Eliminate Transitive Dependencies
> **Rule:** Must be in 2NF **AND** no non-prime attribute is transitively dependent on the primary key ($X \to Y \implies X$ is a superkey OR $Y$ is a prime attribute).

```
Table 1: Students (PK: student_id)
Table 2: StudentCourses (PK: {student_id, course_id})
Table 3: Courses (PK: course_id)
+-----------+-----------------+
| course_id | instructor_name |
+-----------+-----------------+
| CS101     | Dr. Smith       |
| MATH201   | Dr. Fox         |
+-----------+-----------------+

Table 4: Instructors (PK: instructor_name)
+-----------------+-------------+
| instructor_name | dept_office |
+-----------------+-------------+
| Dr. Smith       | Hall-204    |
| Dr. Fox         | Science-101 |
+-----------------+-------------+
```
- **What it fixed:** Eliminates update anomalies when an instructor moves offices.

---

### Step 4: Boyce-Codd Normal Form (BCNF) — Strict Determinant Rule
> **Rule:** Must be in 3NF **AND** for every functional dependency $X \to Y$, **$X$ MUST be a Super Key**.

- **When 3NF is not enough:** Occurs when a table has multiple overlapping composite candidate keys.
- **Example:** In a table `AdvisorSelection(student_id, subject, advisor_name)` where:
  - `{student_id, subject} -> advisor_name`
  - `advisor_name -> subject` (Each advisor teaches only 1 subject)
- `advisor_name` is a determinant but NOT a super key. BCNF forces decomposing into `(student_id, advisor_name)` and `(advisor_name, subject)`.

---

# 4. Denormalization: When & Why to Break the Rules

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | NORMALIZED SCHEMA (3NF / BCNF)        | DENORMALIZED SCHEMA                |
+---------------------------------------------------------------------------------------------------+
| Primary Optimization | Write-heavy OLTP workloads            | Read-heavy dashboards & OLAP       |
| Storage Footprint    | Minimal (Zero duplicate data)         | Higher (Redundant columns stored)  |
| Query Performance    | Slower for complex reads (Needs JOINs)| Ultra-fast reads (Pre-joined data) |
| Data Consistency     | High (Single source of truth)         | Risk of inconsistency on update    |
| Write Complexity     | Simple single-row writes              | Write amplification (Update 10 rows)|
+---------------------------------------------------------------------------------------------------+
```

- **System-Design Angle:** In high-throughput microservices (e.g. Amazon product page), storing `seller_name` directly in the `orders` table avoids an expensive cross-service or cross-shard join on every order fetch.

---

# 5. High-Frequency Interview Drill & Verbal Q&A

### Q1: What is the practical difference between 3NF and BCNF?
> **Answer:** 3NF allows the dependency $X \to Y$ if $Y$ is a **prime attribute** (part of a candidate key), even if $X$ is not a super key. BCNF removes this exception and strictly requires **$X$ to be a super key for EVERY functional dependency $X \to Y$**.

### Q2: Why is 3NF considered the industry standard for OLTP systems instead of BCNF?
> **Answer:** Achieving BCNF can sometimes **destroy dependency preservation**, meaning verifying functional dependencies across tables requires expensive runtime cross-table joins on write. 3NF guarantees both lossless decomposition and dependency preservation.

### Q3: How do you handle Many-to-Many (M:N) relationships in relational database schema design?
> **Answer:** By creating an intermediate **Junction / Bridge table** containing foreign keys referencing the primary keys of both participating tables, with a composite primary key consisting of both foreign keys.

### Q4: Why is an unindexed Foreign Key a major database bottleneck?
> **Answer:** When a parent row is updated or deleted, the database must acquire table-level locks or execute a full table scan on the child table to verify referential integrity constraints, causing severe locking contention during concurrent transactions.

### Q5: What is the difference between Natural Keys and Surrogate Keys?
> **Answer:** A **Natural Key** is an attribute with real-world business meaning (e.g. SSN, ISBN). A **Surrogate Key** is an internally generated identifier (e.g. `BIGINT AUTO_INCREMENT` or `UUIDv7`). Surrogate keys are preferred because business definitions of natural keys frequently change, which breaks foreign key relations across child tables.
