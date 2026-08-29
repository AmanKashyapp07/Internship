# 🚀 The Ultimate Microsoft Interview 1-Day Revision Guide
### Top 20 Core DSA Patterns to Master & Memorize Cold

This guide contains the **Top 20 most critical algorithmic patterns** asked in Microsoft SWE Internship rounds. Review this document the night before or morning of your interview.

---

## ⚡ Quick 5-Minute Interview Routine

1. **Clarify (1-2 min)**: Ask about constraints, empty input `""`/`[]`, single element, negative numbers, duplicates, integer overflow ($> 2 \cdot 10^9$).
2. **State Intuition (1-2 min)**: Start with the brute-force baseline ($O(N^2)$), state why it's suboptimal, and name the target pattern.
3. **Verbalize Invariant**: Explain the core truth *before* writing a single line of code.
4. **Code Cleanly (15 min)**: Use modern C++ standard library features (`std::vector`, `std::unordered_map`, `std::queue`, `std::pair`).
5. **Dry Run with Edge Case (2 min)**: Trace a 3-element test case line-by-line before saying "I am finished".
6. **State Complexity (30 sec)**: Explicitly state Time ($O(\dots)$) and Auxiliary Space ($O(\dots)$).

---

# 🧠 Top 20 Core Patterns

---

### 1. Prefix Sum + Hash Map Frequency
* **When to Use**: "Count/find subarrays with sum $= K$ or sum divisible by $K$ (especially with **negative numbers**)".
* **Core Invariant**: $\text{sum}(i..j) = \text{prefix}[j] - \text{prefix}[i-1] = K \implies \text{prefix}[i-1] = \text{prefix}[j] - K$.
* **C++ Template**:
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
* **Microsoft Archetypes**: `Subarray Sum Equals K`, `Subarray Sums Divisible by K`, `Path Sum III`.
* **Trap**: Never use a 2-pointer sliding window when negative numbers are present—monotonicity is broken!

---

### 2. Dynamic Sliding Window (Last-Seen Index)
* **When to Use**: "Longest substring without repeating characters / with at most $K$ distinct characters".
* **Core Invariant**: When a duplicate of character `c` is seen at `lastIdx[c] >= left`, immediately jump `left = lastIdx[c] + 1`.
* **C++ Template**:
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
* **Microsoft Archetypes**: `Longest Substring Without Repeating Characters`.
* **Say Out Loud**: *"Instead of shrinking the window step-by-step in an inner while-loop, we record the last-seen index of each ASCII character to jump the left pointer in $O(1)$."*

---

### 3. Sliding Window with Match Counter (`have == need`)
* **When to Use**: "Minimum window substring / Permutation in string / Anagram matching".
* **Core Invariant**: Track unique matched characters with integer `have` against `need = targetFreq.size()` to avoid $O(26)$ comparisons per step.
* **C++ Template**:
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
* **Microsoft Archetypes**: `Minimum Window Substring`, `Find All Anagrams in a String`.

---

### 4. Converging Two Pointers (Bottleneck Invariant)
* **When to Use**: `3Sum`, `Container With Most Water`, `Trapping Rain Water`.
* **Core Invariant**: Area / trapped water is bound by the **shorter** boundary. Moving the taller boundary can never improve the result; you must move the shorter pointer inward.
* **C++ Template (Trapping Rain Water)**:
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
* **Microsoft Archetypes**: `Trapping Rain Water`, `Container With Most Water`, `3Sum` (with sorting & deduplication).

---

### 5. Rotated Sorted Binary Search (Sorted-Half Invariant)
* **When to Use**: "Search or find minimum in rotated sorted array".
* **Core Invariant**: At any midpoint `mid`, **at least one half `[left..mid]` or `[mid..right]` is strictly sorted**.
* **C++ Template**:
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
* **Microsoft Archetypes**: `Search in Rotated Sorted Array`, `Find Minimum in Rotated Sorted Array`.

---

### 6. Binary Search on Monotonic Answer Range
* **When to Use**: "Find minimum speed / capacity / threshold such that condition $F(X)$ is satisfied".
* **Core Invariant**: Predicate $F(X)$ is monotonic: `[false, false, ..., true, true]`. Search range is `[low, high]`.
* **C++ Template**:
  ```cpp
  int low = 1, high = max_element(piles.begin(), piles.end());
  int ans = high;
  while (low <= high) {
      int mid = low + (high - low) / 2;
      if (canFinish(mid, h)) {
          ans = mid;
          high = mid - 1; // Try smaller speed
      } else {
          low = mid + 1;
      }
  }
  ```
* **Microsoft Archetypes**: `Koko Eating Bananas`, `Capacity To Ship Packages Within D Days`, `Split Array Largest Sum`.

---

### 7. Floyd's Fast & Slow Pointers (Tortoise & Hare)
* **When to Use**: "Detect cycle, find cycle start, find linked list middle".
* **Core Invariant**: Fast moves $2\times$ faster than slow. If a cycle of length $C$ exists, relative distance shrinks by 1 each step.
* **C++ Template (Find Cycle Start)**:
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
* **Microsoft Archetypes**: `Linked List Cycle I & II`, `Middle of Linked List`, `Reorder List`.

---

### 8. In-Place Linked List Pointer Reversal & K-Group
* **When to Use**: `Reverse Linked List`, `Reverse Nodes in k-Group`, `Palindrome Linked List`.
* **Core Invariant**: Store `nextTemp = curr->next` before redirecting `curr->next = prev`.
* **C++ Template**:
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
* **Microsoft Archetypes**: `Reverse Linked List`, `Reverse Nodes in k-Group`, `Reorder List`.

---

### 9. LRU Cache Invariant (Doubly Linked List + Hash Map)
* **When to Use**: "Design cache with $O(1)$ `get` and `put` with eviction policy".
* **Core Invariant**: Doubly linked list maintains chronological usage (MRU at head, LRU at tail). Map stores `key -> Node*`.
* **C++ Template**:
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
* **Microsoft Archetypes**: `LRU Cache` (Top #1 most asked Microsoft system/DSA problem), `LFU Cache`.

---

### 10. Two Heaps Dynamic Balancing
* **When to Use**: "Find median of a data stream / sliding stream numbers".
* **Core Invariant**: Split stream into `maxHeap` (left smaller half) and `minHeap` (right larger half). Ensure $\text{size}(\text{maxHeap}) \ge \text{size}(\text{minHeap})$ with diff $\le 1$.
* **C++ Template**:
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
* **Microsoft Archetypes**: `Find Median from Data Stream`.

---

### 11. Tree Post-Order Contribution Pattern
* **When to Use**: "Maximum path sum, diameter, lowest common ancestor".
* **Core Invariant**: Compute subtree results bottom-up (`leftGain`, `rightGain`), update global answer with the turning path (`root->val + leftGain + rightGain`), and return single branch max to parent.
* **C++ Template**:
  ```cpp
  int maxGain(TreeNode* root, int& globalMax) {
      if (!root) return 0;
      int left = max(maxGain(root->left, globalMax), 0);   // Discard negative paths
      int right = max(maxGain(root->right, globalMax), 0);
      globalMax = max(globalMax, root->val + left + right); // Turning path
      return root->val + max(left, right);                  // Single branch to parent
  }
  ```
* **Microsoft Archetypes**: `Binary Tree Maximum Path Sum`, `Diameter of Binary Tree`, `Lowest Common Ancestor`.

---

### 12. BST Range Invariant DFS
* **When to Use**: "Validate BST, recover BST, delete in BST".
* **Core Invariant**: Every node in BST must satisfy $\text{low} < \text{node}->\text{val} < \text{high}$. Pass valid range downwards.
* **C++ Template**:
  ```cpp
  bool isValidBST(TreeNode* root, long long low = LLONG_MIN, long long high = LLONG_MAX) {
      if (!root) return true;
      if (root->val <= low || root->val >= high) return false;
      return isValidBST(root->left, low, root->val) && isValidBST(root->right, root->val, high);
  }
  ```
* **Microsoft Archetypes**: `Validate Binary Search Tree`, `Kth Smallest Element in BST`.

---

### 13. Tree BFS with Level-Size Snapshot
* **When to Use**: "Level order traversal, right side view, zigzag level order, max width".
* **Core Invariant**: Snapshot `int sz = q.size()` at the start of loop to process exact level boundaries in one batch.
* **C++ Template**:
  ```cpp
  queue<TreeNode*> q;
  q.push(root);
  while (!q.empty()) {
      int sz = q.size();
      for (int i = 0; i < sz; ++i) {
          TreeNode* node = q.front(); q.pop();
          if (i == sz - 1) /* Last node in level visible from right */;
          if (node->left) q.push(node->left);
          if (node->right) q.push(node->right);
      }
  }
  ```
* **Microsoft Archetypes**: `Binary Tree Level Order Traversal`, `Binary Tree Right Side View`, `Zigzag Traversal`.

---

### 14. Tree Preorder Delimited Serialization
* **When to Use**: "Serialize and deserialize binary tree / N-ary tree".
* **Core Invariant**: Preorder sequence with explicit `#` null sentinels uniquely reconstructs tree topology.
* **C++ Template**:
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
* **Microsoft Archetypes**: `Serialize and Deserialize Binary Tree`, `Find Duplicate Subtrees`.

---

### 15. Grid In-Place Visited Sinking (Matrix DFS/BFS)
* **When to Use**: "Number of islands, word search, flood fill".
* **Core Invariant**: Overwrite `grid[r][c] = '0'` (sink island) during traversal to eliminate $O(MN)$ auxiliary visited space.
* **C++ Template**:
  ```cpp
  void dfsSink(vector<vector<char>>& grid, int r, int c) {
      int m = grid.size(), n = grid[0].size();
      if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != '1') return;
      grid[r][c] = '0'; // Sink in-place
      dfsSink(grid, r+1, c); dfsSink(grid, r-1, c);
      dfsSink(grid, r, c+1); dfsSink(grid, r, c-1);
  }
  ```
* **Microsoft Archetypes**: `Number of Islands`, `Max Area of Island`, `Word Search` (with unmasking for backtracking).

---

### 16. Multi-Source BFS with Level Steps
* **When to Use**: "Rotting oranges, 01 Matrix, shortest bridge".
* **Core Invariant**: Push ALL initial sources (e.g. all rotten oranges) into the queue at $t=0$, then expand simultaneously level-by-level.
* **C++ Template**:
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
* **Microsoft Archetypes**: `Rotting Oranges`, `Shortest Bridge`, `As Far from Land as Possible`.

---

### 17. Topological Sort (Kahn's In-Degree BFS)
* **When to Use**: "Course schedule, dependency resolution, cycle detection in directed graphs".
* **Core Invariant**: Compute in-degrees; push 0-in-degree nodes to queue. If total processed nodes $< N$, a directed cycle exists.
* **C++ Template**:
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
* **Microsoft Archetypes**: `Course Schedule I & II`, `Alien Dictionary`, `Sequence Reconstruction`.

---

### 18. Monotonic Stack (Next Greater Element & Area Boundaries)
* **When to Use**: `Daily Temperatures`, `Next Greater Element`, `Largest Rectangle in Histogram`.
* **Core Invariant**:
  - **Decreasing Stack**: Finds next greater element ($O(N)$).
  - **Increasing Stack**: Finds left & right smaller boundaries for histogram rectangle expansion.
* **C++ Template (Largest Rectangle in Histogram)**:
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
* **Microsoft Archetypes**: `Daily Temperatures`, `Largest Rectangle in Histogram`, `Trapping Rain Water`.

---

### 19. Interval Sweep & Greedy Sorting
* **When to Use**: "Merge intervals, meeting rooms, non-overlapping intervals, insert interval".
* **Core Invariant**: Sort intervals by `start` (for merging) or by `end` (for maximum non-overlapping scheduling).
* **C++ Template (Merge Intervals)**:
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
* **Microsoft Archetypes**: `Merge Intervals`, `Non-overlapping Intervals`, `Insert Interval`, `Meeting Rooms II`.

---

### 20. 1D Dynamic Programming (Knapsack & String Partitioning)
* **When to Use**: `Coin Change`, `Word Break`, `Decode Ways`, `Longest Increasing Subsequence`.
* **Core Invariant**: Transition state `dp[i]` depends strictly on subproblems `dp[i - choice]` or prior valid partitions `dp[j]` where $j < i$.
* **C++ Template (Word Break)**:
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
* **Microsoft Archetypes**: `Coin Change`, `Word Break`, `Decode Ways`, `Longest Increasing Subsequence` (Patience Sort $O(N \log N)$).

---

## 🎯 10 Must-Mention Edge Cases Checklist
1. **Empty / Null Input**: `head == nullptr`, `root == nullptr`, `s.empty()`, `nums.empty()`.
2. **Single Element**: Array with 1 element, single-node tree, single character.
3. **No Solution Case**: Return `-1`, `""`, or `{}` when no valid match exists.
4. **All Duplicates**: Array with identical elements (e.g. `[2, 2, 2, 2]` in 3Sum or Dutch National Flag).
5. **Negative Numbers**: Breaking monotonicity in subarray sums or product swapping in DP.
6. **Integer Overflow**: Always calculate mid as `left + (right - left) / 2` instead of `(left + right) / 2`.
7. **Tree Skewness**: Linked-list-like degenerated tree causing $O(N)$ recursion depth.
8. **Disconnected Graphs**: Graph with multiple disconnected components or isolated 0-degree vertices.
9. **Even vs Odd Dimensions**: Middle element handling in linked lists and palindromes.
10. **String Delimiters**: Trailing/leading spaces and multiple consecutive space delimiters in parsers.
