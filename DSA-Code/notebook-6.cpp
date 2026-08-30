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

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Container With Most Water                   | Inward Converging Two Pointers    | O(N)     | O(1)     |
 | 2  | Spiral Matrix Traversal                     | Layer Boundary Simulation         | O(M * N) | O(1)     |
 | 3  | Merge Two Sorted Arrays                     | Two-Pointer Linear Merge Step     | O(N + M) | O(N + M) |
 | 4  | Longest Consecutive Sequence                | Hash Set Boundary (count(x - 1))  | O(N)     | O(N)     |
 | 5  | Row with Maximum 1s in Binary Matrix        | Top-Right Corner Staircase Scan   | O(N + M) | O(1)     |
 | 6  | Find Median from Data Stream (MedianFinder) | Two Heaps (Max-Heap + Min-Heap)   | O(log N) | O(N)     |
 | 7  | First Missing Positive                      | Cyclic Sort In-Place Hashing      | O(N)     | O(1)     |
 | 8  | Word Break                                  | 1D Dynamic Programming + Set      | O(N^2*L) | O(N + D) |
 | 9  | Array Stack Implementation                  | Array with Top Index              | O(1) all | O(Cap)   |
 | 10 | Kth Largest Element in Array                | Min-Heap of Size K                | O(N logK) | O(K)     |
 | 11 | Longest Valid Parentheses                   | Stack of Indices (Base -1)        | O(N)     | O(N)     |
 | 12 | Move Zeroes                                 | Two-Pointer In-Place Swap         | O(N)     | O(1)     |
 | 13 | Majority Element (Boyer-Moore Voting)       | Candidate & Count Cancellation    | O(N)     | O(1)     |
 | 14 | Design HashMap (MyHashMap)                  | Separate Chaining (Bucket Lists)  | Avg O(1) | O(N)     |
 | 15 | Median of Row-Wise Sorted Matrix            | Binary Search on Range + UB       | O(RlogC) | O(1)     |
 | 16 | Kth Missing Positive Number                 | Binary Search on Missing Count    | O(log N) | O(1)     |
 | 17 | Summary Ranges                              | Sorted Set Range Traversal        | O(log N) | O(N)     |
 | 18 | Design Twitter                              | Multi-Way Heap Merge + Hash Map   | O(N logK) | O(U + T) |
 | 19 | Water and Jug Problem (BFS)                 | BFS 6-State Graph Traversal       | O(X * Y) | O(X * Y) |
 | 20 | Wiggle Subsequence                          | Greedy / DP Peak-Valley Tracking  | O(N)     | O(1)     |
 | 21 | Run-Length Encoding (Compress)              | Two-Pointer Suffix Counter Scan   | O(N)     | O(N)     |
 | 22 | Decode String                               | Nested Number & String Stacks     | O(N)     | O(N)     |
 | 23 | 132 Pattern                                 | Monotonic Decreasing Stack        | O(N)     | O(N)     |
 | 24 | Find All Numbers Disappeared in Array       | Cyclic Sort In-Place Hashing      | O(N)     | O(1)     |
 | 25 | Kth Smallest in Lexicographical Order       | Trie Prefix Tree Level Skipping   | O(log^2N) | O(1)     |
 | 26 | Kth Smallest Element in Sorted Matrix       | Binary Search on Value Range + UB | O(NlogM) | O(1)     |
 | 27 | Longest Nice Subarray (Pairwise AND = 0)    | Sliding Window + Cumulative OR    | O(N)     | O(1)     |
 ====================================================================================================
*/

// ============================================================
// 1. CONTAINER WITH MOST WATER
// ============================================================

int maxArea(vector<int>& height) {
    int l = 0, r = height.size() - 1, ans = 0;
    while (l < r) {
        ans = max(ans, min(height[l], height[r]) * (r - l));
        if (height[l] < height[r]) l++; else r--;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find two lines that together with the x-axis form a container holding the most water.
// - Approach: Two pointers starting from outermost boundaries (l = 0, r = n - 1) moving inward.
// - Intuition: Area is limited by the shorter line; shrinking width can only improve area if height increases, so advance the shorter line.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

// ============================================================
// 2. SPIRAL MATRIX TRAVERSAL
// ============================================================

vi spiralOrder(vvi &matrix) {
    if (matrix.empty()) return {};
    int m = matrix.size(), n = matrix[0].size(), top = 0, bottom = m - 1, left = 0, right = n - 1;
    vi ans;
    while (top <= bottom && left <= right) {
        for (int j = left; j <= right; j++) ans.push_back(matrix[top][j]);
        top++;
        for (int i = top; i <= bottom; i++) ans.push_back(matrix[i][right]);
        right--;
        if (top <= bottom) { for (int j = right; j >= left; j--) ans.push_back(matrix[bottom][j]); bottom--; }
        if (left <= right) { for (int i = bottom; i >= top; i--) ans.push_back(matrix[i][left]); left++; }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Return all elements of an m x n matrix in spiral order traversal.
// - Approach: Layer boundary simulation maintaining four pointers: top, bottom, left, and right.
// - Intuition: Traverse perimeter layer-by-layer; shrink boundaries and guard with (top <= bottom) & (left <= right).
// - Complexity: Time: O(M * N), Space: O(1) auxiliary space.

// ============================================================
// 3. MERGE TWO SORTED ARRAYS
// ============================================================

vi merge2sortedarrays(vi& a, vi& b) {
    int n = a.size(), m = b.size(), i = 0, j = 0, k = 0;
    vi merged(n + m);
    while (i < n && j < m) merged[k++] = (a[i] <= b[j]) ? a[i++] : b[j++];
    while (i < n) merged[k++] = a[i++];
    while (j < m) merged[k++] = b[j++];
    return merged;
}
// Interview Explanation:
// - Problem Statement: Merge two already sorted arrays into a single sorted array.
// - Approach: Two-pointer merge technique (standard merge step of Merge Sort).
// - Intuition: Compare elements at current pointers, append the smaller one, and advance; flush leftovers.
// - Complexity: Time: O(N + M), Space: O(N + M).

// ============================================================
// 4. LONGEST CONSECUTIVE SEQUENCE
// ============================================================

int longestConsecutive(vector<int>& nums) {
    unordered_set<int> st(nums.begin(), nums.end()); int longest_streak = 0;
    for (int x : st) {
        if (!st.count(x - 1)) {
            int current_num = x, current_streak = 1;
            while (st.count(current_num + 1)) { current_num++; current_streak++; }
            longest_streak = max(longest_streak, current_streak);
        }
    }
    return longest_streak;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest consecutive elements sequence in an unsorted array in O(N) time.
// - Approach: Hash Set lookup for O(1) average membership checking.
// - Intuition: Only start streak exploration from `x` if `x - 1` is not in set (ensuring true streak start).
// - Complexity: Time: O(N) amortized linear time, Space: O(N) auxiliary space.

// ============================================================
// 5. ROW WITH MAXIMUM 1s IN BINARY MATRIX
// ============================================================

int rowWithMax1s(vector<vector<int>>& mat) {
    int n = mat.size(), m = mat[0].size(), i = 0, j = m - 1, ans = -1;
    while (i < n && j >= 0) {
        if (mat[i][j] == 1) { ans = i; j--; }
        else i++;
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the row index with the maximum number of 1s in a row-sorted binary matrix.
// - Approach: Top-right staircase traversal exploiting row-sorted properties.
// - Intuition: Start at top-right (0, M - 1); if 1 move left (j--), if 0 move down (i++).
// - Complexity: Time: O(N + M), Space: O(1).

// ============================================================
// 6. FIND MEDIAN FROM DATA STREAM (MEDIANFINDER)
// ============================================================

class MedianFinder {
    priority_queue<int> left;
    priority_queue<int, vector<int>, greater<int>> right;
public:
    MedianFinder() {}
    void addNum(int num) {
        left.push(num); right.push(left.top()); left.pop();
        if (right.size() > left.size()) { left.push(right.top()); right.pop(); }
    }
    double findMedian() {
        return left.size() > right.size() ? left.top() : (left.top() + right.top()) / 2.0;
    }
};
// Interview Explanation:
// - Problem Statement: Design a data structure supporting adding numbers from a data stream and finding the current median.
// - Approach: Two Heaps (Max-Heap `left` for lower half, Min-Heap `right` for upper half).
// - Intuition: Maintain balance: size(left) == size(right) (+1 if odd) and max(left) <= min(right).
// - Complexity: Time: O(log N) for addNum, O(1) for findMedian, Space: O(N).

// ============================================================
// 7. FIRST MISSING POSITIVE
// ============================================================

int firstMissingPositive(vector<int>& nums) {
    int n = nums.size();
    for (int i = 0; i < n; ++i)
        while (nums[i] > 0 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) swap(nums[i], nums[nums[i] - 1]);
    for (int i = 0; i < n; ++i) if (nums[i] != i + 1) return i + 1;
    return n + 1;
}
// Interview Explanation:
// - Problem Statement: Find smallest missing positive integer from unsorted array in O(N) time and O(1) space.
// - Approach: Cyclic Sort / in-place array hashing (value x mapped to index x - 1).
// - Intuition: Swap numbers x in [1, N] to index x - 1; first index i where nums[i] != i + 1 gives missing i + 1.
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 8. WORD BREAK
// ============================================================

class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        int n = s.size(); vector<bool> dp(n + 1, false); dp[0] = true;
        for (int i = 1; i <= n; i++)
            for (int j = 0; j < i; j++)
                if (dp[j] && dict.count(s.substr(j, i - j))) { dp[i] = true; break; }
        return dp[n];
    }
};
// Interview Explanation:
// - Problem Statement: Determine if a string can be segmented into a sequence of dictionary words.
// - Approach: Bottom-up 1D Dynamic Programming with Hash Set for dictionary lookup.
// - Intuition: dp[i] is true if prefix s[0...i-1] can be segmented via split point j < i with dp[j] true and s[j...i-1] in dict.
// - Complexity: Time: O(N^2 * L), Space: O(N + D).

// ============================================================
// 9. ARRAY STACK IMPLEMENTATION
// ============================================================

class Stack {
    vector<int> arr; int topIndex, capacity;
public:
    Stack(int cap) : capacity(cap), topIndex(-1), arr(cap) {}
    void push(int x) { if (topIndex == capacity - 1) throw runtime_error("Overflow"); arr[++topIndex] = x; }
    int pop() { if (topIndex == -1) throw runtime_error("Underflow"); return arr[topIndex--]; }
    int peek() { if (topIndex == -1) throw runtime_error("Empty"); return arr[topIndex]; }
    bool isEmpty() { return topIndex == -1; }
    int size() { return topIndex + 1; }
};
// Interview Explanation:
// - Problem Statement: Implement a stack with basic operations (push, pop, peek, isEmpty, size).
// - Approach: Array with top index pointer.
// - Intuition: LIFO structure using contiguous memory with direct index tracking.
// - Complexity: Time: O(1) all operations, Space: O(Cap).

// ============================================================
// 10. KTH LARGEST ELEMENT IN ARRAY
// ============================================================

int kthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> minHeap;
    for (int num : nums) {
        minHeap.push(num);
        if (minHeap.size() > k) minHeap.pop();
    }
    return minHeap.top();
}
// Interview Explanation:
// - Problem Statement: Find the k-th largest element in an unsorted array.
// - Approach: Min-Heap of size k.
// - Intuition: Min-heap of size k retains the k largest elements; top holds the k-th largest.
// - Complexity: Time: O(N log K), Space: O(K).

// ============================================================
// 11. LONGEST VALID PARENTHESES
// ============================================================

int longestValidParenthesis(string s) {
    stack<int> st; st.push(-1); int ans = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') st.push(i);
        else {
            st.pop();
            if (st.empty()) st.push(i); else ans = max(ans, i - st.top());
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest valid parentheses substring.
// - Approach: Stack of indices initialized with base boundary -1.
// - Intuition: Push '(' indices; on ')' pop match; if empty push new base, else record length `i - st.top()`.
// - Complexity: Time: O(N), Space: O(N).

// ============================================================
// 12. MOVE ZEROES
// ============================================================

void moveZeroes(vector<int>& nums) {
    int lastNonZero = 0;
    for (int i = 0; i < nums.size(); i++) if (nums[i] != 0) swap(nums[lastNonZero++], nums[i]);
}
// Interview Explanation:
// - Problem Statement: Move all zeroes to the end while maintaining relative order of non-zero elements.
// - Approach: Two-pointer in-place swap.
// - Intuition: Swap non-zero elements into `lastNonZero` pointer position and advance.
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 13. MAJORITY ELEMENT (BOYER-MOORE VOTING)
// ============================================================

int majorityElement(vi &nums) {
    int count = 0, candidate = 0;
    for (int num : nums) {
        if (count == 0) candidate = num;
        count += (num == candidate) ? 1 : -1;
    }
    return candidate;
}
// Interview Explanation:
// - Problem Statement: Find majority element (> n/2 occurrences).
// - Approach: Boyer-Moore Voting Algorithm.
// - Intuition: Increment count on match, decrement on mismatch; majority candidate survives cancellations.
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 14. DESIGN HASHMAP (MYHASHMAP)
// ============================================================

class MyHashMap {
    static const int SIZE = 1000;
    vector<list<pair<int, int>>> buckets;
    int hash(int key) { return key % SIZE; }
public:
    MyHashMap() : buckets(SIZE) {}
    void put(int key, int value) {
        int idx = hash(key);
        for (auto& [k, v] : buckets[idx]) if (k == key) { v = value; return; }
        buckets[idx].push_back({key, value});
    }
    int get(int key) {
        for (auto& [k, v] : buckets[hash(key)]) if (k == key) return v;
        return -1;
    }
    void remove(int key) {
        int idx = hash(key);
        for (auto it = buckets[idx].begin(); it != buckets[idx].end(); ++it)
            if (it->first == key) { buckets[idx].erase(it); return; }
    }
};
// Interview Explanation:
// - Problem Statement: Implement a basic HashMap with put, get, and remove operations.
// - Approach: Chaining via array of linked lists (buckets).
// - Intuition: Hash function maps keys to bucket indices; separate chaining handles collisions.
// - Complexity: Time: O(1) average, Space: O(N).

// ============================================================
// 15. MEDIAN OF ROW-WISE SORTED MATRIX
// ============================================================

int median(vector<vector<int>>& mat) {
    int r = mat.size(), c = mat[0].size(), lo = mat[0][0], hi = mat[0][c - 1];
    for (auto& row : mat) { lo = min(lo, row[0]); hi = max(hi, row[c - 1]); }
    int need = (r * c) / 2 + 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2, cnt = 0;
        for (auto& row : mat) cnt += upper_bound(row.begin(), row.end(), mid) - row.begin();
        if (cnt >= need) hi = mid; else lo = mid + 1;
    }
    return lo;
}
// Interview Explanation:
// - Problem Statement: Find the median of a row-wise sorted matrix.
// - Approach: Binary search on value range [lo, hi] + upper_bound count per row.
// - Intuition: Count elements <= mid across all rows; binary search narrow down to the element with >= (r*c)/2 + 1 elements.
// - Complexity: Time: O(R * log C * log(max - min)), Space: O(1).

// ============================================================
// 16. KTH MISSING POSITIVE NUMBER
// ============================================================

int kthMissingPositive(vector<int>& arr, int k) {
    int low = 0, high = arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] - (mid + 1) < k) low = mid + 1; else high = mid - 1;
    }
    return low + k;
}
// Interview Explanation:
// - Problem Statement: Find the k-th missing positive integer in a sorted array.
// - Approach: Binary search on missing count `arr[mid] - (mid + 1)`.
// - Intuition: If missing count < k, search right half; final answer is low + k.
// - Complexity: Time: O(log N), Space: O(1).

// ============================================================
// 17. SUMMARY RANGES
// ============================================================

class SummaryRanges {
    set<int> nums;
public:
    SummaryRanges() {}
    void addNum(int val) { nums.insert(val); }
    vector<vector<int>> getIntervals() {
        if (nums.empty()) return {};
        vector<vector<int>> intervals;
        int start = *nums.begin(), end = start;
        for (int x : nums) {
            if (x == end + 1) end = x;
            else if (x != start) { intervals.push_back({start, end}); start = end = x; }
        }
        intervals.push_back({start, end});
        return intervals;
    }
};
// Interview Explanation:
// - Problem Statement: Maintain disjoint intervals as numbers are added dynamically.
// - Approach: Sorted Set + Linear Interval grouping.
// - Intuition: Iterate through sorted unique values and group consecutive numbers into [start, end].
// - Complexity: Time: O(log N) for addNum, O(N) for getIntervals, Space: O(N).

// ============================================================
// 18. DESIGN TWITTER
// ============================================================

class Twitter {
    using P = pair<int, int>;
    unordered_map<int, unordered_set<int>> followees;
    unordered_map<int, vector<P>> tweets;
    int timer = 0;
public:
    Twitter() {}
    void postTweet(int userId, int tweetId) { tweets[userId].push_back({timer++, tweetId}); }
    vector<int> getNewsFeed(int userId) {
        priority_queue<tuple<int, int, int>> pq;
        if (!tweets[userId].empty()) pq.push({tweets[userId].back().first, userId, (int)tweets[userId].size() - 1});
        for (int f : followees[userId])
            if (!tweets[f].empty()) pq.push({tweets[f].back().first, f, (int)tweets[f].size() - 1});
        vector<int> res;
        while (!pq.empty() && res.size() < 10) {
            auto [time, uid, idx] = pq.top(); pq.pop();
            res.push_back(tweets[uid][idx].second);
            if (idx > 0) pq.push({tweets[uid][idx - 1].first, uid, idx - 1});
        }
        return res;
    }
    void follow(int followerId, int followeeId) { if (followerId != followeeId) followees[followerId].insert(followeeId); }
    void unfollow(int followerId, int followeeId) { followees[followerId].erase(followeeId); }
};
// Interview Explanation:
// - Problem Statement: Design Twitter news feed retrieval of 10 most recent tweets.
// - Approach: Hash Maps + Multi-way Max-Heap Merge on tweet timestamp indices.
// - Intuition: Track tweets with timestamps per user; feed merges tops of recent lists using a priority queue.
// - Complexity: Time: O(K log F) for 10 tweets where F is followees, Space: O(Total Tweets + Follows).

// ============================================================
// 19. WATER AND JUG PROBLEM (BFS)
// ============================================================

bool canMeasureWater(int x, int y, int target) {
    if (target > x + y) return false;
    queue<pair<int, int>> q; set<pair<int, int>> vis;
    q.push({0, 0}); vis.insert({0, 0});
    while (!q.empty()) {
        auto [a, b] = q.front(); q.pop();
        if (a + b == target) return true;
        vector<pair<int, int>> next = {
            {x, b}, {a, y}, {0, b}, {a, 0},
            {max(0, a - (y - b)), min(y, b + a)}, {min(x, a + b), max(0, b - (x - a))}
        };
        for (auto state : next) if (!vis.count(state)) { vis.insert(state); q.push(state); }
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Determine if exactly target liters can be measured using two jugs.
// - Approach: BFS state-space search over 6 transitions (fill, empty, pour).
// - Intuition: Model each jug state as (a, b) and traverse reachable states using BFS with visited set.
// - Complexity: Time: O(x * y), Space: O(x * y).

// ============================================================
// 20. WIGGLE SUBSEQUENCE
// ============================================================

int wiggleMaxLength(vector<int>& nums) {
    if (nums.size() < 2) return nums.size();
    int up = 1, down = 1;
    for (int i = 1; i < nums.size(); i++) {
        if (nums[i] > nums[i - 1]) up = down + 1;
        else if (nums[i] < nums[i - 1]) down = up + 1;
    }
    return max(up, down);
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest alternating wiggle subsequence.
// - Approach: Greedy / 1D DP tracking `up` and `down` difference transitions.
// - Intuition: Maintain longest sequence ending with positive vs negative slope; update mutually.
// - Complexity: Time: O(N), Space: O(1).

// ============================================================
// 21. RUN-LENGTH ENCODING (COMPRESS)
// ============================================================

string encode(string s) {
    string res;
    for (int i = 0; i < s.size(); ) {
        int j = i;
        while (j < s.size() && s[j] == s[i]) j++;
        res += s[i] + to_string(j - i); i = j;
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Compress a string by replacing runs of identical characters with char + count.
// - Approach: Two-pointer run scanning.
// - Intuition: Pointer j locates end of matching run; append char and run length.
// - Complexity: Time: O(N), Space: O(N).

// ============================================================
// 22. DECODE STRING
// ============================================================

string decodeString(string s) {
    stack<int> nums; stack<string> strs;
    int num = 0; string cur;
    for (char c : s) {
        if (isdigit(c)) num = num * 10 + (c - '0');
        else if (c == '[') { nums.push(num); strs.push(cur); num = 0; cur = ""; }
        else if (c == ']') {
            int k = nums.top(); nums.pop();
            string prev = strs.top(); strs.pop();
            while (k--) prev += cur;
            cur = prev;
        } else cur += c;
    }
    return cur;
}
// Interview Explanation:
// - Problem Statement: Decode string format k[encoded_string].
// - Approach: Two Stacks (repeat count stack + prefix string stack).
// - Intuition: Push on '[', pop and repeat string on ']' to resolve nested encodings.
// - Complexity: Time: O(N), Space: O(N).

// ============================================================
// 23. 132 PATTERN
// ============================================================

bool find132pattern(vector<int>& nums) {
    int n = nums.size(), third = INT_MIN; stack<int> st;
    for (int i = n - 1; i >= 0; i--) {
        if (nums[i] < third) return true;
        while (!st.empty() && nums[i] > st.top()) { third = st.top(); st.pop(); }
        st.push(nums[i]);
    }
    return false;
}
// Interview Explanation:
// - Problem Statement: Find if there exist i < j < k with nums[i] < nums[k] < nums[j].
// - Approach: Monotonic Decreasing Stack iterating backwards to maintain candidate "2" (`third`) and "3".
// - Intuition: When nums[i] > stack top, pop to maximize candidate `third`; if nums[i] < third, valid "1" is found.
// - Complexity: Time: O(N), Space: O(N).

// ============================================================
// 24. FIND ALL NUMBERS DISAPPEARED IN ARRAY
// ============================================================

vi findDisappearedNumbers(vi &nums) {
    int n = nums.size();
    for (int i = 0; i < n; ++i)
        while (nums[i] != nums[nums[i] - 1]) swap(nums[i], nums[nums[i] - 1]);
    vi result;
    for (int i = 0; i < n; ++i) if (nums[i] != i + 1) result.push_back(i + 1);
    return result;
}
// Interview Explanation:
// - Problem Statement: Find all missing numbers from array in range [1, n].
// - Approach: Cyclic Sort in-place index mapping.
// - Intuition: Swap nums[i] to nums[i] - 1; indices where nums[i] != i + 1 reveal missing numbers.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.

// ============================================================
// 25. KTH SMALLEST IN LEXICOGRAPHICAL ORDER
// ============================================================

class LexicographicalKth {
public:
    int findKthNumber(int n, int k) {
        int curr = 1; k--;
        while (k > 0) {
            long long steps = 0, first = curr, last = curr + 1;
            while (first <= n) { steps += min((long long)n + 1, last) - first; first *= 10; last *= 10; }
            if (steps <= k) { curr++; k -= steps; }
            else { curr *= 10; k--; }
        }
        return curr;
    }
};
// Interview Explanation:
// - Problem Statement: Find the k-th smallest integer in lexicographical order from 1 to n.
// - Approach: Trie / Prefix Tree Level-by-Level Skipping.
// - Intuition: Count numbers in prefix subtree [curr, curr+1); skip whole subtree if steps <= k, else go deeper.
// - Complexity: Time: O(log^2 N), Space: O(1).

// ============================================================
// 26. KTH SMALLEST ELEMENT IN SORTED MATRIX
// ============================================================

int kthElementInSortedMatrix(vector<vector<int>>& mat, int k) {
    int n = mat.size(), m = mat[0].size(), lo = mat[0][0], hi = mat[n - 1][m - 1];
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2, count = 0;
        for (int i = 0; i < n; i++) count += upper_bound(mat[i].begin(), mat[i].end(), mid) - mat[i].begin();
        if (count < k) lo = mid + 1; else hi = mid;
    }
    return lo;
}
// Interview Explanation:
// - Problem Statement: Find k-th smallest element in a row & column sorted matrix.
// - Approach: Binary Search on value range + upper_bound per row.
// - Intuition: Count elements <= mid; narrow value range until low == high.
// - Complexity: Time: O(N log M * log(max - min)), Space: O(1).

// ============================================================
// 27. LONGEST NICE SUBARRAY (PAIRWISE AND = 0)
// ============================================================

class LongestNiceSubarray {
public:
    int longestNiceSubarray(vector<int>& nums) {
        int n = nums.size(), left = 0, mask = 0, ans = 0;
        for (int right = 0; right < n; right++) {
            while ((mask & nums[right]) != 0) { mask ^= nums[left]; left++; }
            mask |= nums[right];
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find length of longest subarray where bitwise AND of every pair is 0.
// - Approach: Sliding Window with cumulative bitmask.
// - Intuition: Maintain window bitmask with XOR/OR; shrink left whenever adding nums[right] introduces a bit collision.
// - Complexity: Time: O(N), Space: O(1).
