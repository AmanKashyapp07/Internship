# Relational Schema Design & Normalization Theory

> **Scope:** Relational Database Keys, Entity-Relationship (ER) Modeling & Cardinality, Functional Dependencies, Armstrong's Axioms, 1NF to BCNF Normalization Ladder, Lossless Join Decomposition, Dependency Preservation, and Denormalization Tradeoffs.

---

# Table of Contents
1. [Relational Database Keys & Physical Storage Implications](#1-relational-database-keys--physical-storage-implications)
2. [Entity-Relationship (ER) Modeling & Cardinality](#2-entity-relationship-er-modeling--cardinality)
3. [Functional Dependencies & Armstrong's Axioms](#3-functional-dependencies--armstrongs-axioms)
4. [The Normalization Ladder (1NF to BCNF)](#4-the-normalization-ladder-1nf-to-bcnf)
5. [Lossless Decomposition & Dependency Preservation](#5-lossless-decomposition--dependency-preservation)
6. [Denormalization: Performance & Consistency Tradeoffs](#6-denormalization-performance--consistency-tradeoffs)
7. [Core Theoretical Review & Summary Principles](#7-core-theoretical-review--summary-principles)

---

# 1. Relational Database Keys & Physical Storage Implications

```
+---------------------------------------------------------------------------------------------------+
| KEY TYPE             | FORMAL DEFINITION & PROPERTIES              | DATABASE EXAMPLE / ROLE      |
+---------------------------------------------------------------------------------------------------+
| Super Key            | Any attribute set SK such that no two distinct | `{email, name}`, `{ssn, age}`|
|                      | tuples have the same values for SK          | (Superset of Candidate Key)  |
+---------------------------------------------------------------------------------------------------+
| Candidate Key        | A minimal Super Key; removing any attribute | `{id}`, `{email}`, `{ssn}`   |
|                      | destroys uniqueness                         | (Eligible to be Primary Key) |
+---------------------------------------------------------------------------------------------------+
| Primary Key (PK)     | The chosen candidate key; uniquely identifies| `user_id`                    |
|                      | tuples; strictly `UNIQUE` and `NOT NULL`    | (Defines Clustered Index)    |
+---------------------------------------------------------------------------------------------------+
| Foreign Key (FK)     | Attribute in relation R1 referencing a PK   | `user_id` in `orders` table  |
|                      | in relation R2; enforces Referential Integrity | (Requires secondary index) |
+---------------------------------------------------------------------------------------------------+
| Composite Key        | Primary Key composed of two or more         | `{student_id, course_id}`    |
|                      | attributes (Used in Many-to-Many junctions) | in `enrollments` table       |
+---------------------------------------------------------------------------------------------------+
| Surrogate Key        | System-generated artificial identifier with | Auto-increment `BIGINT` or   |
|                      | no intrinsic real-world business meaning    | `UUIDv7` (time-ordered)      |
+---------------------------------------------------------------------------------------------------+
| Natural Key          | Attribute formed by real-world data having  | `ssn`, `isbn`, `vin_number`  |
|                      | an inherent business meaning                | (Prone to business changes)  |
+---------------------------------------------------------------------------------------------------+
```

### Physical Storage & Indexing Engine Considerations
- **Sequential vs. Random Keys:** In B+ Tree clustered storage engines (e.g. MySQL InnoDB, SQLite), the table data is physically sorted on disk by the Primary Key.
- **Random Identifiers (UUIDv4):** Inserting uniformly distributed random keys causes frequent **B+ Tree leaf page splits**, reduces disk page fill factors to ~50%, and increases random I/O.
- **Monotonic Identifiers (BIGINT / UUIDv7):** Inserting strictly increasing, timestamp-ordered keys guarantees append-only leaf page inserts, maximizing page fill factor (>90%) and eliminating page fragmentation.

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

### Relational Mapping Rules:
1. **1:1 Mapping:** The primary key of either entity can be placed as a foreign key in the other with a `UNIQUE` constraint, or both entities can be collapsed into a single relation.
2. **1:N Mapping:** The primary key from the "1" side must be migrated as a foreign key into the relation on the "N" (many) side.
3. **M:N Mapping:** Cannot be represented directly with foreign keys in either table without violating 1NF. It requires an intermediate **Junction Table (Bridge Table)** containing foreign keys referencing both tables, with a composite primary key.

---

# 3. Functional Dependencies & Armstrong's Axioms

A **Functional Dependency (FD)** $X \to Y$ between two attribute sets $X$ and $Y$ in relation $R$ states that if two tuples agree on attributes $X$, they must also agree on attributes $Y$.

```
Given tuples t1, t2 in R:
If t1[X] = t2[X], then t1[Y] = t2[Y]
```

### Armstrong's Axioms (Sound & Complete Inference Rules):
1. **Reflexivity:** If $Y \subseteq X$, then $X \to Y$.
2. **Augmentation:** If $X \to Y$, then $XZ \to YZ$ for any attribute set $Z$.
3. **Transitivity:** If $X \to Y$ and $Y \to Z$, then $X \to Z$.

### Secondary Derived Rules:
- **Union:** If $X \to Y$ and $X \to Z$, then $X \to YZ$.
- **Decomposition:** If $X \to YZ$, then $X \to Y$ and $X \to Z$.
- **Pseudo-Transitivity:** If $X \to Y$ and $WY \to Z$, then $WX \to Z$.

---

# 4. The Normalization Ladder (1NF to BCNF)

Normalization is the formal process of structuring relational schemas to minimize redundancy and eliminate insertion, update, and deletion anomalies.

```
Unnormalized -> 1NF -> 2NF -> 3NF -> BCNF (Strict Determinant Rule)
```

---

### Running Example: University Course Registration System

#### Initial Unnormalized Relation:
```
StudentEnrollment(student_id, student_name, course_id, course_title, instructor_name, instructor_office)
```

---

### Step 1: First Normal Form (1NF)
> **Definition:** A relation is in 1NF if and only if all attribute domains contain only **atomic (indivisible) values**, and there are no repeating groups or arrays.

#### Violating 1NF:
```
+------------+--------------+----------------------+--------------------+
| student_id | student_name | courses              | instructors        |
+------------+--------------+----------------------+--------------------+
| 101        | Alice        | {CS101, MATH201}     | {Dr. Smith, Fox}   |
+------------+--------------+----------------------+--------------------+
```

#### Transformed to 1NF (Atomic rows, Composite Key `{student_id, course_id}`):
```
+------------+--------------+-----------+--------------+-----------------+-------------------+
| student_id | student_name | course_id | course_title | instructor_name | instructor_office |
+------------+--------------+-----------+--------------+-----------------+-------------------+
| 101        | Alice        | CS101     | Intro CS     | Dr. Smith       | Hall-204          |
| 101        | Alice        | MATH201   | Calculus     | Dr. Fox         | Science-101       |
| 102        | Bob          | CS101     | Intro CS     | Dr. Smith       | Hall-204          |
+------------+--------------+-----------+--------------+-----------------+-------------------+
```

---

### Step 2: Second Normal Form (2NF)
> **Definition:** A relation is in 2NF if and only if it is in 1NF and **every non-prime attribute is fully functionally dependent on the entire Primary Key** (no partial dependencies on a subset of a composite primary key).

- **Composite Primary Key:** `{student_id, course_id}`
- **Partial Dependencies Present:**
  - `student_id -> student_name` (Depends only on part of PK)
  - `course_id -> course_title, instructor_name, instructor_office` (Depends only on part of PK)

#### 2NF Decomposition:
```
Relation 1: Students(student_id, student_name) - PK: student_id
Relation 2: CourseOfferings(course_id, course_title, instructor_name, instructor_office) - PK: course_id
Relation 3: Enrollments(student_id, course_id) - PK: {student_id, course_id}
```

---

### Step 3: Third Normal Form (3NF)
> **Definition:** A relation is in 3NF if and only if it is in 2NF and **no non-prime attribute is transitively dependent on the Primary Key**.
> Formally, for every non-trivial functional dependency $X \to Y$:
> - Either $X$ is a **Super Key**, OR
> - $Y$ is a **Prime Attribute** (part of some candidate key).

- In `CourseOfferings(course_id, course_title, instructor_name, instructor_office)`:
  - `course_id -> instructor_name`
  - `instructor_name -> instructor_office` (Transitive dependency via non-key attribute)

#### 3NF Decomposition:
```
Table 1: Students(student_id, student_name) - PK: student_id
Table 2: Enrollments(student_id, course_id) - PK: {student_id, course_id}
Table 3: Courses(course_id, course_title, instructor_name) - PK: course_id, FK: instructor_name
Table 4: Instructors(instructor_name, instructor_office) - PK: instructor_name
```

---

### Step 4: Boyce-Codd Normal Form (BCNF)
> **Definition:** A relation is in BCNF if and only if for **every non-trivial functional dependency $X \to Y$, $X$ MUST be a Super Key**.

- **Comparison with 3NF:** 3NF allows $X \to Y$ if $Y$ is prime even if $X$ is not a super key. BCNF eliminates this exception entirely.
- **Example Scenario:** `AdvisorAssignment(student_id, subject, advisor_name)` where:
  - `{student_id, subject} -> advisor_name` (Candidate Key)
  - `advisor_name -> subject` (Each advisor belongs to only 1 subject)
- Here, `advisor_name` is a determinant but NOT a super key.
- **BCNF Decomposition:**
  - `Advisor(advisor_name, subject)` - PK: `advisor_name`
  - `StudentAdvisor(student_id, advisor_name)` - PK: `{student_id, advisor_name}`

---

# 5. Lossless Decomposition & Dependency Preservation

When decomposing relation $R$ into sub-relations $R_1, R_2, \dots, R_k$:

### 1. Lossless-Join Decomposition (Mandatory Property)
A decomposition of $R$ into $R_1$ and $R_2$ is lossless if and only if the natural join yields the exact original relation:
$$R_1 \bowtie R_2 = R$$
**Theorem:** A decomposition of $R$ into $(R_1, R_2)$ is lossless with respect to a set of FDs $F$ if and only if at least one of the following dependencies is in $F^+$:
- $(R_1 \cap R_2) \to R_1$, OR
- $(R_1 \cap R_2) \to R_2$
*(The shared attribute set must form a super key for at least one of the decomposed relations).*

### 2. Dependency Preservation Property
A decomposition is dependency-preserving if enforcing all functional dependencies on each sub-relation independently guarantees that all original functional dependencies in $F$ are satisfied without computing cross-table joins.
- **Note:** 3NF always guarantees both lossless join and dependency preservation. BCNF guarantees lossless join, but can sometimes fail to preserve all dependencies.

---

# 6. Denormalization: Performance & Consistency Tradeoffs

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | NORMALIZED SCHEMA (3NF / BCNF)        | DENORMALIZED SCHEMA                |
+---------------------------------------------------------------------------------------------------+
| Primary Optimization | Write-heavy OLTP workloads            | Read-heavy dashboards & OLAP       |
| Storage Footprint    | Minimal (Zero duplicate data)         | Higher (Redundant columns stored)  |
| Query Performance    | Slower for complex reads (Needs JOINs)| Ultra-fast reads (Pre-joined data) |
| Data Consistency     | High (Single source of truth)         | Risk of inconsistency on update    |
| Write Complexity     | Simple single-row writes              | Write amplification on update      |
+---------------------------------------------------------------------------------------------------+
```

### Architectural Tradeoff Analysis:
1. **Normalized (3NF):** Minimizes storage, completely eliminates update anomalies, guarantees strict ACID integrity for transactional updates.
2. **Denormalized:** Stores calculated sums or duplicate parent fields directly in child tables to avoid expensive multi-table joins under high read query volumes. Requires application-level triggers or transactional batches to maintain consistency across redundant fields.

---

# 7. Core Theoretical Review & Summary Principles

1. **Super Key vs. Candidate Key:** A Super Key is any attribute set uniquely determining tuples; a Candidate Key is a minimal Super Key with zero extraneous attributes.
2. **Primary Key vs. Secondary Foreign Key:** The Primary Key enforces uniqueness and non-nullability, establishing the clustered index storage order. Foreign Keys enforce referential integrity across relational boundaries.
3. **1NF:** Requires atomic attribute domains and eliminates repeating groups.
4. **2NF:** Eliminates partial functional dependencies on composite candidate keys.
5. **3NF:** Eliminates transitive functional dependencies between non-prime attributes.
6. **BCNF:** Enforces that every determinant in every non-trivial functional dependency must be a super key.
7. **3NF vs BCNF Tradeoff:** 3NF guarantees both lossless decomposition and dependency preservation; BCNF guarantees lossless decomposition but may destroy dependency preservation for schemas with overlapping composite candidate keys.
