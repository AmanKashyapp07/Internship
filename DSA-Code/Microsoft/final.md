# The Microsoft Interview 1-Day Revision Guide
### Top 20 Core DSA Patterns to Recognize Cold

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

---

## The 20 Patterns in Detail

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
ListNode *slow = head, *fast = head;
while (fast && fast->next) {
    slow = slow->next; fast = fast->next->next;
    if (slow == fast) {
        slow = head;
        while (slow != fast) { slow = slow->next; fast = fast->next; }
        return slow; // Cycle start
    }
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

---

## 45-Second Recall List

Say these out loud tomorrow morning without looking. If any one is shaky, that's your last five minutes of review.

1. Negative numbers in a subarray-sum problem → prefix sum + hash map, not sliding window.
2. Repeats in a substring → last-seen index jump, not a shrinking while-loop.
3. Shorter side is the bottleneck → converging two pointers.
4. One half of a rotated array is always sorted → binary search still applies.
5. A monotonic true/false condition over a range → binary search the answer, not the array.
6. Fast pointer at 2x speed meeting slow → cycle detected.
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