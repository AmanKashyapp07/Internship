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