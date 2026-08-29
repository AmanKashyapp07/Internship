#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

const ll MOD = 1e9 + 7;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// ============================================================================
// STRIVER 79 & BLIND 75 - BATCH 4 (Problems 01 to 17)
// ============================================================================

int largestRectangleArea(vi& heights) {
    int n = heights.size(), max_area = 0;
    stack<int> st; // monotonic increasing stack of bar indices

    for (int i = 0; i <= n; i++) {
        int h = (i == n) ? 0 : heights[i];
        while (!st.empty() && heights[st.top()] >= h) {
            int height = heights[st.top()];
            st.pop();
            int width = st.empty() ? i : (i - st.top() - 1);
            max_area = max(max_area, height * width);
        }
        st.push(i);
    }

    return max_area;
}
// Interview Explanation:
// - Problem Statement: Find the area of the largest rectangle that can be formed within a histogram.
// - Approach: Monotonic Increasing Stack in a single pass with virtual zero sentinel.
// - Intuition: When a bar `heights[i]` is smaller than the top of the stack, the rectangle bounded by `heights[st.top()]` can no longer extend rightward. Pop it; its width spans from the previous smaller element (`st.top()`) to the current element `i` (`width = i - st.top() - 1`).
// - Complexity: Time: O(N) single pass, Space: O(N) stack space.

vi asteroidCollision(vi& asteroids) {
    vi st; // vector used as stack for direct return
    for (int ast : asteroids) {
        bool destroyed = false;
        while (!st.empty() && st.back() > 0 && ast < 0) {
            if (st.back() < -ast) {
                st.pop_back(); // top asteroid explodes, continue collision check
            } else if (st.back() == -ast) {
                st.pop_back(); // both explode
                destroyed = true;
                break;
            } else {
                destroyed = true; // current asteroid explodes
                break;
            }
        }
        if (!destroyed) st.push_back(ast);
    }
    return st;
}
// Interview Explanation:
// - Problem Statement: Simulate asteroid collisions where positive asteroids move right and negative move left, destroying the smaller one or both if equal.
// - Approach: Stack simulation (using vector as stack for direct return).
// - Intuition: Collision occurs only when top of stack is moving right (`> 0`) and current asteroid is moving left (`< 0`). Compare absolute values: pop stack if current is larger, destroy both if equal, or destroy current if smaller.
// - Complexity: Time: O(N) each asteroid is pushed and popped at most once, Space: O(N) for output stack.

vi maxSlidingWindow(vi& nums, int k) {
    deque<int> dq; // monotonic decreasing deque storing indices
    vi ans;
    for (int i = 0; i < (int)nums.size(); i++) {
        if (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front(); // remove elements out of current window
        }
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();  // discard elements dominated by nums[i]
        }
        dq.push_back(i);
        if (i >= k - 1) {
            ans.push_back(nums[dq.front()]);
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Return the maximum value in each sliding window of size k as it slides across the array.
// - Approach: Monotonic Decreasing Deque storing indices.
// - Intuition: Elements smaller than `nums[i]` that appeared earlier in the window are useless because `nums[i]` is larger and will outlive them; pop them from the back. The front of the deque always holds the max element for the current window.
// - Complexity: Time: O(N) amortized linear time, Space: O(K) deque space.

class LRUCache {
private:
    struct Node {
        int key, val;
        Node *prev, *next;
        Node(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };

    int cap;
    unordered_map<int, Node*> cache;
    Node *head, *tail;

    void addNode(Node *node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    void removeNode(Node *node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void moveToHead(Node *node) {
        removeNode(node);
        addNode(node);
    }

public:
    LRUCache(int capacity) : cap(capacity) {
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (!cache.count(key)) return -1;
        Node *node = cache[key];
        moveToHead(node);
        return node->val;
    }

    void put(int key, int value) {
        if (cache.count(key)) {
            Node *node = cache[key];
            node->val = value;
            moveToHead(node);
        } else {
            if ((int)cache.size() == cap) {
                Node *lru = tail->prev;
                cache.erase(lru->key);
                removeNode(lru);
                delete lru;
            }
            Node *node = new Node(key, value);
            cache[key] = node;
            addNode(node);
        }
    }
};
// Interview Explanation:
// - Problem Statement: Design a data structure that follows the constraints of a Least Recently Used (LRU) cache with O(1) get and put operations.
// - Approach: Hash Map + Doubly Linked List with dummy head and tail sentinel nodes.
// - Intuition: Doubly Linked List enables O(1) node removal and insertion at head (most recently used). Hash Map provides O(1) key-to-node lookup. On capacity overflow, evict `tail->prev` (least recently used).
// - Complexity: Time: O(1) for both get and put, Space: O(capacity) for hash map and DLL nodes.

int findKthLargest(vi& nums, int k) {
    priority_queue<int, vi, greater<int>> min_heap;
    for (int x : nums) {
        min_heap.push(x);
        if ((int)min_heap.size() > k) {
            min_heap.pop();
        }
    }
    return min_heap.top();
}
// Interview Explanation:
// - Problem Statement: Find the k-th largest element in an unsorted array without full sorting.
// - Approach: Min-Heap of size k (maintaining the k largest elements).
// - Intuition: By maintaining a min-heap of size k, the root always holds the minimum among the k largest elements seen so far (which is the k-th largest overall).
// - Complexity: Time: O(N log K) with min-heap, Space: O(K) heap space.

int leastInterval(vector<char>& tasks, int n) {
    vi freq(26, 0);
    for (char c : tasks) freq[c - 'A']++;
    int max_f = *max_element(freq.begin(), freq.end());
    int max_count = count(freq.begin(), freq.end(), max_f);

    int ans = (max_f - 1) * (n + 1) + max_count;
    return max((int)tasks.size(), ans);
}
// Interview Explanation:
// - Problem Statement: Find the minimum CPU intervals to execute all tasks with cooling period n between identical tasks.
// - Approach: Greedy frequency math based on the most frequent task.
// - Intuition: The most frequent task with frequency `max_f` creates `max_f - 1` chunks of size `n + 1`, plus `max_count` tasks in the final chunk. If idle slots are insufficient or tasks overflow, total time is simply `tasks.size()`.
// - Complexity: Time: O(N) single pass to count frequencies, Space: O(1) 26-element array.

class MinHeap {
private:
    vi heap;

    void heapifyUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (heap[i] < heap[parent]) {
                swap(heap[i], heap[parent]);
                i = parent;
            } else break;
        }
    }

    void heapifyDown(int i) {
        int n = heap.size(), smallest = i;
        int left = 2 * i + 1, right = 2 * i + 2;

        if (left < n && heap[left] < heap[smallest]) smallest = left;
        if (right < n && heap[right] < heap[smallest]) smallest = right;

        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    void push(int val) {
        heap.push_back(val);
        heapifyUp(heap.size() - 1);
    }

    int pop() {
        if (heap.empty()) return -1;
        int root = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
        return root;
    }

    int top() {
        return heap.empty() ? -1 : heap[0];
    }

    int size() {
        return heap.size();
    }
};
// Interview Explanation:
// - Problem Statement: Implement a Min Heap supporting push, pop (extract minimum), and top operations.
// - Approach: Array-based Complete Binary Tree with sift-up and sift-down rebalancing.
// - Intuition: For node at index `i`, parent is `(i - 1) / 2` and children are `2*i + 1` and `2*i + 2`. Push appends to end and bubbles up; Pop replaces root with the last element and bubbles down.
// - Complexity: Time: O(log N) for push/pop, O(1) for top, Space: O(N) array storage.

int treeHeightAndDiameter(TreeNode* root, int& diameter) {
    if (!root) return 0;
    int lh = treeHeightAndDiameter(root->left, diameter);
    int rh = treeHeightAndDiameter(root->right, diameter);
    diameter = max(diameter, lh + rh);
    return 1 + max(lh, rh);
}

int diameterOfBinaryTree(TreeNode* root) {
    int diameter = 0;
    treeHeightAndDiameter(root, diameter);
    return diameter;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest path between any two nodes in a binary tree.
// - Approach: Bottom-Up DFS computing subtree heights while updating global diameter.
// - Intuition: The longest path passing through any node `u` is `left_height + right_height`. Recursively compute subtree heights in post-order and maintain the maximum path sum observed across all nodes.
// - Complexity: Time: O(N) visiting each node once, Space: O(H) recursion stack where H is tree height.

int maxGain(TreeNode* root, int& max_sum) {
    if (!root) return 0;
    int left_gain = max(0, maxGain(root->left, max_sum));
    int right_gain = max(0, maxGain(root->right, max_sum));

    int current_path_sum = root->val + left_gain + right_gain;
    max_sum = max(max_sum, current_path_sum);

    return root->val + max(left_gain, right_gain);
}

int maxPathSum(TreeNode* root) {
    int max_sum = INT_MIN;
    maxGain(root, max_sum);
    return max_sum;
}
// Interview Explanation:
// - Problem Statement: Find the maximum sequence path sum of node values in a binary tree (path can start and end at any node).
// - Approach: Post-order DFS calculating maximum positive branch gains.
// - Intuition: At each node, compute the max path sum turning at this node (`root->val + left_gain + right_gain`), ignoring negative branch gains (`max(0, gain)`). Return `root->val + max(left_gain, right_gain)` to the parent caller.
// - Complexity: Time: O(N), Space: O(H) recursion stack.

vi bottomView(TreeNode *root) {
    if (!root) return {};
    map<int, int> bottom_nodes;
    queue<pair<TreeNode*, int>> q;
    q.push({root, 0});

    while (!q.empty()) {
        auto [node, hd] = q.front();
        q.pop();

        bottom_nodes[hd] = node->val; // overwrite with deepest node at horizontal distance hd

        if (node->left) q.push({node->left, hd - 1});
        if (node->right) q.push({node->right, hd + 1});
    }

    vi ans;
    for (auto& [hd, val] : bottom_nodes) {
        ans.push_back(val);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Return the bottom view of a binary tree (the lowest node visible along each vertical column line from left to right).
// - Approach: BFS Level Order Traversal with Horizontal Distance (HD) coordinate mapping.
// - Intuition: Start root at HD 0; left child is HD - 1, right is HD + 1. Using BFS guarantees that subsequent levels overwrite earlier ones in `bottom_nodes[hd]`, naturally keeping the bottom-most node at each HD coordinate.
// - Complexity: Time: O(N log K) where K is number of unique vertical lines, Space: O(N) queue and map.

// ----------------------------------------------------------------------------
// BLIND 75 INTEGRATED PROBLEMS (11 to 17)
// ----------------------------------------------------------------------------

vvi insertInterval(vvi& intervals, vi& newInterval) {
    vvi ans;
    int i = 0, n = intervals.size();

    // 1. Add all intervals before newInterval starts
    while (i < n && intervals[i][1] < newInterval[0]) {
        ans.push_back(intervals[i++]);
    }

    // 2. Merge all overlapping intervals with newInterval
    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = min(newInterval[0], intervals[i][0]);
        newInterval[1] = max(newInterval[1], intervals[i][1]);
        i++;
    }
    ans.push_back(newInterval);

    // 3. Add all remaining intervals
    while (i < n) {
        ans.push_back(intervals[i++]);
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Insert a newInterval into a sorted non-overlapping interval list and merge if necessary.
// - Approach: Three-phase linear sweep (before overlap, merge overlap, after overlap).
// - Intuition: Intervals before `newInterval` (`interval[1] < newInterval[0]`) are disjoint and added directly. Overlapping intervals (`interval[0] <= newInterval[1]`) are merged by expanding `newInterval` boundaries (`min(start), max(end)`). Remaining intervals are appended.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space (excluding result).

vvi mergeIntervals(vvi& intervals) {
    if (intervals.empty()) return {};
    sort(intervals.begin(), intervals.end());

    vvi merged;
    merged.push_back(intervals[0]);

    for (int i = 1; i < (int)intervals.size(); i++) {
        if (intervals[i][0] <= merged.back()[1]) {
            merged.back()[1] = max(merged.back()[1], intervals[i][1]);
        } else {
            merged.push_back(intervals[i]);
        }
    }

    return merged;
}
// Interview Explanation:
// - Problem Statement: Given an array of intervals, merge all overlapping intervals and return an array of the non-overlapping intervals.
// - Approach: Sorting by start time + Single-pass linear merge.
// - Intuition: Sort intervals by start time. If the current interval starts before or at the end of the previous merged interval (`interval[0] <= merged.back()[1]`), they overlap; extend the end time to `max(end1, end2)`. Otherwise, push as a new non-overlapping interval.
// - Complexity: Time: O(N log N) sorting, Space: O(1) auxiliary space (excluding returned vector).

int eraseOverlapIntervals(vvi& intervals) {
    if (intervals.empty()) return 0;
    sort(intervals.begin(), intervals.end(), [](const vi& a, const vi& b) {
        return a[1] < b[1];
    });

    int count = 0, last_end = intervals[0][1];
    for (int i = 1; i < (int)intervals.size(); i++) {
        if (intervals[i][0] < last_end) {
            count++;
        } else {
            last_end = intervals[i][1];
        }
    }

    return count;
}
// Interview Explanation:
// - Problem Statement: Find the minimum number of intervals you need to remove to make the rest of the intervals non-overlapping.
// - Approach: Greedy Interval Scheduling (sort by end time).
// - Intuition: To maximize the number of non-overlapping intervals, always greedily pick the interval that finishes earliest (smallest end time). Any interval starting before the previous chosen interval's end must be removed.
// - Complexity: Time: O(N log N) sorting, Space: O(1) auxiliary space.

bool canAttendMeetings(vvi& intervals) {
    sort(intervals.begin(), intervals.end());
    for (int i = 1; i < (int)intervals.size(); i++) {
        if (intervals[i][0] < intervals[i - 1][1]) return false;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Determine if a person could attend all meetings (check for any overlapping meeting times).
// - Approach: Sorting by start time + adjacent conflict check.
// - Intuition: Sort meetings by start time. If any meeting starts before the previous meeting ends (`intervals[i][0] < intervals[i-1][1]`), a schedule conflict exists; return false.
// - Complexity: Time: O(N log N) sorting, Space: O(1) auxiliary space.

int minMeetingRooms(vvi& intervals) {
    if (intervals.empty()) return 0;
    int n = intervals.size();
    vi start_times(n), end_times(n);
    for (int i = 0; i < n; i++) {
        start_times[i] = intervals[i][0];
        end_times[i] = intervals[i][1];
    }
    sort(start_times.begin(), start_times.end());
    sort(end_times.begin(), end_times.end());

    int rooms = 0, end_ptr = 0;
    for (int i = 0; i < n; i++) {
        if (start_times[i] < end_times[end_ptr]) {
            rooms++;
        } else {
            end_ptr++;
        }
    }

    return rooms;
}
// Interview Explanation:
// - Problem Statement: Find the minimum number of conference rooms required to hold all meetings.
// - Approach: Two Pointers with separately sorted Start and End time arrays (or Min-Heap).
// - Intuition: Sort start times and end times independently. When a meeting starts before the earliest ending meeting finishes (`start[i] < end[end_ptr]`), an additional room is required. Otherwise, an existing room freed up (`end_ptr++`).
// - Complexity: Time: O(N log N) sorting, Space: O(N) start/end arrays.

void setZeroes(vvi& matrix) {
    int m = matrix.size(), n = matrix[0].size();
    bool col0 = false;

    for (int i = 0; i < m; i++) {
        if (matrix[i][0] == 0) col0 = true;
        for (int j = 1; j < n; j++) {
            if (matrix[i][j] == 0) {
                matrix[i][0] = 0;
                matrix[0][j] = 0;
            }
        }
    }

    for (int i = m - 1; i >= 0; i--) {
        for (int j = n - 1; j >= 1; j--) {
            if (matrix[i][0] == 0 || matrix[0][j] == 0) {
                matrix[i][j] = 0;
            }
        }
        if (col0) matrix[i][0] = 0;
    }
}
// Interview Explanation:
// - Problem Statement: Given an m x n matrix, if an element is 0, set its entire row and column to 0 in-place in O(1) extra space.
// - Approach: Use matrix first row and first column as in-place dummy marker arrays.
// - Intuition: Flag 0s in `matrix[i][0]` (row marker) and `matrix[0][j]` (col marker). Track column 0 separately with boolean `col0`. Update inner cells backwards from `(m-1, n-1)` to preserve marker values during traversal.
// - Complexity: Time: O(M * N) two passes, Space: O(1) in-place.

vi spiralOrder(vvi& matrix) {
    if (matrix.empty()) return {};
    int m = matrix.size(), n = matrix[0].size();
    vi ans;
    int top = 0, bottom = m - 1, left = 0, right = n - 1;

    while (top <= bottom && left <= right) {
        for (int j = left; j <= right; j++) ans.push_back(matrix[top][j]);
        top++;
        for (int i = top; i <= bottom; i++) ans.push_back(matrix[i][right]);
        right--;
        if (top <= bottom) {
            for (int j = right; j >= left; j--) ans.push_back(matrix[bottom][j]);
            bottom--;
        }
        if (left <= right) {
            for (int i = bottom; i >= top; i--) ans.push_back(matrix[i][left]);
            left++;
        }
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Return all elements of an m x n matrix in spiral order traversal.
// - Approach: Boundary simulation maintaining four pointers: top, bottom, left, and right.
// - Intuition: Traverse the perimeter layer-by-layer (L->R, T->B, R->L, B->T), shrinking boundaries after each row/col; guard with (top <= bottom) & (left <= right) before reverse traversals.
// - Complexity: Time: O(M * N) visiting each cell once, Space: O(1) auxiliary space (excluding result vector).
