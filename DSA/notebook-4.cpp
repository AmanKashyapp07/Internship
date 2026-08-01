#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <cstring>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vvi = vector<vector<int>>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;
const ll P = 31;

int kthElement(vector<int>& nums1, vector<int>& nums2, int k) {
    if (nums1.size() > nums2.size())
        return kthElement(nums2, nums1, k);

    int m = nums1.size();
    int n = nums2.size();

    int left = max(0, k - n);
    int right = min(k, m);

    while (left <= right) {
        int i = left + (right - left) / 2;
        int j = k - i;

        int L1 = (i == 0) ? INT_MIN : nums1[i - 1];
        int R1 = (i == m) ? INT_MAX : nums1[i];

        int L2 = (j == 0) ? INT_MIN : nums2[j - 1];
        int R2 = (j == n) ? INT_MAX : nums2[j];

        if (L1 <= R2 && L2 <= R1) {
            return max(L1, L2);
        }
        else if (L1 > R2) {
            right = i - 1;
        }
        else {
            left = i + 1;
        }
    }

    return -1; // unreachable
} 
// For Median 
// int k = (m + n + 1) / 2;
// if ((m + n) % 2 == 1) return kthElement(nums1, nums2, k);
// else return (kthElement(nums1, nums2, k) + kthElement(nums1, nums2, k + 1)) / 2.0;

void nextPermutation(vector<int>& nums) {
    int n = nums.size();
    int i = n - 2;

    while (i >= 0 && nums[i] >= nums[i + 1]) {
        i--;
    } // first find the first decreasing element from the right

    if (i >= 0) { // if the entire array is not in descending order
        int j = n - 1;
        while (nums[j] <= nums[i]) {
            j--;
        }
        swap(nums[i], nums[j]);
    }

    reverse(nums.begin() + i + 1, nums.end()); // reverse the elements to the right of i to get the next permutation

    // eg - 2 3 1 5 4 
    // i = 1, j = 4, swap(3,4) => 2 4 1 5 3 => reverse(1,5,3) => 2 4 3 5 1
}

int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> lastSeen;
        int left = 0;
        int max_len = 0;

        for (int right = 0; right < s.size(); right++) {
            if (lastSeen.count(s[right]) &&
                lastSeen[s[right]] >= left) {
                left = lastSeen[s[right]] + 1;
            } // If the character has been seen and is within the current window, move the left pointer to one position right of its last seen index, if the char was seen and not in the current window, we don't need to move the left pointer, as it is already in the correct position

            lastSeen[s[right]] = right;
            max_len = max(max_len, right - left + 1);
        }

        return max_len;
} // returns length of longest substring without repeating characters in O(n) time and O(min(n, a)) space where a is the size of the alphabet.


long long minimumAbsoluteDifferenceSum(vector<int> &a) {

    int n = a.size();

    sort(a.begin(), a.end());

    int median = a[n / 2];   // Works for both odd and even n

    long long sum = 0;

    for (int x : a)

        sum += abs(1LL * x - median);

    return sum;

}


int minMoves(vector<int>& nums, int k) {
    /*
        Let the positions of all 1's be:
            pos[] = {p1, p2, ...}

        Suppose we want these k ones to occupy consecutive positions:
            x, x+1, x+2, ...

        Cost for ith selected 1:
            |pos[i] - (x + i)|
          = |(pos[i] - i) - x|

        Hence define:
            graphusted[i] = pos[i] - i

        Now every selected element wants to become the SAME value x.
        The minimum sum of absolute differences is achieved at the median.

        Using prefix sums on graphusted[], we compute the cost of every
        window of k ones in O(1), giving overall O(n).
    */

    vector<long long> pos;
    for (int i = 0; i < nums.size(); i++)
        if (nums[i]) pos.push_back(i);

    int m = pos.size();

    vector<long long> graphusted(m), prefix(m + 1);
    for (int i = 0; i < m; i++) {
        graphusted[i] = pos[i] - i;
        prefix[i + 1] = prefix[i] + graphusted[i];
    }

    long long ans = LLONG_MAX;

    for (int left = 0; left + k <= m; left++) {
        int right = left + k - 1, mid = (left + right) / 2;
        long long median = graphusted[mid];

        ans = min(ans,
            median * (mid - left) - (prefix[mid] - prefix[left]) +
            (prefix[right + 1] - prefix[mid + 1]) - median * (right - mid)
        );
    }

    return ans;
}

vector<vector<int>> 2dPrefixSum(vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();

    vector<vector<int>> prefix(m, vector<int>(n));

    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            prefix[i][j] = matrix[i][j];
            if(i > 0) prefix[i][j] += prefix[i - 1][j];
            if(j > 0) prefix[i][j] += prefix[i][j - 1];
            if(i > 0 && j > 0) prefix[i][j] -= prefix[i - 1][j - 1];
        }
    }
    return prefix;
}

class Solution {
public:
    long long countSubarrays(vector<int>& nums, int k) {
        long long ans = 0;
        unordered_map<int, long long> prev, cur;

        for (int x : nums) {
            cur.clear();
            cur[x]++;

            for (auto [v, cnt] : prev)
                cur[v & x] += cnt;

            ans += cur[k];
            prev = move(cur);
        }

        return ans;
    }
}; // count subarrays with bitwise AND equal to k in O(n * log(max(nums))) time and O(n) space.


vector<vector<int>> MultiplyMatrices(vector<vector<int>>& A, vector<vector<int>>& B) {
    int m = A.size();
    int n = A[0].size();
    int p = B[0].size();

    vector<vector<int>> C(m, vector<int>(p, 0));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
} // multiplies two matrices A and B in O(m * n * p) time and O(m * p) space. Here A is of size m x n and B is of size n x p.

long long countSubsequences(vector<int>& nums, int k) {
    vector<long long> dp(k + 1, 0);

    dp[0] = 1;  // Empty subsequence

    for (int x : nums) {
        for (int sum = k; sum >= x; sum--) {
            dp[sum] += dp[sum - x];
        }
    }

    return dp[k];
}

struct Node {
    int value, height;
    Node *left, *right;

    Node(int x) : value(x), height(1), left(nullptr), right(nullptr) {}
};

struct AVLTree {
    int getHeight(Node* node) {
        return node ? node->height : 0;
    }

    void updateHeight(Node* node) {
        if (node)
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }

    int getBalance(Node* node) {
        return getHeight(node->left) - getHeight(node->right);
    }

    Node* rotateRight(Node* root) {
        Node* newRoot = root->left;
        Node* subtree = newRoot->right;

        newRoot->right = root;
        root->left = subtree;

        updateHeight(root);
        updateHeight(newRoot);

        return newRoot;
    }

    Node* rotateLeft(Node* root) {
        Node* newRoot = root->right;
        Node* subtree = newRoot->left;

        newRoot->left = root;
        root->right = subtree;

        updateHeight(root);
        updateHeight(newRoot);

        return newRoot;
    }

    Node* insert(Node* root, int value) {
        if (!root) return new Node(value);

        if (value < root->value)
            root->left = insert(root->left, value);
        else if (value > root->value)
            root->right = insert(root->right, value);
        else
            return root;

        updateHeight(root);

        // LL
        if (getBalance(root) > 1 && value < root->left->value)
            return rotateRight(root);

        // RR
        if (getBalance(root) < -1 && value > root->right->value)
            return rotateLeft(root);

        // LR
        if (getBalance(root) > 1 && value > root->left->value) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        // RL
        if (getBalance(root) < -1 && value < root->right->value) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    int sumOfLeaveNodes(Node* root) {
        if (!root) return 0;
        if (!root->left && !root->right) return root->value;
        return sumOfLeaveNodes(root->left) + sumOfLeaveNodes(root->right);
    }

};

struct Dinic {
    struct Edge {
        int to, rev;
        ll cap;
    };

    int nodes;
    vector<vector<Edge>> adj;
    vector<int> level, it;

    Dinic(int n): nodes(n), adj(n + 1), level(n + 1), it(n + 1) {}

    // directed edge
    void addEdge(int u, int v, ll cap) {
        adj[u].push_back({v, (int)adj[v].size(), cap});
        adj[v].push_back({u, (int)adj[u].size() - 1, 0});
    }

    // build level graph
    bool bfs(int src, int sink) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(src), level[src] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : adj[u])
                if (e.cap && level[e.to] == -1)
                    level[e.to] = level[u] + 1, q.push(e.to);
        }
        return level[sink] != -1;
    }

    // blocking flow
    ll dfs(int u, int sink, ll flow) {
        if (u == sink || !flow) return flow;
        for (int &i = it[u]; i < adj[u].size(); i++) {
            auto &e = adj[u][i];
            if (level[e.to] != level[u] + 1 || !e.cap) continue;
            ll pushed = dfs(e.to, sink, min(flow, e.cap));
            if (pushed)
                return e.cap -= pushed, adj[e.to][e.rev].cap += pushed, pushed;
        }
        return 0;
    }

    // max flow
    ll maxFlow(int src, int sink) {
        ll flow = 0, pushed;
        while (bfs(src, sink)) {
            fill(it.begin(), it.end(), 0);
            while ((pushed = dfs(src, sink, LLONG_MAX)))
                flow += pushed;
        }
        return flow;
    }
};
// maximum flow = capacity of minimum cut
// the absolute max amount of water you can push through network equals the capacity of narrowst bottleneck that cuts off source from sink
// when to apply - when you have a flow network and want to find the maximum amount of flow that can be sent from source to sink or when you want to find the minimum cut in a flow network.
// // Pattern:
// - Maximum amount that can be sent from source to sink.
// - Minimum cut / minimum edges to disconnect s and t.
// - Multiple agents/items moving through a network with capacities.
// - Assignment/matching problems (workers-jobs, students-schools, etc.).
// - Edge/vertex-disjoint paths.
// - Transform constraints into capacities on a graph.

int rowWithMax1s(vector<vector<int>> &mat) {
    int n = mat.size();
    int m = mat[0].size();

    int row = 0, col = m - 1;
    int ans = -1;

    while (row < n && col >= 0) {
        if (mat[row][col] == 1) {
            ans = row;   // Current row has more 1s
            col--;       // Check if there are more 1s to the left
        } else {
            row++;       // Move to next row
        }
    }

    return ans;
}

// bool ok;

long long dfs(int u, int p,
              vector<vector<int>>& g,
              vector<int>& val,
              long long target)
{
    long long sum = val[u];

    for (int v : g[u]) {
        if (v == p) continue;
        sum += dfs(v, u, g, val, target);
    }

    if (sum == target)
        return 0;

    if (sum > target)
        ok = false;

    return sum;
}

int maxComponents(vector<int>& val,
                  vector<vector<int>>& g)
{
    int n = val.size();

    long long total = 0;
    for (int x : val)
        total += x;

    vector<long long> divisors;

    for (long long d = 1; d * d <= total; d++) {
        if (total % d == 0) {
            divisors.push_back(d);
            if (d * d != total)
                divisors.push_back(total / d);
        }
    }

    sort(divisors.begin(), divisors.end());

    for (long long target : divisors) {
        ok = true;

        if (dfs(0, -1, g, val, target) == 0 && ok)
            return total / target - 1;
    }

    return 0;
}

class Solution {
public:
    bool helper(string &s, string &p, int i, int j, vector<vector<int>> &dp) {
        // Base cases
        if (i == s.size() && j == p.size()) return true; // Both strings are exhausted
        if (j == p.size()) return false; // Pattern is exhausted but string is not
        if (i == s.size()) { // String is exhausted
            // Check if remaining pattern only contains '*'
            for (int k = j; k < p.size(); ++k)
                if (p[k] != '*') return false;
            return true;
        }

        // If already computed
        if (dp[i][j] != -1) return dp[i][j];

        // Match conditions
        if (p[j] == s[i] || p[j] == '?') {
            // Characters match or '?' matches any single character
            dp[i][j] = helper(s, p, i + 1, j + 1, dp);
        } else if (p[j] == '*') {
            // '*' matches zero or more characters
            dp[i][j] = helper(s, p, i + 1, j, dp) || helper(s, p, i, j + 1, dp);
        } else {
            // Characters do not match
            dp[i][j] = false;
        }

        return dp[i][j];
    }

    bool isMatch(string s, string p) {
        // Memoization table
        vector<vector<int>> dp(s.size(), vector<int>(p.size(), -1));
        return helper(s, p, 0, 0, dp);
    }
};

vi nextGreaterElement(const vi& nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] < nums[i]) {
            ans[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }

    return ans;
}

// ------------------------------------------------------------
// Next Smaller Element
//
// Stack: increasing
// Pop while current < stack.top()
// ------------------------------------------------------------
vi nextSmallerElement(const vi& nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] > nums[i]) {
            ans[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }

    return ans;
}

// ------------------------------------------------------------
// Previous Greater Element
//
// Stack: decreasing
// Pop while stack.top() <= current
// ------------------------------------------------------------
vi previousGreaterElement(const vi& nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] <= nums[i])
            st.pop();

        if (!st.empty())
            ans[i] = nums[st.top()];

        st.push(i);
    }

    return ans;
}

// ------------------------------------------------------------
// Previous Smaller Element
//
// Stack: increasing
// Pop while stack.top() >= current
// ------------------------------------------------------------
vi previousSmallerElement(const vi& nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] >= nums[i])
            st.pop();

        if (!st.empty())
            ans[i] = nums[st.top()];

        st.push(i);
    }

    return ans;
}

// ------------------------------------------------------------
// Stock Span
//
// Previous Greater Element (index version)
// ------------------------------------------------------------
vi stockSpan(const vi& prices) {
    int n = prices.size();
    vi span(n);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && prices[st.top()] <= prices[i])
            st.pop();

        span[i] = st.empty() ? i + 1 : i - st.top();
        st.push(i);
    }

    return span;
}

// ------------------------------------------------------------
// Next Greater Element II (Circular)
//
// Traverse twice.
// Push indices only during first pass.
// ------------------------------------------------------------
vi nextGreaterCircular(const vi& nums) {
    int n = nums.size();
    vi ans(n, -1);
    stack<int> st;

    for (int i = 0; i < 2 * n; i++) {
        while (!st.empty() && nums[st.top()] < nums[i % n]) {
            ans[st.top()] = nums[i % n];
            st.pop();
        }

        if (i < n)
            st.push(i);
    }

    return ans;
}

ll sumSubarrayMins(const vi& arr) {
    int n = arr.size();
    stack<int> st;
    ll ans = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] >= arr[i])) {
            int mid = st.top();
            st.pop();

            int left = st.empty() ? -1 : st.top();
            int right = i;

            ll leftCnt = mid - left;
            ll rightCnt = right - mid;

            ans = (ans + 1LL * arr[mid] * leftCnt % MOD * rightCnt) % MOD;
        }
        if (i < n) st.push(i);
    }

    return ans;
}

// ------------------------------------------------------------
// LC 1793 - Maximum Score of a Good Subarray
//
// For each element, find the largest interval where it is the
// minimum. If that interval contains k, update the answer.
// ------------------------------------------------------------
int maximumScore(vector<int>& nums, int k) {
    int n = nums.size();
    stack<int> st;
    ll ans = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || nums[st.top()] >= nums[i])) {
            int mid = st.top();
            st.pop();

            int left = st.empty() ? -1 : st.top();
            int right = i;

            if (left < k && k < right)
                ans = max(ans, 1LL * nums[mid] * (right - left - 1));
        }
        if (i < n) st.push(i);
    }

    return ans;
}

// ------------------------------------------------------------
// Sum of Subarray Maximums
//
// Contribution:
// arr[i] is maximum in
// (i - previous greater) * (next greater/equal - i) subarrays.
//
// Stack:
// - decreasing
// - pop while arr[top] <= arr[i]
// ------------------------------------------------------------
ll sumSubarrayMaxs(const vi& arr) {
    int n = arr.size();
    stack<int> st;
    ll ans = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && (i == n || arr[st.top()] <= arr[i])) {
            int mid = st.top();
            st.pop();

            int left = st.empty() ? -1 : st.top();
            int right = i;

            ll leftCnt = mid - left;
            ll rightCnt = right - mid;

            ans = (ans + 1LL * arr[mid] * leftCnt % MOD * rightCnt) % MOD;
        }
        if (i < n) st.push(i);
    }

    return ans;
}

vector<int> maxOfMins(vector<int>& arr) {
        int n = arr.size();

        vector<int> ans(n + 1, INT_MIN);
        stack<int> st;
        for (int i = 0; i <= n; i++) {

            while (!st.empty() &&
                   (i == n || arr[st.top()] >= arr[i])) { 
                // Pop equals (avoid duplicates in stack) to ensure that the contribution range 
                // of equal elements is uniquely partitioned and not counted twice.
                int mid = st.top();
                st.pop();

                int left = st.empty() ? -1 : st.top();
                int right = i;

                int len = right - left - 1; // exlusive boundary
                // mid will be the minimum of the subarray from left+1 to right-1, which has length len
                ans[len] = max(ans[len], arr[mid]);
            }

            if (i < n) {
                st.push(i);
            }
        }

        // suffix maximum
        for (int len = n - 1; len >= 1; len--) {
            ans[len] = max(ans[len], ans[len + 1]);
        }

        vector<int> res;
        for (int len = 1; len <= n; len++) {
            res.push_back(ans[len]);
        }

        return res;
    }


int partition(std::vector<int>& arr, int low, int high) {
    // Pick a random index between low and high to avoid O(n^2) on sorted inputs
    int randomIndex = low + rand() % (high - low + 1);
    swap(arr[randomIndex], arr[high]);

    int pivot = arr[high];
    int i = low;

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            swap(arr[i], arr[j]);
            i++;
        }
    }
    swap(arr[i], arr[high]);
    return i; // Index where the pivot ended up
}

int quickSelect(std::vector<int>& arr, int low, int high, int k) {
    if (low == high) return arr[low];

    int pivotIndex = partition(arr, low, high);

    if (k == pivotIndex) {
        return arr[k];
    } else if (k < pivotIndex) {
        return quickSelect(arr, low, pivotIndex - 1, k);
    } else {
        return quickSelect(arr, pivotIndex + 1, high, k);
    }
}

double findMedian(std::vector<int>& arr) {
    int n = arr.size();
    if (n % 2 == 1) {
        return quickSelect(arr, 0, n - 1, n / 2);
    } else {
        int leftMid = quickSelect(arr, 0, n - 1, n / 2 - 1);
        int rightMid = quickSelect(arr, 0, n - 1, n / 2);
        return (leftMid + rightMid) / 2.0;
    }
}

vvi mexGridConstruction(int n){
    vvi grid(n, vi(n));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            grid[i][j] = (i ^ j);
        }
    }
    return grid;
}


struct BinaryLifting {
    int LOG;
    vector<vector<int>> up, info;
    vector<int> depth;

    // ================= CUSTOMIZE =================

    static constexpr int ID = 0;

    static int combine(int a, int b) {
        return a + b;

        // return max(a,b);
        // return min(a,b);
        // return a ^ b;
        // return gcd(a,b);
        // return a & b;
        // return a | b;
    }

    // =============================================

    BinaryLifting(int n, int root,
                  vector<vector<int>> &g,
                  vector<int> &val)
        : LOG(32 - __builtin_clz(n)),
          up(n + 1, vector<int>(LOG, -1)),
          info(n + 1, vector<int>(LOG, ID)),
          depth(n + 1) {

        dfs(root, -1, 0, g, val);

        for (int j = 1; j < LOG; j++) {
            for (int i = 1; i <= n; i++) {
                if (up[i][j - 1] == -1) continue;

                up[i][j] = up[up[i][j - 1]][j - 1];
                info[i][j] = combine(info[i][j - 1],
                                     info[up[i][j - 1]][j - 1]);
            }
        }
    }

    void dfs(int u, int p, int d,
             vector<vector<int>> &g,
             vector<int> &val) {

        depth[u] = d;
        up[u][0] = p;
        info[u][0] = val[u];

        for (int v : g[u])
            if (v != p)
                dfs(v, u, d + 1, g, val);
    }

    pair<int,int> lift(int u, int k) {
        int ans = ID;

        for (int j = 0; j < LOG && u != -1; j++) {
            if (k & (1 << j)) {
                ans = combine(ans, info[u][j]);
                u = up[u][j];
            }
        }

        return {u, ans};
    }

    int kthAncestor(int u, int k) {
        return lift(u, k).first;
    }

    int lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);

        a = lift(a, depth[a] - depth[b]).first;

        if (a == b) return a;

        for (int j = LOG - 1; j >= 0; j--) {
            if (up[a][j] != up[b][j]) {
                a = up[a][j];
                b = up[b][j];
            }
        }

        return up[a][0];
    }

    // aggregate from u to ancestor anc (inclusive)
    int queryUp(int u, int anc) {
        int ans = ID;
        int k = depth[u] - depth[anc];

        for (int j = 0; j < LOG; j++) {
            if (k & (1 << j)) {
                ans = combine(ans, info[u][j]);
                u = up[u][j];
            }
        }

        return combine(ans, info[u][0]);
    }

    int dist(int u, int v) {
        int w = lca(u, v);
        return depth[u] + depth[v] - 2 * depth[w];
    }
};

/*
--------------------------------------------------------------------------------
BINARY LIFTING TEMPLATE USAGE GUIDE:
--------------------------------------------------------------------------------
1. CUSTOMIZATION (Inside `struct BinaryLifting`):
   - Set `ID` (Identity value):
     * Sum / XOR : ID = 0
     * Min       : ID = INF (1e9)
     * Max       : ID = -INF (-1e9)
     * GCD       : ID = 0
   - Update `combine(a, b)` function to match your query requirement:
     `return a + b;` or `return max(a, b);` or `return gcd(a, b);`

2. INITIALIZATION:
   int n = 7, root = 1;
   vector<vector<int>> g(n + 1); // 1-indexed graph
   vector<int> val(n + 1, 0);   // val[u] = weight/value at node u (or edge weight to parent)
   
   // Add tree edges
   g[1].push_back(2); g[2].push_back(1);
   ...
   
   // Build BinaryLifting object (O(N log N) time & space)
   BinaryLifting bl(n, root, g, val);

3. COMMON API QUERIES (O(log N) per query):
   - Find LCA of u and v:
     int lcaNode = bl.lca(u, v);

   - Find Distance (number of edges) between u and v:
     int d = bl.dist(u, v);

   - Jump K steps up from node u:
     int ancK = bl.kthAncestor(u, k); // returns -1 if out of bounds

   - Query aggregate value on path from u UP to ancestor 'anc' (inclusive):
     int pathVal = bl.queryUp(u, anc);

   - Query aggregate value on full path between arbitrary nodes u and v:
     int anc = bl.lca(u, v);
     int leftPath = bl.queryUp(u, anc);  // u -> anc
     int rightPath = bl.queryUp(v, anc); // v -> anc
     int fullPathAns = bl.combine(leftPath, rightPath); 
     // Note: If combine is sum/XOR, adjust for double-counting val[anc] if both include anc.

--------------------------------------------------------------------------------
COMPLEXITIES:
- Precomputation : O(N log N) Time | O(N log N) Space
- Each Query     : O(log N) Time  | O(1) Auxiliary Space
--------------------------------------------------------------------------------
*/



// ============================================================================
// THEME 4: DIGIT DP
// ============================================================================

class DigitDP {
    string s;
    long long memo[20][2][2][100]; // [pos][started][tight][custom_state]

    long long dp(int pos, bool started, bool tight, int state) {
        if (pos == s.size()) return state; // Evaluate state here
        if (memo[pos][started][tight][state] != -1) return memo[pos][started][tight][state];

        int limit = tight ? s[pos] - '0' : 9;
        long long ans = 0;

        if (!started) ans += dp(pos + 1, false, tight && (0 == limit), state);

        for (int d = (started ? 0 : 1); d <= limit; d++) 
            ans += dp(pos + 1, true, tight && (d == limit), state + d); // Custom logic

        return memo[pos][started][tight][state] = ans;
    }

public:
    long long solve(long long n) {
        if (n < 0) return 0;
        s = to_string(n);
        memset(memo, -1, sizeof(memo));
        return dp(0, false, true, 0);
    }
};

/*
--------------------------------------------------------------------------------
DIGIT DP TIPS & TRICKS:
--------------------------------------------------------------------------------
- Range [L, R] Queries: Always query `solve(R) - solve(L - 1)`.
- The 'started' flag tracks if we have placed any non-zero digits yet (ignores leading zeros).
- The 'tight' flag restricts us to not exceed the prefix of the bound number.
- Reset the memo array inside the solve function before running the DP.
--------------------------------------------------------------------------------
*/

// ============================================================================
// THEME 5: SUM OVER SUBSETS (SOS) DP
// ============================================================================

struct SOS {
    int B = 20; // max bits
    int N = 1 << B; // max masks
    vector<int> f, sub, sup; // f means frequency of each mask, sub = sum over subsets, sup = sum over supersets

    SOS() : f(N), sub(N), sup(N) {}
    // subsets mean all masks y such that (y | x) == x
    // supersets mean all masks y such that (y & x) == x
    void add(int x) { f[x]++; } // add frequency of mask x

    // Build SOS DP
    void build() {
        sub = sup = f;
        for(auto b = 0; b < B; b++) // for each bit position
            for (int m = 0; m < N; m++)
                if (m >> b & 1) sub[m] += sub[m ^ (1 << b)]; // If bit b is set in m, add the subset without bit b
                else            sup[m] += sup[m | (1 << b)]; // If bit b is not set in m, add the superset with bit b
    }

    // (y | x) == x
    int subsets(int x) { return sub[x]; }
    // (y & x) == x
    int supersets(int x) { return sup[x]; }

    // (y & x) != 0 -> n - disjoint -> n - countSubsets((N-1)^x)
    int disjoint(int x) { return sub[(N - 1) ^ x]; } // disjoint means all masks y such that (y & x) == 0
    int intersect(int x, int n) { return n - disjoint(x); } // intersect means all masks y such that (y & x) != 0
};

/*
--------------------------------------------------------------------------------
SOS DP TIPS & TRICKS:
--------------------------------------------------------------------------------
- Use when masks are <= 20-22 bits.
- common relationships:
  - (y | x) == x  -> subset query
  - (y & x) == x  -> superset query
  - (y & x) == 0  -> disjoint = countSubsets((N-1)^x)
  - (y & x) != 0  -> intersection = N - disjoint
--------------------------------------------------------------------------------
*/


// ============================================================================
// THEME 7: BOTTOM-UP INTERVAL DP
// ============================================================================

long long solveIntervalDPBottomUp(int n) {
    vector<vector<long long>> dp(n, vector<long long>(n, 0));

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l+len<=n; l++) {
            int r = l + len - 1;

            dp[l][r] = LLONG_MAX;

            for (int k = l; k < r; k++) {

                long long cur =
                    dp[l][k] +
                    dp[k + 1][r]
                    /* + problem-specific cost(l,k,r) */;

                dp[l][r] = min(dp[l][r], cur);
            }
        }
    }

    return dp[0][n - 1];
}

/*
--------------------------------------------------------------------------------
INTERVAL DP TIPS & TRICKS:
--------------------------------------------------------------------------------
- Matrix Chain Multiplication:
  `dp[l][r] = min(dp[l][k] + dp[k+1][r] + arr[l] * arr[k+1] * arr[r+1])`
- Merge Stones / Slimes:
  `dp[l][r] = min(dp[l][k] + dp[k+1][r]) + prefixSum(l, r)`
- Burst Balloons (Choose LAST balloon k in [l, r]):
  `dp[l][r] = max(dp[l][k-1] + dp[k+1][r] + a[l-1] * a[k] * a[r+1])`
--------------------------------------------------------------------------------
*/

// ============================================================================
// THEME 8: GRAPH CYCLES
// ============================================================================

vector<int> buildCycle(int s, int e, const vector<int>& par) {
    vector<int> cyc;
    for (int u = e; u != s; u = par[u]) cyc.push_back(u);
    cyc.push_back(s);
    reverse(cyc.begin(), cyc.end());
    return cyc;
}

class UndirectedCycle {
    int n, s = -1, e = -1;
    vector<vector<int>> g;
    vector<int> vis, par;

    bool dfs(int u, int p) {
        vis[u] = 1;
        for (int v : g[u]) {
            if (v == p) continue;
            if (!vis[v]) {
                par[v] = u;
                if (dfs(v, u)) return true;
            } else {
                s = v, e = u;
                return true;
            }
        }
        return false;
    }

public:
    UndirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), par(n + 1, -1) {}

    void addEdge(int u, int v) { g[u].push_back(v), g[v].push_back(u); }

    vector<int> getCycle() {
        for (int i = 1; i <= n; i++)
            if (!vis[i] && dfs(i, -1))
                return buildCycle(s, e, par);
        return {};
    }
};

class DirectedCycle {
    int n, s = -1, e = -1;
    vector<vector<int>> g;
    vector<int> vis, inPath, par;

    bool dfs(int u) {
        vis[u] = inPath[u] = 1;
        for (int v : g[u]) {
            if (!vis[v]) {
                par[v] = u;
                if (dfs(v)) return true;
            } else if (inPath[v]) {
                s = v, e = u;
                return true;
            }
        }
        inPath[u] = 0;
        return false;
    }

public:
    DirectedCycle(int n) : n(n), g(n + 1), vis(n + 1), inPath(n + 1), par(n + 1, -1) {}

    void addEdge(int u, int v) { g[u].push_back(v); }

    vector<int> getCycle() {
        for (int i = 1; i <= n; i++)
            if (!vis[i] && dfs(i))
                return buildCycle(s, e, par);
        return {};
    }
};

struct Edge {
    int u, v;
    ll w;
};

vector<int> findNegativeCycle(int n, const vector<Edge> &edges)
{
    vector<ll> dist(n + 1, 0);
    vector<int> parent(n + 1, -1);
    int lastRelaxedNode = -1;

    for (int i = 1; i <= n; i++)
    {
        lastRelaxedNode = -1;
        for (const auto &e : edges)
        {
            if (dist[e.u] + e.w < dist[e.v])
            {
                dist[e.v] = dist[e.u] + e.w;
                parent[e.v] = e.u;
                lastRelaxedNode = e.v;
            }
        }
    }
    if (lastRelaxedNode == -1) return {};
    while(n--) lastRelaxedNode = parent[lastRelaxedNode];   
    return buildCycle(lastRelaxedNode, parent[lastRelaxedNode], parent);
}

/*
--------------------------------------------------------------------------------
GRAPH CYCLES TIPS & TRICKS:
--------------------------------------------------------------------------------
- Undirected cycle detection must skip the immediate parent node `v == p` in traversal.
- Directed cycle detection requires keeping track of the current recursion stack path `inPath` or using 3-state visited array.
- Negative Cycle: Run Bellman-Ford $N$ times. If any edge is relaxed in the $N$-th iteration, a negative cycle exists. Walk back $N$ times from the last relaxed node to ensure you are inside the cycle before reconstructing.
--------------------------------------------------------------------------------
*/

int inc_array(vector<int>& nums) {
    long long totalCost = 0;
    priority_queue<int> medianHeap;
    for(auto x:nums){
        medianHeap.push(x);
        if(medianHeap.top() > x){
            totalCost += medianHeap.top() - x;
            medianHeap.pop();
            medianHeap.push(x);
        }
    }

    return totalCost;
}

struct MaxHeap {
    vector<int> heap;
    
    int parent(int i) {
        return (i - 1) / 2;
    }

    int left(int i) {
        return 2 * i + 1;
    }

    int right(int i) {
        return 2 * i + 2;
    }

    void swapNodes(int i, int j) {
        swap(heap[i], heap[j]);
    }

    void heapifyDown(int i) {
        int largest = i;
        int l = left(i);
        int r = right(i);

        if (l < heap.size() && heap[l] > heap[largest])
            largest = l;

        if (r < heap.size() && heap[r] > heap[largest])
            largest = r;

        if (largest != i) {
            swapNodes(i, largest);
            heapifyDown(largest);
        }
    }

    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)] < heap[i]) {
            swapNodes(i, parent(i));
            i = parent(i);
        }
    }

    void insert(int val) {
        heap.push_back(val);
        heapifyUp(heap.size() - 1);
    }

    int top() {
        return heap[0];
    }

    void pop() {
        if (heap.empty()) return;

        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty())
            heapifyDown(0);
    }

    void buildHeap(vector<int>& nums) {
        heap = nums;

        for (int i = heap.size() / 2 - 1; i >= 0; i--)
            heapifyDown(i);
    }

    vector<int> heapSort() {
        vector<int> original = heap;
        vector<int> ans;

        while (!heap.empty()) {
            ans.push_back(top());
            pop();
        }

        heap = original;
        reverse(ans.begin(), ans.end());

        return ans;
    }
};

// for min heap, just reverse the comparison operators in heapifyDown and 

