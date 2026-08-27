#include <bits/stdc++.h>
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


int maxArea(vector<int>& height) {
    int l = 0, r = height.size() - 1;
    int ans = 0;

    while (l < r) {
        ans = max(ans, min(height[l], height[r]) * (r - l));
        if (height[l] < height[r]) l++;
        else r--;
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Container with most water leetcode problem
// - Approach: Two pointers starting from outermost boundaries (l = 0, r = n - 1) moving inward.
// - Intuition: Area is limited by the shorter line; shrinking width can only yield a larger area if height increases, so greedily advance the shorter line.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.

vi spiralOrder(vvi &matrix) {
    if (matrix.empty()) return {};
    int m = matrix.size(), n = matrix[0].size();
    vi ans;
    int top = 0, bottom = m - 1, left = 0, right = n - 1;

    while (top <= bottom && left <= right) {
        for (int j = left; j <= right; j++) ans.push_back(matrix[top][j]);
        top++;
        for (int i = top; i <= bottom; i++) ans.push_back(matrix[i][right]);
        right--;
        if (top <= bottom) { // this is because we need to check if there are still rows left to traverse after moving the top pointer down
            for (int j = right; j >= left; j--) ans.push_back(matrix[bottom][j]);
            bottom--;
        }
        if (left <= right) { // this is because we need to check if there are still columns left to traverse after moving the right pointer left
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

vi merge2sortedarrays(vi& a, vi& b) {
    int n = a.size(), m = b.size();
    vi merged(n + m);
    int i = 0, j = 0, k = 0;

    while (i < n && j < m) {
        if (a[i] <= b[j]) {
            merged[k++] = a[i++];
        } else {
            merged[k++] = b[j++];
        }
    }

    while (i < n) {
        merged[k++] = a[i++];
    }

    while (j < m) {
        merged[k++] = b[j++];
    }

    return merged;
}
// Interview Explanation:
// - Problem Statement: Merge two already sorted arrays into a single sorted array.
// - Approach: Two-pointer merge technique (standard merge step of Merge Sort).
// - Intuition: Compare elements at current pointers, push the smaller one into the merged array, and advance its pointer; append leftover elements once one array is exhausted.
// - Complexity: Time: O(N + M) linear scan, Space: O(N + M) for the merged array (or O(1) extra if merging in-place from back).

int longestConsecutive(vector<int>& nums) {
    unordered_set<int> st(nums.begin(), nums.end());
    int longest_streak = 0;

    for (int x : st) {
        if (!st.count(x - 1)) { // only start counting if 'x' is the start of a sequence
            int current_num = x;
            int current_streak = 1;

            while (st.count(current_num + 1)) {
                current_num++;
                current_streak++;
            }

            longest_streak = max(longest_streak, current_streak);
        }
    }

    return longest_streak;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest consecutive elements sequence in an unsorted array in O(N) time.
// - Approach: Hash Set lookup for O(1) average membership checking.
// - Intuition: Only start streak exploration from `x` if `x - 1` is not in the set (ensuring `x` is the true start); ensures each number is visited at most twice.
// - Complexity: Time: O(N) amortized linear time, Space: O(N) auxiliary space to store elements in unordered_set.

int rowWithMax1s(vector<vector<int>>& mat) {
    int n = mat.size();
    int m = mat[0].size();

    int i = 0, j = m - 1;
    int ans = -1;

    while (i < n && j >= 0) {
        if (mat[i][j] == 1) {
            ans = i;
            j--;          // look for an earlier 1
        } else {
            i++;          // current row can't improve
        }
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the index of the row with the maximum number of 1s in a row-sorted binary matrix.
// - Approach: Top-right staircase traversal exploiting row-sorted binary properties.
// - Intuition: Start at top-right (0, M - 1); if mat[i][j] == 1, record row and move left (j--) to check for more 1s; if 0, move down (i++) since current row cannot beat the record.
// - Complexity: Time: O(N + M) traversing at most N rows and M cols, Space: O(1) auxiliary space (optimal vs O(N log M) binary search).

class MedianFinder {
private:
    priority_queue<int> left; // Max heap (smaller half)
    priority_queue<int, vector<int>, greater<int>> right; // Min heap (larger half)

public:
    MedianFinder() {
    }

    void addNum(int num) {
        left.push(num);

        // Move the largest element from left to right
        right.push(left.top());
        left.pop();

        // Ensure left has at least as many elements as right
        if (right.size() > left.size()) {
            left.push(right.top());
            right.pop();
        }
    }

    double findMedian() {
        if (left.size() > right.size()) {
            return left.top();
        }
        return (left.top() + right.top()) / 2.0;
    }
};
// Interview Explanation:
// - Problem Statement: Design a data structure supporting adding numbers from a data stream and finding the current median.
// - Approach: Two Heaps (Max-Heap `left` for lower half, Min-Heap `right` for upper half).
// - Intuition: Maintain balance invariant: size(left) == size(right) (+1 if odd) and max(left) <= min(right); median is left.top() or the average of both heap tops.
// - Complexity: Time: O(log N) for addNum, O(1) for findMedian, Space: O(N) to store incoming elements.

int firstMissingPositive(vector<int>& nums) {
    int n = nums.size();

    for (int i = 0; i < n; ++i) {
        while (nums[i] > 0 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) {
            swap(nums[i], nums[nums[i] - 1]);
        }
    }

    for (int i = 0; i < n; ++i) {
        if (nums[i] != i + 1) {
            return i + 1;
        }
    }

    return n + 1;
}
// Interview Explanation:
// - Problem Statement: Find the smallest missing positive integer from an unsorted array in O(N) time and O(1) extra space.
// - Approach: Cyclic Sort / in-place array hashing (values in [1, N] mapped to indices [0, N - 1]).
// - Intuition: Answer must lie in [1, N + 1]. Repeatedly swap positive numbers x in [1, N] to index x - 1; first index i where nums[i] != i + 1 reveals the missing integer i + 1.
// - Complexity: Time: O(N) as each swap places at least one number in its correct bucket, Space: O(1) auxiliary in-place.

class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());

        int n = s.size();
        vector<bool> dp(n + 1, false);

        // Empty string can always be segmented
        dp[0] = true;

        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < i; j++) {
                // s[0...j-1] is segmentable
                // and s[j...i-1] is a dictionary word
                if (dp[j] && dict.count(s.substr(j, i - j))) {
                    dp[i] = true;
                    break;
                }
            }
        }

        return dp[n];
    }
};
// Interview Explanation:
// - Problem Statement: Determine if a string can be segmented into a space-separated sequence of dictionary words.
// - Approach: Bottom-up 1D Dynamic Programming with Hash Set for dictionary lookup.
// - Intuition: dp[i] is true if prefix s[0...i-1] can be segmented; find a split point j < i where dp[j] is true and substring s[j...i-1] exists in the dictionary.
// - Complexity: Time: O(N^2 * L) where N = s.length() and L is substring hashing time, Space: O(N + D) for DP table and word set.

class Stack {
    private int[] arr;
    private int top;
    private int capacity;

    Stack(int capacity) {
        this.capacity = capacity;
        arr = new int[capacity];
        top = -1;
    }

    // Add element
    void push(int x) {
        if (top == capacity - 1) {
            throw new RuntimeException("Stack Overflow");
        }

        arr[++top] = x;
    }

    // Remove and return top element
    int pop() {
        if (top == -1) {
            throw new RuntimeException("Stack Underflow");
        }

        return arr[top--];
    }

    // Return top without removing
    int peek() {
        if (top == -1) {
            throw new RuntimeException("Stack is Empty");
        }

        return arr[top];
    }

    boolean isEmpty() {
        return top == -1;
    }

    int size() {
        return top + 1;
    }
}

// Interview Explanation:
// - Problem Statement: Implement a stack with basic operations (push, pop, peek, isEmpty, size).
// - Approach: Use an array to store elements and maintain a top index.
// - Complexity: Time: O(1) for all operations, Space: O(N) where N is the capacity of the stack.
// - Intuition: Stack is a LIFO data structure; array provides contiguous memory for efficient access, and top index tracks the current top element.

int kthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> minHeap;

    for (int num : nums) {
        minHeap.push(num);
        if (minHeap.size() > k) {
            minHeap.pop();
        }
    }

    return minHeap.top();
}
// Interview Explanation:
// - Problem Statement: Find the k-th largest element in an unsorted array.
// - Approach: Min-Heap of size k to maintain the k largest elements seen so far.
// - Intuition: The top of the min-heap will always be the k-th largest element after processing all elements; if the heap exceeds size k, remove the smallest to keep only the k largest.
// - Complexity: Time: O(N log K) for N elements, Space: O(K) for the heap.

int longestValidParenthesis(string s) {
    stack<int> st; // stack stores the indices of characters in the string, it stores the index of the last unmatched '(' or the base index for valid substrings
    st.push(-1);  // base index

    int ans = 0;

    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            st.push(i);
        } else {
            st.pop(); // if s[i] == ')', pop the last index, because we are trying to find a matching '(' for this ')'
            if (st.empty()) st.push(i); // if stack becomes empty, push the current index as a new base for future valid substrings
            else ans = max(ans, i - st.top());
        }
    }

    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the length of the longest valid (well-formed) parentheses substring.
// - Approach: Stack of indices initialized with base boundary `-1`.
// - Intuition: We will push the index of '(' onto the stack. When we encounter a ')', we pop the top index (which should be the matching '('). If the stack becomes empty after popping, it means we have found a valid substring, and we can calculate its length by subtracting the current index from the new top of the stack. If the stack is not empty, we continue to check for longer valid substrings.
// - Complexity: Time: O(N) single pass, Space: O(N) auxiliary space for stack.

void moveZeroes(vector<int>& nums) {
    int lastNonZeroFoundAt = 0;

    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] != 0) {
            swap(nums[lastNonZeroFoundAt++], nums[i]);
        }
    }
}
// Interview Explanation:
// - Problem Statement: Move all zeroes in an array to the end while maintaining the relative order of non-zero elements.
// - Approach: Two-pointer technique where one pointer iterates through the array and the other keeps track of the position to place the next non-zero element.
// - Intuition: When a non-zero element is found, it is swapped with the element at the last non-zero index, effectively moving all zeroes to the end while preserving the order of non-zero elements.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space in-place.

int majorityElement(vi &nums) {
    int count = 0, candidate = 0;

    for (int num : nums) {
        if (count == 0) {
            candidate = num;
        }
        if(num == candidate) {
            count++;
        } else {
            count--;
        }
    }

    return candidate;
}
// Interview Explanation:
// - Problem Statement: Find the majority element in an array (the element that appears more than n/2 times).
// - Approach: Boyer-Moore Voting Algorithm.
// - Intuition: The algorithm maintains a count of the current candidate for majority element. When the count drops to zero, a new candidate is chosen. The majority element will always be the last candidate standing after processing the entire array.
// - Complexity: Time: O(N) single pass, Space: O(1) auxiliary space.