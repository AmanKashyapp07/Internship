# CSES Graph Algorithms - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 32 problems in the **Graph Algorithms** topic.

---

## 1. Counting Rooms

- **Link**: [Counting Rooms](https://cses.fi/problemset/task/1192)
- **Problem Statement**: Given a grid map of size $N \times M$ consisting of floor tiles (`.`) and wall tiles (`#`), count the total number of connected floor rooms.
- **Interview Pattern**: 2D Grid Connected Components (Flood Fill / BFS / DFS).
- **Intuition**:
  - Each floor tile `.` belongs to a room. Connected floor tiles in 4 cardinal directions form a single connected component.
  - The total number of rooms is the count of connected components in the grid.
- **Step-by-Step Interview Walkthrough**:
  1. Iterate over every cell $(r, c)$ in the $N \times M$ grid.
  2. If cell $(r, c)$ is `.`, increment room count and start a Flood Fill (DFS/BFS) to mark all 4-directionally reachable `.` cells as visited (`#`).
  3. Return total room count.
- **Pseudocode**:
```text
function count_rooms():
    rooms = 0
    for r = 0 to N-1:
        for c = 0 to M-1:
            if grid[r][c] == '.':
                rooms += 1
                dfs(r, c)
    return rooms

function dfs(r, c):
    grid[r][c] = '#'
    for (dr, dc) in [(-1,0), (1,0), (0,-1), (0,1)]:
        nr = r + dr, nc = c + dc
        if valid(nr, nc) and grid[nr][nc] == '.':
            dfs(nr, nc)
```
- **Complexity**: Time: $\mathcal{O}(N \times M)$, Space: $\mathcal{O}(N \times M)$ recursion/queue.

---

## 2. Labyrinth

- **Link**: [Labyrinth](https://cses.fi/problemset/task/1193)
- **Problem Statement**: Given a grid map of size $N \times M$ with a start tile `A` and an end tile `B`, find the shortest path from `A` to `B` and print the sequence of moves ('L', 'R', 'U', 'D').
- **Interview Pattern**: Grid Shortest Path BFS + Parent Pointer Reconstruction.
- **Intuition**:
  - In an unweighted grid, BFS guarantees the shortest path from start `A` to end `B`.
  - Store directional moves ('L', 'R', 'U', 'D') at each cell to reconstruct the path backwards once `B` is reached.
- **Step-by-Step Interview Walkthrough**:
  1. Enqueue start cell `A` and mark visited.
  2. Perform BFS. For each neighbor, record the move direction used to reach it (`parent_dir[nr][nc] = dir`).
  3. When `B` is popped, trace back from `B` to `A` using `parent_dir` and reverse the string to get the shortest path.
- **Pseudocode**:
```text
queue.push(start_A)
vis[start_A] = true

while queue not empty:
    (r, c) = queue.pop()
    if (r, c) == end_B: break
    for (dr, dc, move_char) in [(-1,0,'U'), (1,0,'D'), (0,-1,'L'), (0,1,'R')]:
        nr = r + dr, nc = c + dc
        if valid(nr, nc) and not vis[nr][nc]:
            vis[nr][nc] = true
            parent_dir[nr][nc] = move_char
            queue.push((nr, nc))

reconstruct path by backtracking from end_B to start_A
```
- **Complexity**: Time: $\mathcal{O}(N \times M)$, Space: $\mathcal{O}(N \times M)$.

---

## 3. Building Roads

- **Link**: [Building Roads](https://cses.fi/problemset/task/1666)
- **Problem Statement**: Given $N$ cities and $M$ existing roads, find the minimum number of new roads required to connect all cities into a single component, and output which roads to build.
- **Interview Pattern**: Connected Components via DFS / Disjoint Set Union (DSU).
- **Intuition**:
  - To make the entire graph connected with minimum roads, find all connected components and connect representative nodes of adjacent components in a line.
  - $K$ components require $K - 1$ new edges.
- **Step-by-Step Interview Walkthrough**:
  1. Find representatives of each connected component via DFS/BFS.
  2. Store one representative node from each component in a list `rep`.
  3. Output `rep.size() - 1` roads connecting `(rep[i], rep[i+1])`.
- **Pseudocode**:
```text
representatives = []
for u = 1 to N:
    if not vis[u]:
        representatives.push(u)
        dfs(u)

output representatives.size() - 1
for i = 0 to representatives.size() - 2:
    print representatives[i], representatives[i+1]
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 4. Message Route

- **Link**: [Message Route](https://cses.fi/problemset/task/1667)
- **Problem Statement**: Given a network of $N$ computers and $M$ connections, find the minimum number of computers needed to send a message from computer 1 to computer $N$ and print the route.
- **Interview Pattern**: Unweighted Graph Shortest Path via BFS.
- **Intuition**:
  - Unweighted shortest path from node 1 to node $N$ is directly solved by BFS.
  - Maintain a `parent` array to reconstruct the sequence of computers along the shortest route.
- **Step-by-Step Interview Walkthrough**:
  1. Start BFS from node 1 with `parent[1] = 0`.
  2. When visiting unvisited node $v$ from $u$, set `parent[v] = u` and enqueue $v$.
  3. If node $N$ is reached, backtrack from $N$ to 1 using `parent` pointers and print the route. If unreachable, output `IMPOSSIBLE`.
- **Pseudocode**:
```text
queue.push(1), vis[1] = true
while queue not empty:
    u = queue.pop()
    for v in adj[u]:
        if not vis[v]:
            vis[v] = true, parent[v] = u, queue.push(v)

if not vis[N]: print "IMPOSSIBLE"
else: backtrack path from N to 1 using parent array
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 5. Building Teams

- **Link**: [Building Teams](https://cses.fi/problemset/task/1668)
- **Problem Statement**: Given $N$ pupils and $M$ friendship pairs, divide pupils into 2 teams such that no two friends belong to the same team (check if graph is bipartite).
- **Interview Pattern**: Bipartite Graph Checking / 2-Coloring via BFS/DFS.
- **Intuition**:
  - Assign every person to Team 1 or Team 2 such that no two friends are in the same team. This is equivalent to 2-coloring a graph.
  - If any odd cycle exists, 2-coloring is impossible.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize `color[u] = 0` for all nodes.
  2. For each uncolored node, set `color[u] = 1` and run BFS/DFS.
  3. For neighbor $v$, if uncolored set `color[v] = 3 - color[u]`. If colored and `color[v] == color[u]`, graph is not bipartite (`IMPOSSIBLE`).
- **Pseudocode**:
```text
for u = 1 to N:
    if color[u] == 0:
        color[u] = 1
        queue.push(u)
        while queue not empty:
            curr = queue.pop()
            for v in adj[curr]:
                if color[v] == 0:
                    color[v] = 3 - color[curr]
                    queue.push(v)
                else if color[v] == color[curr]:
                    return "IMPOSSIBLE"
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 6. Round Trip

- **Link**: [Round Trip](https://cses.fi/problemset/task/1669)
- **Problem Statement**: Given $N$ cities and $M$ roads (undirected), find any simple cycle containing at least 3 cities, or state that no such cycle exists.
- **Interview Pattern**: Cycle Detection in Undirected Graph via DFS.
- **Intuition**:
  - A cycle exists in an undirected graph if a DFS traversal encounters a visited node $v$ that is **not** the immediate parent of the current node $u$.
- **Step-by-Step Interview Walkthrough**:
  1. Run DFS maintaining `parent[u]`.
  2. For neighbor $v$: if $v == \text{parent}[u]$, continue.
  3. If $v$ is already visited, a cycle is found! Backtrack from $u$ to $v$ using `parent` pointers to reconstruct the cycle path.
- **Pseudocode**:
```text
function dfs(u, p):
    vis[u] = true, parent[u] = p
    for v in adj[u]:
        if v == p: continue
        if vis[v]:
            cycle_start = v, cycle_end = u
            return true
        if dfs(v, u): return true
    return false
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 7. Monsters

- **Link**: [Monsters](https://cses.fi/problemset/task/1194)
- **Problem Statement**: Given a grid with a player at `A`, multiple monsters `M`, and walls `#`, determine if the player can escape to any boundary tile without ever occupying the same cell as a monster.
- **Interview Pattern**: Multi-Source BFS + Path Verification.
- **Intuition**:
  - Monsters and human player move at equal speed. A cell is safe for human if human distance < minimum monster distance to that cell.
- **Step-by-Step Interview Walkthrough**:
  1. **Multi-Source BFS for Monsters**: Push all monster starting cells into a queue to compute `monster_dist[r][c]` for all cells.
  2. **Single-Source BFS for Human**: Start BFS from human start `A`. Human can move to cell $(nr, nc)$ only if `human_dist[nr][nc] < monster_dist[nr][nc]`.
  3. If human reaches any boundary cell, reconstruct path and print `YES`. Otherwise `NO`.
- **Pseudocode**:
```text
// Step 1: Multi-source BFS for monsters
monster_queue.push(all monster locations)
compute monster_dist[r][c]

// Step 2: BFS for player 'A'
player_queue.push(start_A)
while player_queue not empty:
    (r, c) = player_queue.pop()
    if is_boundary(r, c): reconstruct_and_exit()
    for (nr, nc, dir) in neighbors:
        if human_dist[r][c] + 1 < monster_dist[nr][nc]:
            human_dist[nr][nc] = human_dist[r][c] + 1
            parent_dir[nr][nc] = dir
            player_queue.push((nr, nc))
```
- **Complexity**: Time: $\mathcal{O}(N \times M)$, Space: $\mathcal{O}(N \times M)$.

---

## 8. Shortest Routes I

- **Link**: [Shortest Routes I](https://cses.fi/problemset/task/1671)
- **Problem Statement**: Given a directed weighted graph with $N$ nodes and non-negative edge weights, calculate the shortest distance from node 1 to all nodes $1 \dots N$.
- **Interview Pattern**: Single-Source Shortest Path (Dijkstra's Algorithm).
- **Intuition**:
  - Non-negative weighted directed graph shortest path from node 1 to all nodes. Min-priority queue greedy expansion.
- **Step-by-Step Interview Walkthrough**:
  1. Set `dist[1] = 0` and `dist[u] = INF` for all $u > 1$.
  2. Push `(0, 1)` into min-heap priority queue.
  3. Pop node $u$ with distance $d$. If $d > \text{dist}[u]$, skip (stale entry).
  4. Relax edge $(u, v, w)$: if $\text{dist}[u] + w < \text{dist}[v]$, update $\text{dist}[v] = \text{dist}[u] + w$ and push `(dist[v], v)`.
- **Pseudocode**:
```text
dist[1..N] = INF, dist[1] = 0
pq.push((0, 1))

while pq not empty:
    (d, u) = pq.pop_min()
    if d > dist[u]: continue
    for (v, weight) in adj[u]:
        if dist[u] + weight < dist[v]:
            dist[v] = dist[u] + weight
            pq.push((dist[v], v))
```
- **Complexity**: Time: $\mathcal{O}((V + E) \log V)$, Space: $\mathcal{O}(V + E)$.

---

## 9. Shortest Routes II

- **Link**: [Shortest Routes II](https://cses.fi/problemset/task/1672)
- **Problem Statement**: Given a weighted graph with $N$ nodes, process $Q$ queries asking for the shortest distance between any pair of nodes $(u, v)$.
- **Interview Pattern**: All-Pairs Shortest Path (Floyd-Warshall Algorithm).
- **Intuition**:
  - Compute shortest path between all pairs $(u, v)$ for $N \le 500$. DP state transition considers using intermediate vertex $k$.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize distance matrix `dist[u][v]` with edge weights, `dist[u][u] = 0`, and `INF` elsewhere.
  2. Triple nested loops: for intermediate vertex $k$, for source $i$, for destination $j$:
     $$\text{dist}[i][j] = \min(\text{dist}[i][j], \text{dist}[i][k] + \text{dist}[k][j])$$
  3. Answer queries in $\mathcal{O}(1)$ lookup.
- **Pseudocode**:
```text
for k = 1 to N:
    for i = 1 to N:
        for j = 1 to N:
            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
```
- **Complexity**: Time: $\mathcal{O}(N^3)$, Space: $\mathcal{O}(N^2)$.

---

## 10. High Score

- **Link**: [High Score](https://cses.fi/problemset/task/1673)
- **Problem Statement**: Given a directed graph with edge weights (scores), find the maximum score path from node 1 to node $N$, or output -1 if an infinitely large score is possible.
- **Interview Pattern**: Bellman-Ford + Negative Cycle Detection (Max Path via Sign Inversion).
- **Intuition**:
  - Maximize score from 1 to $N$ with edge weights. Invert edge weights ($w' = -w$) to turn it into a shortest path problem with negative cycles.
  - A positive weight cycle reachable from 1 and able to reach $N$ allows infinite score ($\to -1$).
- **Step-by-Step Interview Walkthrough**:
  1. Negate all edge weights. Run $N - 1$ rounds of Bellman-Ford edge relaxation.
  2. Run $N$-th round: if an edge $(u, v)$ can still be relaxed, node $v$ is in/reachable from a negative cycle.
  3. Run BFS from all cycle nodes to check if any can reach node $N$ AND is reachable from node 1.
  4. If reachable, print -1. Otherwise print `-dist[N]`.
- **Pseudocode**:
```text
dist[1..N] = INF, dist[1] = 0
for round = 1 to N-1:
    for (u, v, w) in edges:
        dist[v] = min(dist[v], dist[u] + w)

// Detect reachable cycles
for (u, v, w) in edges:
    if dist[u] + w < dist[v] and reachable_from_1[u] and can_reach_N[v]:
        return -1
return -dist[N]
```
- **Complexity**: Time: $\mathcal{O}(V \times E)$, Space: $\mathcal{O}(V + E)$.

---

## 11. Flight Discount

- **Link**: [Flight Discount](https://cses.fi/problemset/task/1195)
- **Problem Statement**: Given a directed weighted graph representing flight routes, find the minimum cost to travel from 1 to $N$ if you can halve the cost of exactly one flight.
- **Interview Pattern**: State-Space Graph Dijkstra (Expanded Node State).
- **Intuition**:
  - You can halve the weight of at most 1 flight edge.
  - Double node state `(u, coupon_used)`:
    - State `(u, 0)`: reached node $u$ without using coupon yet.
    - State `(u, 1)`: reached node $u$ having used coupon on some prior edge (or current edge).
- **Step-by-Step Interview Walkthrough**:
  1. Maintain `dist[u][0]` and `dist[u][1]`.
  2. For edge $(u, v, w)$:
     - Move without coupon: `dist[u][0] + w -> dist[v][0]`
     - Move using coupon now: `dist[u][0] + w/2 -> dist[v][1]`
     - Move after coupon used: `dist[u][1] + w -> dist[v][1]`
  3. Answer is $\min(\text{dist}[N][0], \text{dist}[N][1])$.
- **Pseudocode**:
```text
pq.push((0, 1, 0)) // (dist, node, coupon_used)
while pq not empty:
    (d, u, state) = pq.pop_min()
    if d > dist[u][state]: continue
    for (v, w) in adj[u]:
        if state == 0:
            relax(v, 0, d + w)
            relax(v, 1, d + w / 2)
        else:
            relax(v, 1, d + w)
```
- **Complexity**: Time: $\mathcal{O}((V + E) \log V)$, Space: $\mathcal{O}(V + E)$.

---

## 12. Cycle Finding

- **Link**: [Cycle Finding](https://cses.fi/problemset/task/1197)
- **Problem Statement**: Given a directed graph with edge weights (which can be negative), determine if a negative cycle exists and print the nodes in the cycle.
- **Interview Pattern**: Bellman-Ford Negative Cycle Extraction.
- **Intuition**:
  - Run $N$ iterations of Bellman-Ford. If distance decreases in the $N$-th iteration, a negative cycle exists.
  - Trace `parent` pointers back $N$ times to guarantee landing inside the cycle.
- **Step-by-Step Interview Walkthrough**:
  1. Run $N$ rounds of Bellman-Ford edge relaxations.
  2. If no relaxation occurs on $N$-th round, output `NO`.
  3. If node $X$ relaxes on $N$-th round: walk $X = \text{parent}[X]$ $N$ times to enter the cycle loop.
  4. Trace cycle until $X$ is revisited, store nodes, reverse, and print.
- **Pseudocode**:
```text
last_updated = -1
for round = 1 to N:
    last_updated = -1
    for (u, v, w) in edges:
        if dist[u] + w < dist[v]:
            dist[v] = dist[u] + w
            parent[v] = u
            last_updated = v

if last_updated == -1: print "NO"
else:
    cycle_node = last_updated
    for i = 1 to N: cycle_node = parent[cycle_node]
    reconstruct cycle loop from cycle_node
```
- **Complexity**: Time: $\mathcal{O}(V \times E)$, Space: $\mathcal{O}(V + E)$.

---

## 13. Flight Routes

- **Link**: [Flight Routes](https://cses.fi/problemset/task/1196)
- **Problem Statement**: Given a directed weighted graph, find the $K$ shortest path lengths from node 1 to node $N$.
- **Interview Pattern**: $K$-Shortest Paths via Modified Dijkstra.
- **Intuition**:
  - Maintain a max-heap/vector of up to $K$ shortest distances for every node instead of just 1.
- **Step-by-Step Interview Walkthrough**:
  1. `dist[u]` is a `priority_queue` / `vector` storing the top $K$ smallest distances to $u$.
  2. Pop `(d, u)` from min-heap. If $d > \text{dist}[u][K-1]$, continue.
  3. For neighbor $v$ with edge weight $w$: if $d + w < \text{dist}[v][K-1]$, insert $d + w$ into $v$'s top-$K$ container and push `(d + w, v)` to heap.
- **Pseudocode**:
```text
dist[u] = max_heap of size up to K initialized to INF
dist[1].push(0)
pq.push((0, 1))

while pq not empty:
    (d, u) = pq.pop_min()
    if d > dist[u].largest(): continue
    for (v, w) in adj[u]:
        if dist[v].size() < K or d + w < dist[v].largest():
            dist[v].insert(d + w)
            pq.push((d + w, v))
```
- **Complexity**: Time: $\mathcal{O}(K \cdot (V + E) \log(V \cdot K))$, Space: $\mathcal{O}(K \cdot V + E)$.

---

## 14. Round Trip II

- **Link**: [Round Trip II](https://cses.fi/problemset/task/1678)
- **Problem Statement**: Given a directed graph with $N$ nodes and $M$ edges, find any directed cycle or output `IMPOSSIBLE`.
- **Interview Pattern**: Directed Graph Cycle Detection via 3-Color DFS.
- **Intuition**:
  - In a directed graph, a cycle exists iff DFS visits a node currently on the recursion stack (`visited_state == IN_STACK` / `COLOR_GRAY`).
- **Step-by-Step Interview Walkthrough**:
  1. Node states: 0 = Unvisited, 1 = Visiting (in stack), 2 = Visited (done).
  2. DFS from $u$: set `state[u] = 1`. For neighbor $v$:
     - If `state[v] == 1`: cycle detected! $v$ is cycle start, $u$ is cycle end.
     - If `state[v] == 0`: set `parent[v] = u`, recurse.
  3. Mark `state[u] = 2` when leaving $u$. Reconstruct cycle via parent pointers.
- **Pseudocode**:
```text
function dfs(u):
    state[u] = 1 // GRAY
    for v in adj[u]:
        if state[v] == 1:
            cycle_start = v, cycle_end = u
            return true
        if state[v] == 0:
            parent[v] = u
            if dfs(v): return true
    state[u] = 2 // BLACK
    return false
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 15. Course Schedule

- **Link**: [Course Schedule](https://cses.fi/problemset/task/1679)
- **Problem Statement**: Given $N$ courses and $M$ prerequisite requirements (directed edges $a \to b$), output any valid ordering of courses to complete all of them, or `IMPOSSIBLE` if a cycle exists.
- **Interview Pattern**: Topological Sort (Kahn's BFS / DFS Post-order).
- **Intuition**:
  - Directed Acyclic Graph (DAG) dependency resolution. Kahn's algorithm processes nodes with `indegree == 0`.
- **Step-by-Step Interview Walkthrough**:
  1. Compute indegrees for all nodes.
  2. Push all nodes with `indegree == 0` into queue.
  3. Pop node $u$, add to `order`. For each neighbor $v$, decrement `indegree[v]`. If `indegree[v] == 0`, push $v$.
  4. If `order.size() == N`, valid schedule found. Else graph has cycle (`IMPOSSIBLE`).
- **Pseudocode**:
```text
for u = 1 to N:
    if indegree[u] == 0: queue.push(u)

while queue not empty:
    u = queue.pop()
    topo_order.push(u)
    for v in adj[u]:
        indegree[v] -= 1
        if indegree[v] == 0: queue.push(v)

if topo_order.size() == N: return topo_order
else: return "IMPOSSIBLE"
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 16. Longest Flight Route

- **Link**: [Longest Flight Route](https://cses.fi/problemset/task/1680)
- **Problem Statement**: Given a directed acyclic graph (DAG) representing flight connections, find the longest path (maximum number of nodes visited) from node 1 to node $N$.
- **Interview Pattern**: Dynamic Programming on DAG (Topological Order DP).
- **Intuition**:
  - Longest path on a DAG from 1 to $N$ is computed in topological order: $\text{dp}[v] = \max(\text{dp}[v], \text{dp}[u] + 1)$.
- **Step-by-Step Interview Walkthrough**:
  1. Obtain Topological Sort order using Kahn's or DFS.
  2. Set `dp[1] = 1`, `dp[u] = -INF` for $u > 1$.
  3. Iterate nodes $u$ in topological order: if `dp[u] != -INF`, for neighbor $v$, if `dp[u] + 1 > dp[v]`, update `dp[v] = dp[u] + 1` and `parent[v] = u`.
  4. Output `dp[N]` and path if reachable.
- **Pseudocode**:
```text
dp[1..N] = -INF, dp[1] = 1
for u in topo_order:
    if dp[u] == -INF: continue
    for v in adj[u]:
        if dp[u] + 1 > dp[v]:
            dp[v] = dp[u] + 1
            parent[v] = u
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 17. Game Routes

- **Link**: [Game Routes](https://cses.fi/problemset/task/1681)
- **Problem Statement**: Given a directed acyclic graph (DAG) representing game levels, count the total number of distinct paths from level 1 to level $N$ modulo $10^9+7$.
- **Interview Pattern**: Counting Paths on DAG via Topological Order DP.
- **Intuition**:
  - `dp[v]` = total paths from 1 to $v$. Recurrence: $\text{dp}[v] = \sum_{(u \to v)} \text{dp}[u] \pmod{10^9+7}$.
- **Step-by-Step Interview Walkthrough**:
  1. Compute indegrees and run Kahn's algorithm for topological order.
  2. Set `dp[1] = 1`, all other `dp[u] = 0`.
  3. Process nodes $u$ in topological order: for neighbor $v$, `dp[v] = (dp[v] + dp[u]) % MOD`.
  4. Output `dp[N]`.
- **Pseudocode**:
```text
dp[1..N] = 0, dp[1] = 1
for u in topo_order:
    for v in adj[u]:
        dp[v] = (dp[v] + dp[u]) % MOD
return dp[N]
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 18. Investigation

- **Link**: [Investigation](https://cses.fi/problemset/task/1202)
- **Problem Statement**: Given a directed weighted graph, calculate four properties for paths from 1 to $N$: (1) minimum cost, (2) number of minimum-cost paths, (3) minimum number of flights in a minimum-cost path, (4) maximum number of flights in a minimum-cost path.
- **Interview Pattern**: Multi-Property Shortest Path DP over Dijkstra DAG.
- **Intuition**:
  - Simultaneously compute 4 values for shortest path from 1 to $N$:
    1. Minimum cost (`dist[u]`)
    2. Number of minimum-cost paths (`ways[u]`)
    3. Minimum number of flights (`min_flights[u]`)
    4. Maximum number of flights (`max_flights[u]`)
- **Step-by-Step Interview Walkthrough**:
  1. Run Dijkstra. When expanding edge $(u, v, w)$:
     - If `dist[u] + w < dist[v]`: replace all 4 DP values at $v$.
     - If `dist[u] + w == dist[v]`: accumulate `ways[v] += ways[u]`, update `min_flights` and `max_flights`.
- **Pseudocode**:
```text
if d + w < dist[v]:
    dist[v] = d + w
    ways[v] = ways[u]
    min_flights[v] = min_flights[u] + 1
    max_flights[v] = max_flights[u] + 1
    pq.push((dist[v], v))
else if d + w == dist[v]:
    ways[v] = (ways[v] + ways[u]) % MOD
    min_flights[v] = min(min_flights[v], min_flights[u] + 1)
    max_flights[v] = max(max_flights[v], max_flights[u] + 1)
```
- **Complexity**: Time: $\mathcal{O}((V + E) \log V)$, Space: $\mathcal{O}(V + E)$.

---

## 19. Planets Queries I

- **Link**: [Planets Queries I](https://cses.fi/problemset/task/1750)
- **Problem Statement**: Given a functional graph (each node has exactly 1 outgoing edge), process $Q$ queries asking for the destination node after starting at node $u$ and taking $K$ steps.
- **Interview Pattern**: Functional Graph Binary Lifting (Successor Graphs).
- **Intuition**:
  - Each node has outdegree 1 (functional graph). Jump pointers `up[u][j]` store the destination after $2^j$ teleports.
- **Step-by-Step Interview Walkthrough**:
  1. Base case: `up[u][0] = successor[u]`.
  2. Precompute: `up[u][j] = up[ up[u][j-1] ][j-1]` for $j=1..30$.
  3. Query $(u, k)$: jump using set bits of $k$.
- **Pseudocode**:
```text
for j = 1 to 29:
    for u = 1 to N:
        up[u][j] = up[ up[u][j-1] ][j-1]

function query(u, k):
    for j = 29 down to 0:
        if k & (1 << j): u = up[u][j]
    return u
```
- **Complexity**: Build: $\mathcal{O}(N \log K)$, Query: $\mathcal{O}(\log K)$, Space: $\mathcal{O}(N \log K)$.

---

## 20. Planets Queries II

- **Link**: [Planets Queries II](https://cses.fi/problemset/task/1160)
- **Problem Statement**: Given a functional graph, process $Q$ queries asking for the minimum number of steps required to reach node $v$ starting from node $u$, or output -1 if unreachable.
- **Interview Pattern**: Functional Graph Decomposition into Cycles and Trees + Binary Lifting.
- **Intuition**:
  - A functional graph consists of component cycles with trees rooted on cycle nodes pointing inward towards the cycle.
- **Step-by-Step Interview Walkthrough**:
  1. Identify cycles and tree branches using Kahn's indegree reduction algorithm.
  2. Compute tree depth (distance to cycle) for tree nodes and cycle position/index for cycle nodes.
  3. Query $(u, v)$:
     - If both on same tree: jump $u$ by depth diff and check if $u == v$.
     - If $u$ on tree and $v$ on cycle: jump $u$ to cycle root, then measure cycle distance to $v$.
     - If both on cycle: measure cycle distance directly.
- **Complexity**: Build: $\mathcal{O}(N \log N)$, Query: $\mathcal{O}(\log N)$, Space: $\mathcal{O}(N \log N)$.

---

## 21. Planets Cycles

- **Link**: [Planets Cycles](https://cses.fi/problemset/task/1751)
- **Problem Statement**: Given a functional graph where each node has 1 successor, calculate for every node $u$ the number of steps required to return to a previously visited node.
- **Interview Pattern**: Functional Graph Cycle Length & Tree Distance Propagation.
- **Intuition**:
  - For each node in a functional graph, the path length to revisit a node equals: (length of path to cycle) + (length of cycle).
- **Step-by-Step Interview Walkthrough**:
  1. Find cycle lengths using Floyd's cycle-finding / DFS traversal.
  2. For nodes on a cycle of size $L$, `ans[u] = L`.
  3. For tree nodes attached to cycle, `ans[u] = depth_to_cycle[u] + cycle_len`.
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 22. Road Reparation

- **Link**: [Road Reparation](https://cses.fi/problemset/task/1675)
- **Problem Statement**: Given $N$ cities and $M$ possible weighted roads, find the minimum total cost to connect all cities into a single connected network, or `IMPOSSIBLE`.
- **Interview Pattern**: Minimum Spanning Tree (Kruskal's / Prim's Algorithm).
- **Intuition**:
  - Connect all cities with minimum total edge weight. Kruskal's greedy edge selection using DSU.
- **Step-by-Step Interview Walkthrough**:
  1. Sort all $E$ edges by weight ascending.
  2. Initialize DSU for $N$ nodes.
  3. Iterate edges $(u, v, w)$: if `dsu.find(u) != dsu.find(v)`, unite them and add weight $w$ to `total_cost`.
  4. If merged edges count equals $N - 1$, return `total_cost`. Else return `IMPOSSIBLE`.
- **Pseudocode**:
```text
sort(edges by weight)
total_cost = 0, edges_count = 0
for (u, v, w) in edges:
    if dsu.unite(u, v):
        total_cost += w
        edges_count += 1

if edges_count == N - 1: return total_cost
else: return "IMPOSSIBLE"
```
- **Complexity**: Time: $\mathcal{O}(E \log E)$, Space: $\mathcal{O}(V + E)$.

---

## 23. Road Construction

- **Link**: [Road Construction](https://cses.fi/problemset/task/1676)
- **Problem Statement**: Starting with $N$ isolated cities and no roads, process $M$ road additions dynamically: after each addition, print the number of connected components and the size of the largest component.
- **Interview Pattern**: Dynamic Graph Connectivity via DSU with Component Tracking.
- **Intuition**:
  - Track connected component count and maximum component size after adding each edge dynamically.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize DSU with `components_count = N` and `max_size = 1`.
  2. For each edge $(u, v)$: if `dsu.unite(u, v)` succeeds (different components):
     - `components_count -= 1`
     - `max_size = max(max_size, new_merged_size)`
  3. Print `components_count` and `max_size`.
- **Pseudocode**:
```text
function unite(u, v):
    root_u = find(u), root_v = find(v)
    if root_u != root_v:
        parent[root_v] = root_u
        sz[root_u] += sz[root_v]
        components_count -= 1
        max_size = max(max_size, sz[root_u])
        return true
    return false
```
- **Complexity**: Time: $\mathcal{O}(M \cdot \alpha(N))$, Space: $\mathcal{O}(N)$.

---

## 24. Flight Routes Check

- **Link**: [Flight Routes Check](https://cses.fi/problemset/task/1682)
- **Problem Statement**: Given a directed graph representing flight routes between $N$ cities, determine if it is possible to travel between any pair of cities, or output two cities $a$ and $b$ such that $a$ cannot reach $b$.
- **Interview Pattern**: Strongly Connected Components (SCC) / Reachability Test via 2 DFS Passes.
- **Intuition**:
  - Graph is strongly connected iff node 1 can reach all nodes in original graph AND all nodes can reach node 1 (equivalent to 1 reaching all nodes in reversed graph).
- **Step-by-Step Interview Walkthrough**:
  1. Run DFS from node 1 on original graph $G$. If any node $X$ is unvisited, node 1 cannot reach $X$ (output `NO`, `1 X`).
  2. Run DFS from node 1 on reversed graph $G^{rev}$. If any node $Y$ is unvisited, $Y$ cannot reach node 1 (output `NO`, `Y 1`).
  3. If both pass, output `YES`.
- **Pseudocode**:
```text
dfs_original(1)
if exists unvisited node X: return NO (1 -> X impossible)

dfs_reversed(1)
if exists unvisited node Y: return NO (Y -> 1 impossible)

return YES
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 25. Planets and Kingdoms

- **Link**: [Planets and Kingdoms](https://cses.fi/problemset/task/1683)
- **Problem Statement**: Given a directed graph of $N$ planets and $M$ teleporters, partition planets into kingdoms such that two planets belong to the same kingdom iff they can reach each other (find SCCs).
- **Interview Pattern**: Strongly Connected Components (Kosaraju's / Tarjan's Algorithm).
- **Intuition**:
  - Each kingdom corresponds to a Strongly Connected Component (SCC) in a directed graph.
- **Step-by-Step Interview Walkthrough**:
  1. **Pass 1**: Run DFS on original graph to get finish order of nodes in a stack.
  2. **Pass 2**: Pop nodes from stack; if unvisited, run DFS on reversed graph to assign component kingdom ID to all reachable nodes.
  3. Output total kingdoms $K$ and component ID for each node.
- **Pseudocode**:
```text
// Step 1: Finish time order
for u = 1 to N:
    if not vis[u]: dfs1(u)

// Step 2: Assign SCC IDs on reversed graph
kingdom_count = 0
while stack not empty:
    u = stack.pop()
    if comp[u] == 0:
        kingdom_count += 1
        dfs2_reversed(u, kingdom_count)
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 26. Coin Collector

- **Link**: [Coin Collector](https://cses.fi/problemset/task/1686)
- **Problem Statement**: Given a directed graph where each room contains coins, find the maximum total coins you can collect by starting at any room and following directed tunnels.
- **Interview Pattern**: SCC Condensation DAG + Dynamic Programming.
- **Intuition**:
  - All coins in an SCC can be collected if any node in it is visited.
  - Condense each SCC into a single super-node with sum of coins as its weight; resulting graph is a DAG. Longest weighted path on DAG gives max coins.
- **Step-by-Step Interview Walkthrough**:
  1. Find SCCs using Kosaraju's algorithm.
  2. Sum coin values for all nodes in each SCC $C_i$.
  3. Build condensed DAG of SCC super-nodes.
  4. Run DP on DAG in topological order: $\text{dp}[v] = \text{coins}[v] + \max_{(u \to v)} \text{dp}[u]$.
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 27. Mail Delivery

- **Link**: [Mail Delivery](https://cses.fi/problemset/task/1691)
- **Problem Statement**: Given an undirected graph of $N$ junctions and $M$ streets, find a circuit that starts and ends at junction 1 and traverses every street exactly once, or output `IMPOSSIBLE`.
- **Interview Pattern**: Eulerian Circuit in Undirected Graph (Hierholzer's Algorithm).
- **Intuition**:
  - An Eulerian circuit exists iff graph is connected (ignoring isolated nodes) and every node has an **even degree**.
- **Step-by-Step Interview Walkthrough**:
  1. Check if all node degrees are even. If any is odd, output `IMPOSSIBLE`.
  2. Run Hierholzer's algorithm using DFS/stack: traverse edges, deleting used edges to prevent re-walking.
  3. Push node to `circuit` path upon backtracking.
  4. If `circuit.size() != M + 1`, graph was disconnected (`IMPOSSIBLE`). Otherwise print circuit.
- **Pseudocode**:
```text
function hierholzer(u):
    while adj[u] not empty:
        (v, edge_id) = adj[u].pop()
        if used_edge[edge_id]: continue
        used_edge[edge_id] = true
        hierholzer(v)
    circuit.push(u)
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 28. De Bruijn Sequence

- **Link**: [De Bruijn Sequence](https://cses.fi/problemset/task/1692)
- **Problem Statement**: Construct a binary string of minimum length that contains every binary string of length $N$ as a substring.
- **Interview Pattern**: Eulerian Path on De Bruijn Graph.
- **Intuition**:
  - Construct a directed graph where nodes are binary strings of length $N-1$ (total $2^{N-1}$ nodes), and edges represent length $N$ bit strings (transitions by adding bit 0 or 1).
  - An Eulerian circuit on this graph visits every length $N$ binary string exactly once.
- **Step-by-Step Interview Walkthrough**:
  1. Build graph with $2^{N-1}$ nodes representing bit patterns of length $N-1$.
  2. Add directed edges for appending '0' and '1'.
  3. Find Eulerian circuit using Hierholzer's algorithm starting from node 0.
  4. Append edge bits to reconstruct the minimum length string.
- **Complexity**: Time: $\mathcal{O}(2^N)$, Space: $\mathcal{O}(2^N)$.

---

## 29. Teleporters Path

- **Link**: [Teleporters Path](https://cses.fi/problemset/task/1693)
- **Problem Statement**: Given a directed graph of $N$ level nodes and $M$ teleporters, find a path that starts at node 1, ends at node $N$, and uses every teleporter edge exactly once.
- **Interview Pattern**: Directed Eulerian Path from 1 to $N$ (Hierholzer's Algorithm).
- **Intuition**:
  - Directed Eulerian Path from 1 to $N$ exists iff:
    - $\text{outdegree}[1] - \text{indegree}[1] == 1$
    - $\text{indegree}[N] - \text{outdegree}[N] == 1$
    - All other nodes have $\text{indegree} == \text{outdegree}$.
- **Step-by-Step Interview Walkthrough**:
  1. Validate in-degrees and out-degrees. If invalid, output `IMPOSSIBLE`.
  2. Run Hierholzer's algorithm from node 1.
  3. If path length is $M + 1$ and ends at $N$, print path; else `IMPOSSIBLE`.
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 30. Hamiltonian Flights

- **Link**: [Hamiltonian Flights](https://cses.fi/problemset/task/1690)
- **Problem Statement**: Given a directed graph of $N$ cities, count the number of routes from city 1 to city $N$ that visit every city **exactly once** modulo $10^9+7$.
- **Interview Pattern**: Bitmask Dynamic Programming (TSP / Hamiltonian Path).
- **Intuition**:
  - Count paths visiting every node exactly once starting at 1 and ending at $N$.
  - State: `dp[mask][u]` = number of valid paths visiting sub-set `mask` of nodes ending at node $u$.
- **Step-by-Step Interview Walkthrough**:
  1. Base case: `dp[1][0] = 1` (subset containing node 1 only).
  2. Iterate bitmask `mask` from 1 to $2^N - 1$:
     - Skip if node 1 is not in `mask` or if node $N$ is in `mask` while `mask != (1<<N)-1`.
     - For each node $u$ in `mask`, for incoming edge $(v \to u)$:
       $$\text{dp}[\text{mask}][u] = (\text{dp}[\text{mask}][u] + \text{dp}[\text{mask} \setminus \{u\}][v]) \pmod{10^9+7}$$
  3. Output `dp[(1<<N)-1][N-1]`.
- **Pseudocode**:
```text
dp[1][0] = 1
for mask = 1 to (1<<N) - 1:
    for u in mask:
        prev_mask = mask ^ (1 << u)
        for v in in_neighbors[u]:
            if (prev_mask & (1 << v)):
                dp[mask][u] = (dp[mask][u] + dp[prev_mask][v]) % MOD
```
- **Complexity**: Time: $\mathcal{O}(2^N \cdot N^2)$, Space: $\mathcal{O}(2^N \cdot N)$.

---

## 31. Download Speed

- **Link**: [Download Speed](https://cses.fi/problemset/task/1694)
- **Problem Statement**: Given a network of $N$ computers connected by directed connections with given speed capacities, calculate the maximum total download speed from computer 1 to computer $N$.
- **Interview Pattern**: Maximum Network Flow (Dinic's Algorithm / Edmonds-Karp).
- **Intuition**:
  - Maximize total flow from source 1 to sink $N$ given edge capacities. Dinic's algorithm uses BFS level graphs and DFS blocking flows.
- **Step-by-Step Interview Walkthrough**:
  1. Construct residual graph.
  2. **BFS Phase**: Build level graph `level[u]` from source 1. If sink $N$ is unreachable, stop.
  3. **DFS Phase**: Send blocking flow along shortest augmenting paths using `level` graph and pointer optimization `ptr[u]`.
  4. Accumulate total flow.
- **Pseudocode**:
```text
max_flow = 0
while bfs_build_level_graph():
    ptr[1..N] = 0
    while pushed = dfs_send_flow(source=1, sink=N, flow=INF):
        max_flow += pushed
return max_flow
```
- **Complexity**: Time: $\mathcal{O}(V^2 E)$, Space: $\mathcal{O}(V + E)$.

---

## 32. Police Chase

- **Link**: [Police Chase](https://cses.fi/problemset/task/1695)
- **Problem Statement**: Given a street network of $N$ intersections and $M$ streets, find the minimum number of streets to close off (and output which ones) to disconnect intersection 1 from intersection $N$.
- **Interview Pattern**: Max-Flow Min-Cut Theorem (Dinic's + BFS Cut Edge Extraction).
- **Intuition**:
  - Min-Cut equals Max-Flow. The minimum number of edges to cut to disconnect 1 from $N$ equals the maximum flow on a graph with unit capacity edges.
- **Step-by-Step Interview Walkthrough**:
  1. Set capacity = 1 for all original undirected edges.
  2. Run Dinic's algorithm to compute maximum flow.
  3. Run BFS on the residual graph starting from source 1 to find all reachable nodes $S$.
  4. Cut edges are all original edges $(u, v)$ where $u \in S$ and $v \notin S$.
- **Pseudocode**:
```text
compute_max_flow_dinic()

vis[1..N] = false
bfs_residual_graph(source=1) // Marks vis[u] = true for reachable nodes

for (u, v) in original_edges:
    if vis[u] and not vis[v]:
        print u, v
```
- **Complexity**: Time: $\mathcal{O}(V E^2)$, Space: $\mathcal{O}(V + E)$.
