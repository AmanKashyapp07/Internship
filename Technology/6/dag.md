# DAG Pipeline Schedulers & Kahn's Algorithm: Architecture Guide

> **Core Concept:** A CI/CD build pipeline is mathematically a **Directed Acyclic Graph (DAG)** where vertices represent build/test stages and directed edges represent execution dependencies ($A \to B$ indicates stage $A$ must succeed before stage $B$ can start). A DAG Scheduler resolves stage dependencies, detects circular deadlock dependencies via **DFS Cycle Detection**, and executes independent stages in parallel using **Kahn's Topological Sort Algorithm**.

---

## 1. What It Is in Plain English

In a complex CI/CD system (like GitHub Actions, GitLab CI, or MagnusCI), pipelines are rarely simple linear scripts. For example:
- `Frontend Build` and `Backend Build` can run **in parallel**.
- `E2E Integration Tests` can only run **after both** frontend and backend builds finish.
- `Production Deploy` can only run **after** E2E tests pass.

If a developer accidentally configures circular dependencies (Stage A depends on B, and B depends on A), a naive scheduler enters an infinite wait loop (**Deadlock**).

A DAG Scheduler validates that the pipeline contains **zero cycles**, identifies which stages have 0 unresolved dependencies, and dispatches all ready stages to worker queues simultaneously for maximum parallel execution speed.

---

## 2. DAG Pipeline Graph & Topological Execution Flow

```
[ Stage: Lint (In-degree: 0) ]       [ Stage: Unit Tests (In-degree: 0) ]
              \                                      /
               \                                    /
                v                                  v
              [ Stage: Docker Build (In-degree: 2) ]
                                |
                                v
             [ Stage: Deploy Staging (In-degree: 1) ]

EXECUTION PHASES (Kahn's BFS):
Phase 1: In-degree 0 Queue: [ Lint, Unit Tests ] -> Run concurrently in 2 worker pods!
Phase 2: Lint & Unit Tests finish -> Decrement Docker Build in-degree (2 -> 0) -> Enqueue Docker Build!
Phase 3: Docker Build finishes -> Enqueue Deploy Staging!
```

---

## 3. How I Used It (MagnusCI)

- **MagnusCI (Custom DAG Pipeline Scheduler):**
  - **Pre-Execution Cycle Validation (DFS 3-Color Cycle Detection):** Before scheduling a pipeline run, the engine runs a Depth-First Search with recursion state tracking (`UNVISITED`, `VISITING`, `VISITED`). If a back-edge to a `VISITING` node is encountered, the scheduler immediately rejects the pipeline configuration with a descriptive cycle error path (e.g. `Stage A -> Stage B -> Stage A`), preventing deadlocks.
  - **Dynamic Parallel Execution (Kahn's Algorithm + BullMQ):**
    1. Built an in-memory dependency graph and calculated the `inDegree` map for all stages.
    2. Enqueued all stages with `inDegree === 0` into the BullMQ task queue.
    3. When a stage completes successfully, the worker emits an event that decrements the `inDegree` of its dependent child stages. The moment a child stage reaches `inDegree === 0`, it is immediately dispatched for parallel execution.

---

## 4. Analogy for Live Interviews

> *"Imagine building a house: You can paint the exterior walls (Task A) and install the interior kitchen plumbing (Task B) at the same time in parallel. But you cannot install the roof (Task C) until the structural concrete pillars (Task D) are dry. Kahn's Algorithm is the master construction site manager who keeps a live checklist of prerequisites for every contractor. The second a prerequisite is completed, the manager radios the waiting crew to start immediately."*

---

## 5. Kahn's Algorithm (BFS) vs. DFS Topological Sort

| Dimension | Kahn's Algorithm (BFS / In-Degree) | DFS Post-Order Topological Sort |
| :--- | :--- | :--- |
| **Data Structures** | In-degree Array/Map + FIFO Queue | Recursion Call Stack / Visited Set |
| **Time / Space Complexity**| $O(V + E)$ Time, $O(V)$ Space | $O(V + E)$ Time, $O(V)$ Space |
| **Concurrency Fit** | **Perfect for Parallel Schedulers** (Queue holds all parallel runnable tasks) | Natural for static linear ordering |
| **Cycle Detection** | Count of processed nodes $< V \implies$ Cycle exists | Back-edge to node in recursion stack $\implies$ Cycle exists |

---

## 6. Algorithmic Implementation: Kahn's Scheduling Engine

```ts
interface Stage { id: string; dependencies: string[]; }

function scheduleDAG(stages: Stage[]): void {
    const inDegree = new Map<string, number>();
    const adjList = new Map<string, string[]>();

    // 1. Initialize Graph & In-Degree Map
    stages.forEach(s => { inDegree.set(s.id, s.dependencies.length); adjList.set(s.id, []); });
    stages.forEach(s => s.dependencies.forEach(dep => adjList.get(dep)?.push(s.id)));

    // 2. Queue all zero-dependency stages for parallel execution
    const readyQueue: string[] = [];
    inDegree.forEach((deg, id) => { if (deg === 0) readyQueue.push(id); });

    // 3. Dispatch ready stages to BullMQ worker pool
    readyQueue.forEach(stageId => dispatchToWorker(stageId));
}

// 4. Invoked when a worker finishes a stage
function onStageSuccess(completedStageId: string): void {
    const children = adjList.get(completedStageId) || [];
    children.forEach(childId => {
        const remaining = (inDegree.get(childId) || 1) - 1;
        inDegree.set(childId, remaining);
        if (remaining === 0) {
            dispatchToWorker(childId); // Run immediately!
        }
    });
}
```

---

## 7. 5–8 High-Yield Interview Questions & Direct Answers

### Q1: What is a Directed Acyclic Graph (DAG)?
> **Answer:** A DAG is a directed graph containing vertices and directed edges that has **no directed cycles**. That is, starting from any vertex $v$, there is no sequence of directed edges that loops back to $v$. In workflow orchestration, this guarantees that tasks have a finite, ordered start and end without circular deadlock dependencies.

### Q2: How does Kahn's algorithm detect cycles in a graph?
> **Answer:** Kahn's algorithm maintains a queue of nodes with in-degree 0. Each time a node is popped, it increments a `processedCount` counter and decrements the in-degree of its neighbors. If the graph contains a cycle, the nodes involved in the cycle (and their descendants) never reach an in-degree of 0 and never enter the queue. If `processedCount !== totalNodes`, **a cycle exists**.

### Q3: How does the 3-Color DFS algorithm detect cycles?
> **Answer:**
> - **WHITE (0):** Unvisited node.
> - **GRAY (1):** Currently being explored in the active recursion call stack.
> - **BLACK (2):** Fully explored node and all its subtrees.
> During DFS traversal, if we transition from a node to a neighbor that is currently marked **GRAY**, we have found a **Back-Edge**, which mathematically proves the existence of a directed cycle.

### Q4: How do you handle Stage Failures in a DAG Pipeline?
> **Answer:** If Stage A fails, all downstream stages that directly or transitively depend on Stage A must be marked as **`SKIPPED` / `CANCELLED`**. Independent branches that do not depend on Stage A continue executing to completion. Once all active branches terminate, the overall pipeline run status transitions to `FAILED`.

---

## 8. Common "Gotcha" Questions Interviewers Ask

### Gotcha 1: "What happens if two stages finish at the exact same millisecond in a distributed multi-worker cluster?"
- **The Danger:** A race condition on decrementing the child stage's `inDegree` counter in the database/Redis. If Worker 1 and Worker 2 read `inDegree = 2` simultaneously, both decrement it to `1`, and the child stage is never dispatched to the queue (deadlocking the pipeline!).
- **The Senior Fix:** Use atomic database operations:
  ```sql
  UPDATE pipeline_stages 
  SET remaining_dependencies = remaining_dependencies - 1 
  WHERE id = $child_id 
  RETURNING remaining_dependencies;
  ```
  The worker that receives `remaining_dependencies === 0` from the atomic SQL return value is the sole worker responsible for enqueuing the stage.
