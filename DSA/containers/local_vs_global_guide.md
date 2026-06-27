# C++ Cheat Sheet: Class-Member "Global" Containers in LeetCode-style OAs

In LeetCode-style Online Assessments (like CodeSignal, HackerRank, and LeetCode itself), code is wrapped inside a `Solution` class. 

Instead of passing massive vectors through recursive helper functions (which causes stack clutter and minor performance overhead), the best practice is to declare **class member variables** (often referred to as "global vectors inside the class").

This guide details how to use this pattern, how to avoid common traps, and how to optimize it for speed.

---

## 1. The "Global inside Class" Pattern

By declaring containers as private members of the `Solution` class, they are visible to all helper functions (like `dfs` or `bfs`) without being passed by reference.

### Recommended Template
```cpp
class Solution {
private:
    // "Global" containers inside the class scope
    vector<vector<int>> adj;
    vector<bool> visited;
    vector<int> memo;

    void dfs(int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                dfs(v);
            }
        }
    }

public:
    int solve(int n, vector<vector<int>>& edges) {
        // ⚠️ CRITICAL RESET: Must re-initialize containers on every function call
        adj.assign(n, vector<int>());
        visited.assign(n, false);
        memo.assign(n, -1);

        // Build Graph
        for (const auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
        }

        dfs(0);
        return memo[n - 1];
    }
};
```

---

## 2. Why This is the Best Practice for OAs

1. **Syntactic Cleanliness**: Helper functions like `dfs(int u)` do not need to carry long, bloated signatures (e.g. `dfs(int u, vector<vector<int>>& adj, vector<bool>& visited, vector<int>& memo)`).
2. **Speed (Memory Reuse)**: In C++, calling `.assign(n, val)` on a class member vector reuses the vector's already-allocated capacity under the hood. It avoids the operating system call to release and request heap memory, making it significantly faster than declaring `vector<int> visited(n, false)` inside the function on every test case.
3. **Instance-Safe**: Unlike traditional file-scope globals (outside the class), class members are tied to the specific instance of `Solution`. This prevents interference if the testing harness instantiates multiple objects.

---

## 3. The 3 Golden Rules of Class Containers

### Rule 1: Always Reset at the Entry Function (WA Prevention)
Many OA platforms call your method multiple times sequentially on the same class instance. If you do not reset the vectors, data from previous test cases will persist.
* **Do NOT do this:** Relying on default constructor values or using only `.clear()` on nested vectors.
* **DO this:** Use `.assign()` to resize and reinitialize all elements:
  ```cpp
  adj.assign(n, vector<int>()); // Clears previous values & resizes
  visited.assign(n, false);     // Resets all visited markers to false
  memo.assign(n, -1);           // Resets DP table
  ```

### Rule 2: Avoid the `static` Keyword
Do not declare class member variables as `static`. Static members are shared across *all* instances of the class. If the testing platform runs tests in parallel or reuses instances, static variables will bleed state, leading to unpredictable Wrong Answers.
```cpp
class Solution {
    // ❌ DANGEROUS: Will cause WA on LeetCode/OAs due to shared state
    static vector<int> memo; 
    
    //  SAFE: Normal member variable
    vector<int> memo; 
};
```

### Rule 3: 2D Grid DFS / BFS Resetting
For 2D grid DP or search algorithms (e.g. size $R \times C$), use `.assign()` with nested structures to clean and set sizes:
```cpp
class Solution {
    vector<vector<int>> memo;
    vector<vector<bool>> visited;

public:
    int minPathSum(vector<vector<int>>& grid) {
        int r = grid.size();
        int c = grid[0].size();

        // Safely reset 2D class containers
        memo.assign(r, vector<int>(c, -1));
        visited.assign(r, vector<bool>(c, false));
        
        // ...
    }
};
```

---

## 4. How to Reassign / Reset Each Container Type

When re-initializing containers at the top of your solver function, use the following syntax patterns based on the container type:

### 1. Vector (`std::vector<T>`)
Do **not** use `v = vector<int>(n, val)` because it allocates new memory. Use `.assign()` to reuse pre-allocated capacity:
```cpp
// 1D Vector
v.assign(n, val); // resizes to n, fills with val

// 2D Vector (e.g., Grid)
grid.assign(rows, vector<int>(cols, val));
```

### 2. Set & Map (`std::set`, `std::map`, `std::unordered_set`, `std::unordered_map`)
These containers do not have an `assign` method. You must use `.clear()`:
```cpp
s.clear(); // Empty the set
mp.clear(); // Empty the map
```

### 3. Stack & Queue (`std::stack`, `std::queue`)
These adaptor containers do **not** have a `.clear()` method. To reassign/empty them:
```cpp
st = stack<int>(); // Reassigns to a fresh, empty stack
q = queue<int>();   // Reassigns to a fresh, empty queue
```

### 4. Priority Queue (`std::priority_queue`)
Priority queues do **not** have a `.clear()` method. Reassign them based on their type:
```cpp
// Standard Max Heap
pq = priority_queue<int>();

// Standard Min Heap
min_pq = priority_queue<int, vector<int>, greater<int>>();

// Custom Comparator Heap (Declaring lambda comparator)
pq_custom = priority_queue<int, vector<int>, decltype(cmp)>(cmp);
```

### 5. Primitive Arrays / Tables (`int arr[1005]`)
For raw C-style arrays declared inside the class:
```cpp
// Reset with std::fill (Type-safe)
fill(arr, arr + n, val);

// Reset with memset (Very fast, but works best for 0 or -1 values)
memset(arr, -1, sizeof(arr)); 
```

---

## 5. Summary: Local vs. Class-Member vs. Global

| Feature | Local Containers | Class-Member "Global" | File-Scope Global |
| :--- | :--- | :--- | :--- |
| **Passed as Arguments?** | Yes (clutters helper functions) | No (accessed directly) | No (accessed directly) |
| **OA Allocation Speed** | Slow (allocates on every call) | **Fast** (reuses capacity) | **Fastest** (allocated once) |
| **Multi-Test Case Safety** | Safe (destroyed on return) | **Safe if `.assign()` / reset is used** | Dangerous (must `memset`/`clear`) |
| **LeetCode Class Safe?** | Yes | **Yes** | Dangerous (shared state) |
| **Stack Overflow Risk** | High (if using local stack arrays) | **None** (allocates on heap) | **None** (allocates in static data) |
