# OA Clean Code & Bug Prevention Guide

This guide compiles essential techniques and best practices to prevent common Online Assessment (OA) issues like **Integer Overflow**, **Time Limit Exceeded (TLE)**, **Memory Limit Exceeded (MLE)**, and **Runtime Errors (RE)**.

---

## 1. Preventing Integer Overflow

Integer overflow is the most common reason for passing 8/10 test cases. Google’s test cases are designed to trigger overflows.

### Use `long long` Generously
*   When constraints or cumulative sums can exceed 2 * 10^9, use `long long` (`ll`).
*   **Safe casting:** Always multiply by `1LL` before multiplying two `int` variables.
    ```cpp
    int a = 1e6;
    int b = 1e6;
    long long c = 1LL * a * b; // Correct: cast to long long before multiplication
    // long long c = a * b;    // WRONG: 'a * b' overflows first as int, then gets assigned.
    ```

### Binary Search Invariant
*   To prevent overflow when calculating the midpoint:
    ```cpp
    int mid = lo + (hi - lo) / 2; // Correct
    // int mid = (lo + hi) / 2;   // WRONG: 'lo + hi' can overflow
    ```

### Modular Arithmetic Safeguards
*   Whenever a problem asks for `answer % MOD` (usually 10^9 + 7 or 998244353):
    *   **Addition:** `(a + b) % MOD`
    *   **Multiplication:** `(1LL * a * b) % MOD` (ensures the intermediate step does not overflow `long long`)
    *   **Subtraction:** C++ `%` operator can return negative values for negative inputs. Always wrap subtraction like this:
        ```cpp
        long long diff = (a - b) % MOD;
        long long safe_diff = (diff + MOD) % MOD;
        ```
*   **Never use `pow()`:** C++ `pow()` uses `double` precision, which introduces precision errors and overflows quickly. Use binary exponentiation:
    ```cpp
    long long binpow(long long base, long long exp) {
        long long res = 1;
        base %= MOD;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % MOD;
            base = (base * base) % MOD;
            exp /= 2;
        }
        return res;
    }
    ```

---

## 2. Preventing Time Limit Exceeded (TLE)

A standard C++ solution can perform ~ 10^8 operations per second. Keep your complexities strictly under this limit.

### Fast I/O Template
Always include this at the top of your `main()` function:
```cpp
ios_base::sync_with_stdio(false);
cin.tie(NULL);
```

### Avoid `std::endl`
*   `endl` forces a buffer flush every single time it is called, which slows down execution significantly.
*   Use `\n` or `'\n'` instead.

### Pass Large Collections by Reference
*   When passing vectors, strings, or sets to recursive functions (like DFS/BFS), **never** pass them by value. It copies the entire data structure on every recursive call, causing TLE/MLE.
    ```cpp
    void dfs(int u, const vector<vector<int>>& adj, vector<bool>& vis) { // Correct (by reference)
        // ...
    }
    ```

### String Concatenation Efficiency
*   Avoid using `+` operator on strings in loops as it creates new string objects. Use `+=` or `.push_back()`.
    ```cpp
    string s = "";
    for(int i = 0; i < n; ++i) s += "a"; // Fast
    // s = s + "a";                      // Slow: creates copies
    ```

---

## 3. Preventing Memory Limit Exceeded (MLE)

OA memory limits are usually 256MB or 512MB, but sometimes they can be as low as 64MB.

### Dynamic Programming Space Optimization
*   If your DP state transition `dp[i][j]` only depends on the previous row `dp[i-1]`, optimize space from O(N^2) to O(N) by maintaining only 1D rows.
    ```cpp
    vector<int> prev(M, 0), curr(M, 0);
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= M; ++j) {
            // Update curr based on prev
        }
        prev = curr;
    }
    ```

### Watch Out for Global Arrays / Allocations
*   Google OA platforms run multiple test cases. Static/Global structures must be explicitly cleared using `.clear()` before every run to avoid accumulating memory across test cases.

---

## 4. Preventing Runtime Errors (RE) / Segfaults

### Array & Grid Boundary Safeguards
*   Always check boundary limits *before* indexing:
    ```cpp
    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !visited[nr][nc]) { ... }
    ```
    *(C++ evaluates conditions left-to-right. If the boundary check fails, the index check is skipped, preventing a segfault.)*

### Graph Vertices Indexing (Off-by-One)
*   Look at whether nodes are `0`-indexed or `1`-indexed in the input description.
*   If they are `1`-indexed, size your vectors to `N + 1` to prevent out-of-bounds access.
    ```cpp
    vector<vector<int>> adj(N + 1);
    ```

### Prevent Recursion Stack Overflow
*   If recursion depth can go up to 10^5 or 2 * 10^5, implement your solution iteratively (e.g. iterative DFS or BFS) or optimize recursion stack usage by not declaring large objects locally inside recursive functions.

---

## 5. Using Global / Class-Member Containers in LeetCode/OA Templates

When coding in LeetCode-style templates (where your code is inside a `class Solution`), passing references like `vector<vector<int>>& adj` or `vector<bool>& visited` to every recursive helper function creates significant writing overhead. 

To speed up coding and keep helper functions clean, you can use **Class-Member variables** or **Global variables**. 

### ⚠️ The Golden Rule: Reset containers for every test case!
LeetCode and OA platforms instantiate your class once or run test cases in a loop over the same execution lifetime. If you do not reset your containers, the data from the **previous test case** will persist, leading to **Wrong Answer (WA)** or **Runtime Error (RE/Segfault)**.

---

### Approach A: Class-Member Variables (Highly Recommended)
Declare your helper structures as private/public member variables of the class. Re-initialize them in the main entry function before triggering any logic.

```cpp
class Solution {
private:
    // Declare containers here to avoid passing them as function arguments
    vector<vector<int>> adj;
    vector<bool> visited;
    int max_depth = 0;

    void dfs(int u, int depth) {
        visited[u] = true;
        max_depth = max(max_depth, depth);
        for (int v : adj[u]) {
            if (!visited[v]) {
                dfs(v, depth + 1);
            }
        }
    }

public:
    int maxDepth(int n, vector<vector<int>>& edges) {
        // CRITICAL: You MUST clear and resize class-member variables on every call
        adj.assign(n + 1, vector<int>());
        visited.assign(n + 1, false);
        max_depth = 0;

        for (auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }

        dfs(1, 1);
        return max_depth;
    }
};
```

---

### Approach B: Global Variables (Alternative)
If you prefer declaring variables in the global namespace (outside the class), you must clear them inside the class's entry function.

```cpp
// Global declarations
vector<vector<int>> adj;
vector<bool> visited;

class Solution {
private:
    void dfs(int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) dfs(v);
        }
    }

public:
    void solve(int n, vector<vector<int>>& edges) {
        // CRITICAL: Clear/Reset global variables before use
        adj.clear();
        adj.resize(n + 1);
        visited.assign(n + 1, false);

        for (auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
        }
        dfs(1);
    }
};
```

---

## 6. Most Used Ways to Initialize Each Container in C++

Quick reference for the most common and fastest initialization syntax to save writing time during the OA.

### 1. `std::vector` (Dynamic Arrays & Grids)
*   **Size N filled with default value (e.g., 0):**
    ```cpp
    vector<int> vec(n, 0);
    ```
*   **Direct values (Initializer List):**
    ```cpp
    vector<int> vec = {1, 2, 3, 4};
    ```
*   **2D Grid of size R x C filled with -1:**
    ```cpp
    vector<vector<int>> grid(R, vector<int>(C, -1));
    ```
*   **Copying another vector:**
    ```cpp
    vector<int> copy_vec(original_vec);
    ```

### 2. `std::string`
*   **Size N filled with character 'a':**
    ```cpp
    string s(n, 'a');
    ```
*   **String from substring:**
    ```cpp
    string sub = original.substr(start_index, length);
    ```

### 3. `std::unordered_set` / `std::set`
*   **Initializer List:**
    ```cpp
    unordered_set<int> st = {1, 2, 3};
    ```
*   **Initialize from a vector (Crucial for filtering duplicates fast):**
    ```cpp
    unordered_set<int> st(vec.begin(), vec.end());
    ```

### 4. `std::unordered_map` / `std::map`
*   **Initializer List:**
    ```cpp
    unordered_map<string, int> mp = {{"key1", 1}, {"key2", 2}};
    ```

### 5. `std::priority_queue` (Heaps)
*   **Max-Heap (default):**
    ```cpp
    priority_queue<int> max_heap;
    ```
*   **Min-Heap:**
    ```cpp
    priority_queue<int, vector<int>, greater<int>> min_heap;
    ```
*   **Heapify a vector in O(N) (Fastest way to initialize from a list):**
    ```cpp
    priority_queue<int> pq(vec.begin(), vec.end());
    ```

### 6. `std::pair` & `std::tuple`
*   **Pairs:**
    ```cpp
    pair<int, string> p = {10, "apple"}; // Direct initializer list
    // or
    auto p = make_pair(10, "apple");
    ```
*   **Tuples (For storing 3 or more values per entry in heaps/queues):**
    ```cpp
    tuple<int, int, int> t = {10, 20, 30};
    // or
    auto t = make_tuple(10, 20, 30);
    ```

### 7. C-style Arrays
*   **Global array zero-initialized:**
    ```cpp
    int arr[100005] = {0};
    int grid[1005][1005] = {{0}};
    ```

---

## 7. How to Clear / Resize / Re-initialize Each Container in C++

Different C++ containers have different methods for resetting. Knowing the exact syntax prevents compiler errors and memory leaks.

### 1. `std::vector` (Dynamic Arrays / Adjacency Lists)
*   **Clear to size 0 (capacity remains):**
    ```cpp
    vec.clear();
    ```
*   **Resize and fill with default values (e.g., set size to N with all `0`s or `false`):**
    ```cpp
    vec.assign(n, 0); // Sets size to n and fills with 0
    ```
*   **Reset a Graph Adjacency List of size N:**
    ```cpp
    adj.assign(n + 1, vector<int>()); // Resizes to n+1 and clears each sub-vector
    ```

### 2. `std::string`
*   **Clear to empty string:**
    ```cpp
    s.clear();
    // or
    s = "";
    ```

### 3. Associative Containers (`std::map`, `std::set`, `std::unordered_map`, `std::unordered_set`)
*   **Clear all entries:**
    ```cpp
    mp.clear();
    st.clear();
    ```

### 4. Container Adapters (`std::queue`, `std::stack`, `std::priority_queue`)
⚠️ **C++ Gotcha:** None of these adapters have a `.clear()` member function. 
*   **Correct way to reset (re-assign to a new empty instance):**
    ```cpp
    q = queue<int>();
    s = stack<int>();
    pq = priority_queue<int>();
    min_pq = priority_queue<int, vector<int>, greater<int>>();
    ```
*   **Alternative way (swapping with an empty container):**
    ```cpp
    queue<int> empty_q;
    swap(q, empty_q);
    ```

### 5. C-style Arrays (`int arr[N]` or `int grid[R][C]`)
*   **Reset to `0` or `-1` (fastest using `memset`):**
    ```cpp
    memset(arr, 0, sizeof(arr));
    memset(grid, -1, sizeof(grid)); // memset ONLY works reliably for 0 and -1
    ```
*   **Reset to any custom value (using `std::fill`):**
    ```cpp
    fill(arr, arr + n, custom_value);
    // For 2D grid:
    for (int i = 0; i < rows; ++i) {
        fill(grid[i], grid[i] + cols, custom_value);
    }
    ```

---

## Quick Reference / OA Cheat Sheet

| Situation | Potential Issue | Solution |
| :--- | :--- | :--- |
| Multiplying two `int`s | Integer Overflow | Multiply by `1LL` first: `1LL * a * b` |
| Calculating `mid` in BS | Integer Overflow | `mid = lo + (hi - lo) / 2` |
| Negative differences in `% MOD` | Negative result | `(diff + MOD) % MOD` |
| Large inputs (N >= 10^5) | TLE | Add Fast I/O, replace `endl` with `'\n'` |
| Recursion on massive structures | TLE / MLE | Pass vectors/strings by `const &` reference |
| Grid/Graph traversal | Runtime Error (Segfault) | Check boundary variables before accessing indices |
| Class-based / Global variables | Wrong Answer / RE | Re-initialize/reset containers (`.clear()`, `.assign()`) inside the entry function |
| Resetting queues/stacks/heaps | Compile Error | Re-assign to empty instance: `q = queue<int>();` |

