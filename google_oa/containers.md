# C++ STL Containers Operations Cheat Sheet

A quick reference guide for the most common C++ STL containers, their primary operations, syntaxes, and average time complexities.

---

## 1. `std::vector` (Dynamic Array)
*Underlying structure: Dynamic contiguous array.*

| Operation | Syntax | Time Complexity |
| :--- | :--- | :--- |
| **Add to end** | `vec.push_back(val);` | $O(1)$ amortized |
| **Remove from end** | `vec.pop_back();` | $O(1)$ |
| **Insert at index** | `vec.insert(vec.begin() + index, val);` | $O(N)$ |
| **Erase at index** | `vec.erase(vec.begin() + index);` | $O(N)$ |
| **Access element** | `vec[i]` or `vec.at(i)` | $O(1)$ |
| **Front element** | `vec.front();` | $O(1)$ |
| **Back element** | `vec.back();` | $O(1)$ |
| **Reset/Fill size** | `vec.assign(size, value);` | $O(N)$ |
| **Clear all** | `vec.clear();` | $O(N)$ |

*   **Key Tip:** Use `vec.reserve(size)` before inserting elements if the final size is known to prevent multiple dynamic reallocations.

---

## 2. `std::string`
*Underlying structure: Dynamic array of characters.*

| Operation | Syntax | Time Complexity |
| :--- | :--- | :--- |
| **Append character** | `s.push_back('c');` or `s += 'c';` | $O(1)$ amortized |
| **Pop last character** | `s.pop_back();` | $O(1)$ |
| **Substring** | `s.substr(start_index, length);` | $O(\text{length})$ |
| **Find substring** | `s.find("target");` (returns `string::npos` if not found) | $O(N \cdot M)$ |
| **Compare strings** | `s1 == s2` | $O(N)$ |
| **String to Int** | `stoi(s)` or `stoll(s)` (for `long long`) | $O(\text{length})$ |
| **Int to String** | `to_string(num)` | $O(\log_{10} \text{num})$ |

---

## 3. `std::deque` (Double-Ended Queue)
*Underlying structure: Non-contiguous chunked memory buffers.*

| Operation | Syntax | Time Complexity |
| :--- | :--- | :--- |
| **Insert at back** | `dq.push_back(val);` | $O(1)$ |
| **Insert at front** | `dq.push_front(val);` | $O(1)$ |
| **Remove from back** | `dq.pop_back();` | $O(1)$ |
| **Remove from front** | `dq.pop_front();` | $O(1)$ |
| **Access element** | `dq[i]` or `dq.at(i)` | $O(1)$ |
| **Front element** | `dq.front();` | $O(1)$ |
| **Back element** | `dq.back();` | $O(1)$ |

*   **Key Use Case:** Ideal for **sliding window maximum/minimum** (monotonic queue) and **0-1 BFS** (where 0-weight edges are pushed to the front and 1-weight edges to the back).

---

## 4. `std::queue` & `std::stack`
*Underlying structure: Adapters (usually built on top of `std::deque` or `std::vector`).*

### `std::queue` (FIFO)
*   **Insert at back:** `q.push(val);` — $O(1)$
*   **Remove from front:** `q.pop();` — $O(1)$
*   **Access front:** `q.front();` — $O(1)$
*   **Access back:** `q.back();` — $O(1)$

### `std::stack` (LIFO)
*   **Insert at top:** `st.push(val);` — $O(1)$
*   **Remove from top:** `st.pop();` — $O(1)$
*   **Access top:** `st.top();` — $O(1)$

*   **Reset Trick:** There is no `.clear()` for queue/stack. Use: `q = queue<int>();` or `st = stack<int>();`.

---

## 5. `std::priority_queue` (Heap / Priority Queue)
*Underlying structure: Binary Heap (array-based).*

| Operation | Syntax | Time Complexity |
| :--- | :--- | :--- |
| **Insert element** | `pq.push(val);` | $O(\log N)$ |
| **Remove top** | `pq.pop();` | $O(\log N)$ |
| **Access top** | `pq.top();` | $O(1)$ |
| **Size** | `pq.size();` | $O(1)$ |

*   **Heapify in $O(N)$:** `priority_queue<int> pq(vec.begin(), vec.end());` (much faster than inserting one-by-one).
*   **Min-Heap declaration:**
    ```cpp
    priority_queue<int, vector<int>, greater<int>> min_heap;
    ```

---

## 6. `std::set` & `std::map` (Ordered / Balanced BST)
*Underlying structure: Red-Black Tree (Self-balancing BST).*

| Operation | Syntax | Time Complexity |
| :--- | :--- | :--- |
| **Insert** | `st.insert(val);` or `mp[key] = val;` | $O(\log N)$ |
| **Erase** | `st.erase(val);` or `mp.erase(key);` | $O(\log N)$ |
| **Search/Find** | `st.find(val);` (returns iterator, `st.end()` if missing) | $O(\log N)$ |
| **Check presence** | `st.count(val);` (returns `1` or `0`) | $O(\log N)$ |
| **Lower Bound** | `st.lower_bound(val);` (first element `>= val`) | $O(\log N)$ |
| **Upper Bound** | `st.upper_bound(val);` (first element `> val`) | $O(\log N)$ |

*   **Key Tip:** Set/Map elements are always kept in **sorted order**. Iterating over them traverses elements in ascending order.
*   **C++ Gotcha:** Do NOT use `std::lower_bound(st.begin(), st.end(), val)`. That runs in $O(N)$ for sets/maps. Always use the member function: `st.lower_bound(val)`.

---

## 7. `std::unordered_set` & `std::unordered_map` (Hash Table)
*Underlying structure: Hash Map with chained buckets.*

| Operation | Syntax | Time Complexity (Average) | Time Complexity (Worst-case) |
| :--- | :--- | :--- | :--- |
| **Insert** | `ust.insert(val);` or `ump[key] = val;` | $O(1)$ | $O(N)$ |
| **Erase** | `ust.erase(val);` or `ump.erase(key);` | $O(1)$ | $O(N)$ |
| **Search/Find** | `ust.find(val);` | $O(1)$ | $O(N)$ |
| **Check presence** | `ust.count(val);` | $O(1)$ | $O(N)$ |

*   **Worst-case TLE Risk:** In Google OAs, test cases are designed to trigger hash collisions on standard hash tables, causing `unordered_map` operations to degrade to $O(N)$. If you get TLE, switch to `std::map` ($O(\log N)$ guaranteed) or use a custom hash.
