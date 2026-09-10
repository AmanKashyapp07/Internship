# The Microsoft Interview 1-Day Revision Guide
### Top 27 Core DSA Patterns to Recognize Cold

This is a recognition guide, not a study guide. The night before, you are not learning these patterns for the first time — you are refreshing the trigger words that tell you which one applies. Skim the index below first. Only open the full entry for a pattern if the invariant doesn't come back to you immediately.

---

## How to Use This Tonight

1. Read the **Pattern Recognition Index** below top to bottom. For each row, cover the "Pattern" column and see if the trigger phrase alone brings the approach to mind.
2. Any row where it doesn't — jump to that numbered section, reread the invariant and template once, then move on. Don't re-derive the whole thing from scratch.
3. Finish with the **Edge Case Checklist** and the **45-Second Recall List** at the very end. That list is what you should be able to reproduce from memory tomorrow morning.
4. Pair this with your verbal playbook — the code here is the *what*; the other guide is the *how you talk through it*.

---

## Pattern Recognition Index

| # | Trigger Phrase in the Problem | Pattern | One-Line Invariant |
|---|---|---|---|
| 1 | Subarray sum equals K, sum divisible by K, **negative numbers present** | Prefix Sum + Hash Map | `prefix[i-1] = prefix[j] - K` |
| 2 | Longest substring without repeating characters | Sliding Window (Last-Seen Index) | Jump `left` past the duplicate's last index |
| 3 | Minimum window substring, permutation/anagram in string | Sliding Window (Match Counter) | Track `have == need` instead of full frequency comparison |
| 4 | Container with most water, trapping rain water, 3Sum | Converging Two Pointers | Result bound by the **shorter** boundary — move it |
| 5 | Search/find minimum in rotated sorted array | Rotated Binary Search | One half of `[left..mid..right]` is always sorted |
| 6 | Minimum speed/capacity/threshold satisfying a condition | Binary Search on Answer | Predicate is monotonic: false...false, true...true |
| 7 | Detect cycle, cycle start, middle of linked list | Fast & Slow Pointers | Fast moves 2x; meeting point implies a cycle |
| 8 | Reverse linked list, reverse in k-groups | In-Place Pointer Reversal | Save `next` before rewiring `curr->next` |
| 9 | Design a cache with O(1) get/put and eviction | LRU Cache (DLL + Hash Map) | MRU at head, LRU at tail, map gives O(1) node access |
| 10 | Median of a data stream | Two Heaps | maxHeap holds the smaller half, minHeap the larger half |
| 11 | Max path sum, diameter, LCA in a tree | Post-Order Contribution | Compute child gains bottom-up, update global at the "turn" |
| 12 | Validate BST, recover BST | BST Range DFS | Every node must satisfy `low < val < high`, passed down |
| 13 | Level order, right-side view, zigzag | BFS with Level-Size Snapshot | Snapshot `queue.size()` before processing a level |
| 14 | Serialize/deserialize a tree | Preorder + Null Sentinel | Preorder with explicit `#` uniquely rebuilds topology |
| 15 | Number of islands, word search, flood fill | Grid In-Place Sinking | Mark visited by mutating the grid itself, no extra space |
| 16 | Rotting oranges, 01 Matrix, shortest bridge | Multi-Source BFS | Push all sources at t=0, expand level by level together |
| 17 | Course schedule, dependency order, cycle in DAG | Topological Sort (Kahn's) | Processed count < N means a cycle exists |
| 18 | Daily temperatures, next greater element, histogram | Monotonic Stack | Decreasing stack -> next greater; increasing -> boundaries |
| 19 | Merge intervals, meeting rooms, insert interval | Interval Sweep | Sort by start to merge, by end to schedule maximally |
| 20 | Coin change, word break, decode ways, LIS | 1D Dynamic Programming | `dp[i]` depends only on strictly earlier subproblems |
| 21 | Connected components, redundant connection, accounts merge | Union-Find (DSU) | Union by root; component count drops by 1 per successful merge |
| 22 | Kth largest/smallest, top K frequent elements | Heap-Based Top-K | Maintain a heap of size K; evict the worst when it overflows |
| 23 | Generate all subsets, permutations, or combinations | Backtracking (Choose / Explore / Un-choose) | Mutate state, recurse, then undo the mutation on return |
| 24 | Edit distance, longest common subsequence, distinct subsequences | 2D Grid Dynamic Programming | `dp[i][j]` compares prefixes of length i and j from each string |
| 25 | Prefix search, autocomplete, word search II | Trie (Prefix Tree) | Each node branches per character; `isEnd` marks a complete word |
| 26 | Shortest path with weighted edges (non-negative) | Dijkstra's Algorithm | Greedily relax via a min-heap; lazily skip stale, outdated entries |
| 27 | Sliding window maximum, next larger element in a window | Monotonic Deque | Front of deque is always the current window's max index |

---

## The 27 Patterns in Detail

---

### 1. Prefix Sum + Hash Map Frequency
**When to use:** Count or find subarrays with sum = K or sum divisible by K, especially with negative numbers present.
**Core invariant:** `sum(i..j) = prefix[j] - prefix[i-1] = K` implies `prefix[i-1] = prefix[j] - K`.
```cpp
unordered_map<int, int> prefixCount;
prefixCount[0] = 1; // Base case: empty prefix before array starts
int sum = 0, count = 0;
for (int x : nums) {
    sum += x;
    if (prefixCount.count(sum - k)) count += prefixCount[sum - k];
    prefixCount[sum]++;
}
```
**Archetypes:** Subarray Sum Equals K, Subarray Sums Divisible by K, Path Sum III.
**Trap:** Never use a two-pointer sliding window when negative numbers are present — monotonicity is broken.
**Say out loud:** "Because negative numbers break the monotonicity a sliding window relies on, I'll track prefix sums in a hash map instead."

---

### 2. Dynamic Sliding Window (Last-Seen Index)
**When to use:** Longest substring without repeating characters, or with at most K distinct characters.
**Core invariant:** When a duplicate of character `c` is seen at `lastIdx[c] >= left`, jump `left = lastIdx[c] + 1` immediately.
```cpp
vector<int> lastIdx(128, -1);
int left = 0, maxLen = 0;
for (int right = 0; right < (int)s.size(); ++right) {
    char c = s[right];
    if (lastIdx[c] >= left) left = lastIdx[c] + 1; // Jump past duplicate
    lastIdx[c] = right;
    maxLen = max(maxLen, right - left + 1);
}
```
**Archetypes:** Longest Substring Without Repeating Characters.
**Say out loud:** "Instead of shrinking the window step by step in an inner while loop, I'll record each character's last-seen index so the left pointer can jump in O(1)."

---

### 3. Sliding Window with Match Counter (have == need)
**When to use:** Minimum window substring, permutation in string, anagram matching.
**Core invariant:** Track unique matched characters with an integer `have` against `need = targetFreq.size()`, avoiding an O(26) comparison every step.
```cpp
unordered_map<char, int> targetFreq, windowFreq;
for (char c : t) targetFreq[c]++;
int have = 0, need = targetFreq.size(), left = 0, minLen = INT_MAX, start = -1;

for (int right = 0; right < (int)s.size(); ++right) {
    char c = s[right];
    windowFreq[c]++;
    if (targetFreq.count(c) && windowFreq[c] == targetFreq[c]) have++;

    while (have == need) {
        if (right - left + 1 < minLen) { minLen = right - left + 1; start = left; }
        char leftChar = s[left];
        windowFreq[leftChar]--;
        if (targetFreq.count(leftChar) && windowFreq[leftChar] < targetFreq[leftChar]) have--;
        left++;
    }
}
```
**Archetypes:** Minimum Window Substring, Find All Anagrams in a String.

---

### 4. Converging Two Pointers (Bottleneck Invariant)
**When to use:** 3Sum, Container With Most Water, Trapping Rain Water.
**Core invariant:** Area or trapped water is bound by the shorter boundary. Moving the taller one can never help — you must move the shorter pointer inward.
```cpp
int left = 0, right = (int)height.size() - 1, leftMax = 0, rightMax = 0, water = 0;
while (left < right) {
    if (height[left] <= height[right]) {
        if (height[left] >= leftMax) leftMax = height[left];
        else water += leftMax - height[left];
        left++;
    } else {
        if (height[right] >= rightMax) rightMax = height[right];
        else water += rightMax - height[right];
        right--;
    }
}
```
**Archetypes:** Trapping Rain Water, Container With Most Water, 3Sum (with sorting and deduplication).

---

### 5. Rotated Sorted Binary Search (Sorted-Half Invariant)
**When to use:** Search or find minimum in a rotated sorted array.
**Core invariant:** At any midpoint `mid`, at least one half — `[left..mid]` or `[mid..right]` — is strictly sorted.
```cpp
int left = 0, right = (int)nums.size() - 1;
while (left <= right) {
    int mid = left + (right - left) / 2;
    if (nums[mid] == target) return mid;
    if (nums[left] <= nums[mid]) { // Left half sorted
        if (target >= nums[left] && target < nums[mid]) right = mid - 1;
        else left = mid + 1;
    } else { // Right half sorted
        if (target > nums[mid] && target <= nums[right]) left = mid + 1;
        else right = mid - 1;
    }
}
```
**Archetypes:** Search in Rotated Sorted Array, Find Minimum in Rotated Sorted Array.

---

### 6. Binary Search on Monotonic Answer Range
**When to use:** Find the minimum speed, capacity, or threshold such that condition F(X) holds.
**Core invariant:** Predicate F(X) is monotonic — `[false, false, ..., true, true]` — over the search range `[low, high]`.
```cpp
int low = 1, high = *max_element(piles.begin(), piles.end());
int ans = high;
while (low <= high) {
    int mid = low + (high - low) / 2;
    if (canFinish(mid, h)) {
        ans = mid;
        high = mid - 1; // Try a smaller value
    } else {
        low = mid + 1;
    }
}
```
**Archetypes:** Koko Eating Bananas, Capacity To Ship Packages Within D Days, Split Array Largest Sum.

---

### 7. Floyd's Fast & Slow Pointers (Tortoise & Hare)
**When to use:** Detect a cycle, find the cycle start, find the linked list middle.
**Core invariant:** Fast moves twice as fast as slow. If a cycle of length C exists, the relative distance shrinks by 1 each step.
```cpp
ListNode* detectCycleStart(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next; fast = fast->next->next;
        if (slow == fast) {
            slow = head;
            while (slow != fast) { slow = slow->next; fast = fast->next; }
            return slow; // Cycle start
        }
    }
    return nullptr; // FIX: no cycle exists — fast reached the end, must return explicitly
}
```
**Archetypes:** Linked List Cycle I & II, Middle of Linked List, Reorder List.

---

### 8. In-Place Linked List Pointer Reversal & K-Group
**When to use:** Reverse Linked List, Reverse Nodes in k-Group, Palindrome Linked List.
**Core invariant:** Store `nextTemp = curr->next` before redirecting `curr->next = prev`.
```cpp
ListNode* reverseList(ListNode* head) {
    ListNode *prev = nullptr, *curr = head;
    while (curr) {
        ListNode* nxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nxt;
    }
    return prev;
}
```
**Archetypes:** Reverse Linked List, Reverse Nodes in k-Group, Reorder List.

---

### 9. LRU Cache Invariant (Doubly Linked List + Hash Map)
**When to use:** Design a cache with O(1) get and put with an eviction policy. This is the single most-asked Microsoft DSA/design problem — know it cold.
**Core invariant:** Doubly linked list keeps chronological usage order (MRU at head, LRU at tail); the map stores `key -> Node*` for O(1) access.
```cpp
struct DNode {
    int key, val;
    DNode *prev = nullptr, *next = nullptr;
    DNode(int k, int v) : key(k), val(v) {}
};
// Use dummy head and dummy tail to avoid null checks!
void insertAtHead(DNode* node) {
    node->next = head->next; node->prev = head;
    head->next->prev = node; head->next = node;
}
void removeNode(DNode* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}
```
**Archetypes:** LRU Cache, LFU Cache.

---

### 10. Two Heaps Dynamic Balancing
**When to use:** Find the median of a data stream or sliding stream of numbers.
**Core invariant:** Split the stream into a maxHeap (smaller half) and minHeap (larger half); keep `size(maxHeap) >= size(minHeap)` with a difference of at most 1.
```cpp
priority_queue<int> maxHeap;                            // Left half
priority_queue<int, vector<int>, greater<int>> minHeap; // Right half

void addNum(int num) {
    maxHeap.push(num);
    minHeap.push(maxHeap.top()); maxHeap.pop(); // Balance order
    if (maxHeap.size() < minHeap.size()) {
        maxHeap.push(minHeap.top()); minHeap.pop(); // Balance size
    }
}
double findMedian() {
    return maxHeap.size() > minHeap.size() ? maxHeap.top() : (maxHeap.top() + minHeap.top()) / 2.0;
}
```
**Archetypes:** Find Median from Data Stream.
**Don't confuse with:** Pattern 22 (Heap-Based Top-K) — this pattern balances two heaps to find a *midpoint*; Top-K uses a *single* heap of fixed size K.

---

### 11. Tree Post-Order Contribution Pattern
**When to use:** Maximum path sum, diameter, lowest common ancestor.
**Core invariant:** Compute subtree results bottom-up (`leftGain`, `rightGain`), update the global answer with the turning path (`root->val + leftGain + rightGain`), and return only the single-branch max to the parent.
```cpp
int maxGain(TreeNode* root, int& globalMax) {
    if (!root) return 0;
    int left = max(maxGain(root->left, globalMax), 0);   // Discard negative paths
    int right = max(maxGain(root->right, globalMax), 0);
    globalMax = max(globalMax, root->val + left + right); // Turning path
    return root->val + max(left, right);                  // Single branch to parent
}
```
**Archetypes:** Binary Tree Maximum Path Sum, Diameter of Binary Tree, Lowest Common Ancestor.

---

### 12. BST Range Invariant DFS
**When to use:** Validate BST, recover BST, delete in BST.
**Core invariant:** Every node must satisfy `low < node->val < high`; pass the valid range downward.
```cpp
bool isValidBST(TreeNode* root, long long low = LLONG_MIN, long long high = LLONG_MAX) {
    if (!root) return true;
    if (root->val <= low || root->val >= high) return false;
    return isValidBST(root->left, low, root->val) && isValidBST(root->right, root->val, high);
}
```
**Archetypes:** Validate Binary Search Tree, Kth Smallest Element in BST.

---

### 13. Tree BFS with Level-Size Snapshot
**When to use:** Level order traversal, right side view, zigzag level order, max width.
**Core invariant:** Snapshot `int sz = q.size()` at the start of the loop to process an exact level boundary in one batch.
```cpp
queue<TreeNode*> q;
q.push(root);
while (!q.empty()) {
    int sz = q.size();
    for (int i = 0; i < sz; ++i) {
        TreeNode* node = q.front(); q.pop();
        if (i == sz - 1) /* last node in level, visible from right */;
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}
```
**Archetypes:** Binary Tree Level Order Traversal, Binary Tree Right Side View, Zigzag Traversal.

---

### 14. Tree Preorder Delimited Serialization
**When to use:** Serialize and deserialize a binary tree or N-ary tree.
**Core invariant:** A preorder sequence with explicit `#` null sentinels uniquely reconstructs the tree's topology.
```cpp
void serialize(TreeNode* root, ostringstream& out) {
    if (!root) { out << "# "; return; }
    out << root->val << " ";
    serialize(root->left, out);
    serialize(root->right, out);
}
TreeNode* deserialize(istringstream& in) {
    string val;
    if (!(in >> val) || val == "#") return nullptr;
    TreeNode* root = new TreeNode(stoi(val));
    root->left = deserialize(in);
    root->right = deserialize(in);
    return root;
}
```
**Archetypes:** Serialize and Deserialize Binary Tree, Find Duplicate Subtrees.

---

### 15. Grid In-Place Visited Sinking (Matrix DFS/BFS)
**When to use:** Number of islands, word search, flood fill.
**Core invariant:** Overwrite `grid[r][c] = '0'` (sink the island) during traversal to eliminate O(MN) auxiliary visited space.
```cpp
void dfsSink(vector<vector<char>>& grid, int r, int c) {
    int m = grid.size(), n = grid[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != '1') return;
    grid[r][c] = '0'; // Sink in-place
    dfsSink(grid, r+1, c); dfsSink(grid, r-1, c);
    dfsSink(grid, r, c+1); dfsSink(grid, r, c-1);
}
```
**Archetypes:** Number of Islands, Max Area of Island, Word Search (with unmasking for backtracking).

---

### 16. Multi-Source BFS with Level Steps
**When to use:** Rotting oranges, 01 Matrix, shortest bridge.
**Core invariant:** Push all initial sources (for example, every rotten orange) into the queue at t=0, then expand simultaneously level by level.
```cpp
queue<pair<int, int>> q;
for (int r = 0; r < m; ++r)
    for (int c = 0; c < n; ++c)
        if (grid[r][c] == 2) q.push({r, c}); // Push all sources at t=0

int minutes = 0;
while (!q.empty() && freshCount > 0) {
    int sz = q.size();
    for (int i = 0; i < sz; ++i) {
        auto [r, c] = q.front(); q.pop();
        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == 1) {
                grid[nr][nc] = 2; freshCount--;
                q.push({nr, nc});
            }
        }
    }
    minutes++;
}
```
**Archetypes:** Rotting Oranges, Shortest Bridge, As Far from Land as Possible.

---

### 17. Topological Sort (Kahn's In-Degree BFS)
**When to use:** Course schedule, dependency resolution, cycle detection in directed graphs.
**Core invariant:** Compute in-degrees, push all 0-in-degree nodes to a queue. If total processed nodes < N, a directed cycle exists.
```cpp
vector<int> inDegree(n, 0);
for (auto& e : prerequisites) inDegree[e[0]]++;

queue<int> q;
for (int i = 0; i < n; ++i) if (inDegree[i] == 0) q.push(i);

int processed = 0;
while (!q.empty()) {
    int u = q.front(); q.pop();
    processed++;
    for (int v : adj[u]) {
        if (--inDegree[v] == 0) q.push(v);
    }
}
return processed == n;
```
**Archetypes:** Course Schedule I & II, Alien Dictionary, Sequence Reconstruction.

---

### 18. Monotonic Stack (Next Greater Element & Area Boundaries)
**When to use:** Daily Temperatures, Next Greater Element, Largest Rectangle in Histogram.
**Core invariant:** A decreasing stack finds the next greater element in O(N); an increasing stack finds left and right smaller boundaries for histogram rectangle expansion.
```cpp
stack<int> st; // Monotonic increasing stack of indices
int maxArea = 0;
for (int i = 0; i <= n; ++i) {
    int h = (i == n) ? 0 : heights[i];
    while (!st.empty() && h < heights[st.top()]) {
        int height = heights[st.top()]; st.pop();
        int width = st.empty() ? i : (i - st.top() - 1);
        maxArea = max(maxArea, height * width);
    }
    st.push(i);
}
```
**Archetypes:** Daily Temperatures, Largest Rectangle in Histogram, Trapping Rain Water.
**Don't confuse with:** Pattern 27 (Monotonic Deque) — a stack only ever pops from one end and answers "next greater," while a deque pops from both ends to track a moving window's max.

---

### 19. Interval Sweep & Greedy Sorting
**When to use:** Merge intervals, meeting rooms, non-overlapping intervals, insert interval.
**Core invariant:** Sort intervals by start to merge, or by end to maximize non-overlapping scheduling.
```cpp
sort(intervals.begin(), intervals.end());
vector<vector<int>> merged = {intervals[0]};
for (size_t i = 1; i < intervals.size(); ++i) {
    if (intervals[i][0] <= merged.back()[1]) {
        merged.back()[1] = max(merged.back()[1], intervals[i][1]); // Overlap
    } else {
        merged.push_back(intervals[i]);
    }
}
```
**Archetypes:** Merge Intervals, Non-overlapping Intervals, Insert Interval, Meeting Rooms II.

---

### 20. 1D Dynamic Programming (Knapsack & String Partitioning)
**When to use:** Coin Change, Word Break, Decode Ways, Longest Increasing Subsequence.
**Core invariant:** State `dp[i]` depends strictly on subproblems `dp[i - choice]` or prior valid partitions `dp[j]` where `j < i`.
```cpp
unordered_set<string> dict(wordDict.begin(), wordDict.end());
vector<bool> dp(n + 1, false);
dp[0] = true;
for (int i = 1; i <= n; ++i) {
    for (int j = 0; j < i; ++j) {
        if (dp[j] && dict.count(s.substr(j, i - j))) {
            dp[i] = true;
            break;
        }
    }
}
```
**Archetypes:** Coin Change, Word Break, Decode Ways, Longest Increasing Subsequence (Patience Sort, O(N log N)).

---

### 21. Union-Find (Disjoint Set Union with Path Compression)
**When to use:** Number of connected components, redundant connection, accounts merge, checking if a graph stays acyclic as edges are added one at a time.
**Core invariant:** Two elements are in the same group iff they share the same root after path compression; every successful `union` reduces the component count by exactly 1.
```cpp
struct DSU {
    vector<int> parent, rank_;
    int components;
    DSU(int n) : parent(n), rank_(n, 0), components(n) { iota(parent.begin(), parent.end(), 0); }
    int find(int i) { return parent[i] == i ? i : (parent[i] = find(parent[i])); }
    bool unite(int u, int v) {
        int ru = find(u), rv = find(v);
        if (ru == rv) return false;                     // Already connected -> would form a cycle
        if (rank_[ru] < rank_[rv]) swap(ru, rv);          // Union by rank keeps trees shallow
        parent[rv] = ru;
        if (rank_[ru] == rank_[rv]) rank_[ru]++;
        components--;
        return true;
    }
};
```
**Archetypes:** Number of Provinces, Redundant Connection, Accounts Merge, Number of Connected Components in an Undirected Graph.
**Say out loud:** "Since I only care about grouping, not path length, Union-Find gives me near-O(1) merge and lookup instead of a full graph traversal for every query."

---

### 22. Heap-Based Top-K Elements
**When to use:** Kth largest element in a stream/array, top K frequent elements, K closest points to origin.
**Core invariant:** Maintain a heap of size exactly K; for "K largest," use a **min-heap** so the smallest of the K survivors sits on top and gets evicted first when a bigger element arrives.
```cpp
priority_queue<int, vector<int>, greater<int>> minHeap; // Holds the K largest seen so far
for (int x : nums) {
    minHeap.push(x);
    if (minHeap.size() > k) minHeap.pop();               // Evict the current smallest of the K
}
int kthLargest = minHeap.top();
```
**Archetypes:** Kth Largest Element in an Array/Stream, Top K Frequent Elements (heap of `{freq, val}` pairs), K Closest Points to Origin.
**Trap:** It's counterintuitive — finding the K *largest* uses a **min**-heap (so the smallest of the survivors is the one you can cheaply evict), not a max-heap.
**Alternative:** Quickselect (Hoare partition) solves the single-answer "Kth largest" case in average O(N) instead of O(N log K), but is harder to get exactly right under pressure — default to the heap unless explicitly asked for O(N).

---

### 23. Backtracking (Choose / Explore / Un-choose)
**When to use:** Generate all subsets, permutations, or combinations; word search with path reconstruction; N-Queens/Sudoku-style constraint placement.
**Core invariant:** At every branch, mutate shared state, recurse into the next decision, then **undo the exact mutation** before returning — the call stack itself tracks the partial solution so no extra "path so far" copies are needed.
```cpp
void backtrack(vector<int>& nums, int start, vector<int>& curr, vector<vector<int>>& res) {
    res.push_back(curr);                                  // Record every partial state (for subsets)
    for (int i = start; i < (int)nums.size(); ++i) {
        curr.push_back(nums[i]);                            // Choose
        backtrack(nums, i + 1, curr, res);                  // Explore
        curr.pop_back();                                    // Un-choose
    }
}
```
**Archetypes:** Subsets, Permutations, Combination Sum, Word Search, N-Queens, Palindrome Partitioning.
**Trap:** Forgetting the "un-choose" step (`pop_back`, resetting a visited flag) silently corrupts every subsequent branch — it won't crash, it'll just give the wrong answer.
**Say out loud:** "I'll explore this as a decision tree — at each node I choose an option, recurse, then backtrack by undoing that exact choice before trying the next sibling."

---

### 24. 2D Grid Dynamic Programming (Two-String Alignment)
**When to use:** Edit distance, longest common subsequence, distinct subsequences — anywhere you're comparing prefixes of two sequences against each other.
**Core invariant:** `dp[i][j]` represents the answer using the first `i` characters of string A and the first `j` characters of string B; transitions compare `A[i-1]` vs `B[j-1]` and pull from `dp[i-1][j]`, `dp[i][j-1]`, or `dp[i-1][j-1]`.
```cpp
// Longest Common Subsequence
vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
        if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
        else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
    }
}
return dp[m][n];
```
**Archetypes:** Longest Common Subsequence, Edit Distance, Distinct Subsequences, Interleaving String.
**Trap:** Off-by-one is brutal here — row/column 0 represents the *empty prefix*, so the actual characters are always `a[i-1]` / `b[j-1]`, never `a[i]` / `b[j]`.

---

### 25. Trie (Prefix Tree)
**When to use:** Prefix search/autocomplete, word search II (multiple words on a grid), longest word built from other words.
**Core invariant:** Each node branches per character (commonly a 26-element array for lowercase input); a boolean `isEnd` flag marks where a complete word terminates, distinguishing a full word from a mere prefix.
```cpp
struct TrieNode {
    TrieNode* children[26] = {nullptr};
    bool isEnd = false;
};
void insert(TrieNode* root, const string& word) {
    TrieNode* node = root;
    for (char c : word) {
        int i = c - 'a';
        if (!node->children[i]) node->children[i] = new TrieNode();
        node = node->children[i];
    }
    node->isEnd = true;
}
```
**Archetypes:** Implement Trie, Word Search II, Design Add and Search Words Data Structure, Longest Word in Dictionary.
**Say out loud:** "Since many queries share common prefixes, a trie lets me reuse that shared work instead of re-scanning the whole word list per query."

---

### 26. Dijkstra's Algorithm (Weighted Shortest Path)
**When to use:** Shortest path with non-negative weighted edges — plain BFS only works when every edge costs the same.
**Core invariant:** Greedily pop the currently-closest unvisited node from a min-heap and relax its neighbors; a popped entry whose recorded distance is worse than the best known distance is stale and safely skipped (lazy deletion, avoids a separate "visited" set).
```cpp
vector<long long> dijkstra(int n, int src, vector<vector<pair<int,int>>>& adj) {
    vector<long long> dist(n, LLONG_MAX);
    dist[src] = 0;
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;                        // Stale entry, a better path was already found
        for (auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```
**Archetypes:** Network Delay Time, Cheapest Flights Within K Stops (with modification), Path With Minimum Effort.
**Trap:** Dijkstra breaks down with negative edge weights — that needs Bellman-Ford instead, which is rarely required in an OA setting but worth naming if asked.

---

### 27. Monotonic Deque (Sliding Window Maximum)
**When to use:** Maximum (or minimum) in every window of size K as it slides across an array.
**Core invariant:** The deque holds indices in strictly decreasing order of value; the front is always the current window's max, and any index that falls outside the window on the left is evicted from the front first.
```cpp
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq; // Indices, strictly decreasing values
    vector<int> res;
    for (int i = 0; i < (int)nums.size(); ++i) {
        while (!dq.empty() && dq.front() <= i - k) dq.pop_front();     // Evict out-of-window
        while (!dq.empty() && nums[dq.back()] <= nums[i]) dq.pop_back(); // Maintain decreasing order
        dq.push_back(i);
        if (i >= k - 1) res.push_back(nums[dq.front()]);
    }
    return res;
}
```
**Archetypes:** Sliding Window Maximum, Shortest Subarray with Sum at Least K (with prefix sums), Constrained Subsequence Sum.

---

## Edge Case Checklist (Mention Before You Say "I'm Done")

1. **Empty or null input** — `head == nullptr`, `root == nullptr`, `s.empty()`, `nums.empty()`.
2. **Single element** — array with one item, single-node tree, single character.
3. **No solution case** — return `-1`, `""`, or `{}` when nothing valid exists.
4. **All duplicates** — identical elements (for example `[2, 2, 2, 2]` in 3Sum or Dutch National Flag).
5. **Negative numbers** — breaks monotonicity in subarray sums or product-based DP.
6. **Integer overflow** — always compute mid as `left + (right - left) / 2`, never `(left + right) / 2`.
7. **Tree skewness** — a linked-list-shaped tree causing O(N) recursion depth.
8. **Disconnected graphs** — multiple components or isolated 0-degree vertices.
9. **Even vs. odd dimensions** — middle-element handling in linked lists and palindromes.
10. **String delimiters** — leading/trailing spaces and repeated consecutive delimiters in parsers.
11. **K larger than the input size** — top-K, sliding-window-K, and k-group reversal problems all need a guard for `k >= n`.
12. **Self-loops and duplicate edges** — Union-Find and graph traversal problems where an edge connects a node to itself or repeats an existing edge.

---

## 45-Second Recall List

Say these out loud tomorrow morning without looking. If any one is shaky, that's your last five minutes of review.

1. Negative numbers in a subarray-sum problem → prefix sum + hash map, not sliding window.
2. Repeats in a substring → last-seen index jump, not a shrinking while-loop.
3. Shorter side is the bottleneck → converging two pointers.
4. One half of a rotated array is always sorted → binary search still applies.
5. A monotonic true/false condition over a range → binary search the answer, not the array.
6. Fast pointer at 2x speed meeting slow → cycle detected; no meeting means return null explicitly.
7. LRU → doubly linked list + hash map, dummy head/tail, no null checks.
8. Median of a stream → two heaps, balance size within 1.
9. Turning path in a tree → post-order gains, discard negative branches.
10. BST validity → range shrinks as you go down, not just parent comparison.
11. Level-based tree problems → snapshot queue size before the inner loop.
12. Grid flood-fill → sink in place instead of a separate visited set.
13. Multiple simultaneous starting points → push them all before BFS begins.
14. Course/dependency ordering → Kahn's in-degree BFS; unprocessed nodes mean a cycle.
15. Next greater / histogram area → monotonic stack.
16. Scheduling intervals → sort by end; merging intervals → sort by start.
17. DP state depends only on strictly smaller subproblems — define `dp[i]` before coding, out loud.
18. Grouping/connectivity without needing path details → Union-Find, not a full traversal.
19. "Kth largest" → min-heap of size K, not a max-heap (this trips people up under pressure).
20. Generate all X → backtracking; every "choose" needs a matching "un-choose" before the next sibling call.
21. Comparing two strings/sequences against each other → 2D DP table, indices are prefix lengths (offset by one from the characters).
22. Shared prefixes across many words → trie, not repeated string scanning.
23. Shortest path with weighted edges → Dijkstra's with a min-heap and lazy-skip stale pops.
24. Max/min of every window as it slides → monotonic deque, front is the current answer.