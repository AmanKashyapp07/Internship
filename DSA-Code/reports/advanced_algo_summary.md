# Range Query & Update Cheat Sheet (What to Use When)

Use this quick-reference guide during OAs to select the absolute fastest and simplest data structure based on the problem's updates and operations.

---

## 1. Decision Matrix

| Updates Type | Operation Type | Best Structure | Query Time | Update Time | Space | Coding Time |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **No Updates** (Static) | **Invertible** (Sum, XOR, Product) | **Prefix Array** | **$O(1)$** | Not Supported | $O(N)$ | 1 min |
| **No Updates** (Static) | **Non-Invertible** (Min, Max, GCD) | **Sparse Table** | **$O(1)$** | Not Supported | $O(N \log N)$ | 5 mins |
| **Point Updates** | **Invertible** (Sum, XOR) | **Fenwick Tree (BIT)** | $O(\log N)$ | $O(\log N)$ | $O(N)$ | 3 mins |
| **Point Updates** | **Non-Invertible** (Min, Max, GCD) | **Segment Tree** | $O(\log N)$ | $O(\log N)$ | $O(4N)$ | 8 mins |
| **Range Updates** | **Any Operation** (Sum, Min, Max) | **Lazy Segment Tree** | $O(\log N)$ | $O(\log N)$ | $O(4N)$ | 12 mins |

*   **Invertible operations:** Operations that have an "undo" operator (e.g., `+` has `-`, `^` has `^`).
*   **Non-Invertible operations:** Operations where information is lost during combination (e.g., `min`, `max`, `gcd`).
*   **Large Coordinates Check:** If coordinate values are up to $10^9$ or $10^{12}$ but elements/queries $N, Q \le 10^5$, always apply **Coordinate Compression** first to reduce memory scale to $O(N + Q)$, then build the chosen tree on the compressed ranks.

---

## 2. Fast Identification Flowchart

1.  **Are the coordinate/index values massive (e.g., $\le 10^9$) but total inputs are small (e.g., $\le 10^5$)?**
    *   **YES:** Run **Coordinate Compression** first to map coordinates to ranks $[0, \text{unique\_size}-1]$.
    *   **NO:** Proceed directly to step 2.

2.  **Does the array get modified/updated?**
    *   **NO (Static):**
        *   Is the query looking for **Sum / XOR**? $\rightarrow$ **Use Prefix Array** ($O(1)$ query).
        *   Is the query looking for **Min / Max / GCD**? $\rightarrow$ **Use Sparse Table** ($O(1)$ query).
    *   **YES (Dynamic):**
        *   Are the updates **Point Updates** (modifying a single index)?
            *   Is the query looking for **Sum / XOR**? $\rightarrow$ **Use Fenwick Tree / BIT** (fastest code, minimal memory).
            *   Is the query looking for **Min / Max / GCD**? $\rightarrow$ **Use Segment Tree**.
        *   Are the updates **Range Updates** (modifying a range of indices $[L, R]$)? $\rightarrow$ **Use Lazy Segment Tree**.

---

## 3. Top Pitfalls to Avoid in OAs
*   **Memory Limit TLE/MLE:** Standard Segment Trees require $4 \times N$ array sizes. If memory limits are very tight (e.g., $N = 10^7$ or 64MB limit), a Segment Tree might trigger MLE. Prefer **Fenwick Tree** ($1 \times N$ size) if the operation is invertible.
*   **`std::endl` TLE:** For questions requiring printing $10^5$ query answers, never use `std::endl` because it flushes the system buffer. Always use `'\n'`.
*   **Segment Tree 0-based vs 1-based indexing:** Standard templates use 0-based indexing for the array, but internally index nodes from `1` (root) to easily transition to children via `2*node` and `2*node+1`. Always ensure your recursive parameters use `0` to `n-1` for bounds.
*   **Offline Coordinate Compression Trap:** When performing coordinate compression, you must compress **both** the initial values **and** the query bounds ($L$ and $R$). If you only compress the initial values, any query spanning coordinates not present in the original array will fail to map.

