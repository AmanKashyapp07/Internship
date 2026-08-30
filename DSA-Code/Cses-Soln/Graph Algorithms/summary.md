# CSES Graph Algorithms - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 3 problems in the **Graph Algorithms** topic.

---

## 1. Planets Cycles

- **Tier**: Tier 2
- **Link**: [Planets Cycles](https://cses.fi/problemset/task/1751)
- **Problem Statement**: Given a functional graph where each node has 1 successor, calculate for every node $u$ the number of steps required to return to a previously visited node.
- **Interview Pattern**: Functional Graph Cycle Length & Tree Distance Propagation.
- **Intuition**:
  - For each node in a functional graph, the path length to revisit a node equals: (length of path to cycle) + (length of cycle).
- **Step-by-Step Interview Walkthrough**:
  1. Find cycle lengths using Floyd's cycle-finding / DFS traversal.
  2. For nodes on a cycle of size $L$, `ans[u] = L`.
  3. For tree nodes attached to cycle, `ans[u] = depth_to_cycle[u] + cycle_len`.
- **Pseudocode**:
```text
for u = 1 to N:
    if not vis[u]:
        path = [], curr = u
        while not vis[curr]:
            vis[curr] = true, path.push(curr)
            curr = successor[curr]
        if curr in path:
            cycle_start = index_of(curr in path)
            cycle_len = length(path) - cycle_start
            for node in path[cycle_start..]: ans[node] = cycle_len
            for i = cycle_start - 1 down to 0: ans[path[i]] = ans[path[i+1]] + 1
        else:
            for i = length(path) - 1 down to 0: ans[path[i]] = ans[successor[path[i]]] + 1
return ans
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 2. Coin Collector

- **Tier**: Tier 3
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
- **Pseudocode**:
```text
scc_id = kosaraju_scc()
for u = 1 to N:
    scc_coins[scc_id[u]] += coins[u]
    for v in adj[u]:
        if scc_id[u] != scc_id[v]: scc_adj[scc_id[u]].add(scc_id[v])

dp = copy(scc_coins)
for c in topo_sort(scc_adj):
    for nxt in scc_adj[c]:
        dp[nxt] = max(dp[nxt], dp[c] + scc_coins[nxt])
return max(dp)
```
- **Complexity**: Time: $\mathcal{O}(V + E)$, Space: $\mathcal{O}(V + E)$.

---

## 3. Hamiltonian Flights

- **Tier**: Tier 3
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
