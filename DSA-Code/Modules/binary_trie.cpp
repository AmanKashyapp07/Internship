#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
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
 | 1  | Maximum XOR of Two Numbers (LC 421)         | 0-1 Bitwise Binary Trie           | O(31 * N)| O(31 * N)|
 | 2  | Max XOR With Element Query (LC 1707)        | Offline Sorted Queries + Dynamic  | O(Q logQ)| O(31 * N)|
 | 3  | Maximum Subarray XOR                        | Prefix XOR Trie Max Query         | O(31 * N)| O(31 * N)|
 | 4  | Count Pairs With XOR in Range (LC 1803)     | Range Difference: [0..R]-[0..L-1] | O(31 * N)| O(31 * N)|
 | 5  | Count Subarrays With XOR < K                | Prefix XOR + countLessThanK Trie  | O(31 * N)| O(31 * N)|
 | 6  | Max Genetic Difference Query (LC 1938)      | Tree DFS + Backtracking Trie      | O(31*(N+Q| O(31 * N)|
 | 7  | Minimum XOR Value Pair                      | Min Bit Branching Invariant       | O(31 * N)| O(31 * N)|
 | 8  | Max XOR Two Non-Overlapping Subarrays       | Left Prefix & Right Suffix Tries  | O(31 * N)| O(31 * N)|
 | 9  | K-th Smallest Element in Multiset           | Subtree Count Bit-Rank Traversal  | O(31)    | O(31 * N)|
 | 10 | Max XOR Path in Tree                        | Tree Root-Path Prefix XOR Trie    | O(31 * N)| O(31 * N)|
 | 11 | Subarray XOR Equal to K (LC 560 / GFG)      | Prefix XOR Frequency Invariant    | O(N)     | O(N)     |
 | 12 | Minimize XOR (LC 2429)                      | Bit Match & Least Significant Set | O(31)    | O(1)     |
 | 13 | Maximum XOR After Operations (LC 2317)      | Bitwise OR Property Exploitation  | O(N)     | O(1)     |
 | 14 | Minimum XOR Sum of Two Arrays (LC 1879)     | Bitmask DP / Min-Cost Match       | O(N 2^N) | O(2^N)   |
 | 15 | Persistent 0-1 Trie Range Query             | Functional Persistent Binary Trie | O(31 logN| O(31 * N)|
 ====================================================================================================
*/

// ====================================================================================================
// SECTION 1: CORE BITWISE BINARY TRIE DATA STRUCTURE
// ====================================================================================================

struct BinaryTrieNode {
    BinaryTrieNode* child[2];
    int count; // Subtree element count

    BinaryTrieNode() : count(0) {
        child[0] = nullptr;
        child[1] = nullptr;
    }
};

class BinaryTrie {
public:
    BinaryTrieNode* root;
    static const int BIT_LEN = 30; // Supports integers up to 2^30 - 1

    BinaryTrie() { root = new BinaryTrieNode(); }

    // Inserts a number x with delta frequency (+1 for insert, -1 for remove)
    void insert(int x, int delta = 1) {
        BinaryTrieNode* cur = root;
        for (int b = BIT_LEN; b >= 0; b--) {
            int bit = (x >> b) & 1;
            if (!cur->child[bit]) {
                cur->child[bit] = new BinaryTrieNode();
            }
            cur = cur->child[bit];
            cur->count += delta;
        }
    }

    // Removes one occurrence of number x
    void erase(int x) {
        insert(x, -1);
    }

    // Returns maximum XOR achievable with x: max_{v in Trie} (x ^ v)
    int maxXor(int x) const {
        BinaryTrieNode* cur = root;
        int maxXorVal = 0;
        for (int b = BIT_LEN; b >= 0; b--) {
            int bit = (x >> b) & 1;
            int opp = bit ^ 1;
            // Greedily choose opposite bit to set b-th bit in XOR result
            if (cur->child[opp] && cur->child[opp]->count > 0) {
                maxXorVal |= (1 << b);
                cur = cur->child[opp];
            } else if (cur->child[bit] && cur->child[bit]->count > 0) {
                cur = cur->child[bit];
            } else {
                break;
            }
        }
        return maxXorVal;
    }

    // Returns minimum XOR achievable with x: min_{v in Trie} (x ^ v)
    int minXor(int x) const {
        BinaryTrieNode* cur = root;
        int minXorVal = 0;
        for (int b = BIT_LEN; b >= 0; b--) {
            int bit = (x >> b) & 1;
            // Greedily choose same bit to keep b-th bit 0 in XOR result
            if (cur->child[bit] && cur->child[bit]->count > 0) {
                cur = cur->child[bit];
            } else if (cur->child[bit ^ 1] && cur->child[bit ^ 1]->count > 0) {
                minXorVal |= (1 << b);
                cur = cur->child[bit ^ 1];
            } else {
                break;
            }
        }
        return minXorVal;
    }

    // Counts number of elements v in Trie such that (x ^ v) < k
    int countLessThanK(int x, int k) const {
        BinaryTrieNode* cur = root;
        int count = 0;
        for (int b = BIT_LEN; b >= 0 && cur; b--) {
            int xb = (x >> b) & 1;
            int kb = (k >> b) & 1;

            if (kb == 1) {
                // If k has 1 at bit b, all elements in branch where (x ^ v) produces 0 are strictly < k
                if (cur->child[xb] && cur->child[xb]->count > 0) {
                    count += cur->child[xb]->count;
                }
                // To match prefix equal to k's prefix, traverse opposite branch where (x ^ v) produces 1
                cur = cur->child[xb ^ 1];
            } else {
                // If k has 0 at bit b, we must match 0 (no strictly smaller subtree available at this bit)
                cur = cur->child[xb];
            }
        }
        return count;
    }

    // Finds the K-th smallest element in the multiset (1-indexed)
    int kthSmallest(int k) const {
        BinaryTrieNode* cur = root;
        int val = 0;
        for (int b = BIT_LEN; b >= 0; b--) {
            int leftCount = (cur->child[0] ? cur->child[0]->count : 0);
            if (k <= leftCount) {
                cur = cur->child[0];
            } else {
                k -= leftCount;
                val |= (1 << b);
                cur = cur->child[1];
            }
        }
        return val;
    }
};


// ====================================================================================================
// SECTION 2: TOP INTERVIEW BINARY TRIE QUESTIONS
// ====================================================================================================

// =========================================================
// 1. MAXIMUM XOR OF TWO NUMBERS IN AN ARRAY (LEETCODE 421)
// =========================================================
int findMaximumXOR(vector<int>& nums) {
    BinaryTrie trie;
    int maxAns = 0;
    for (int x : nums) {
        trie.insert(x);
        maxAns = max(maxAns, trie.maxXor(x));
    }
    return maxAns;
}
// Interview Explanation:
// - Problem Statement: Find maximum result of `nums[i] XOR nums[j]` for any pair (LeetCode 421).
// - Approach: Insert numbers into 0-1 Bitwise Trie; for each number, greedily follow opposite bit at each step.
// - Complexity: Time: O(31 * N), Space: O(31 * N).


// =========================================================
// 2. MAXIMUM XOR WITH AN ELEMENT FROM ARRAY (LEETCODE 1707)
// =========================================================
vector<int> maximizeXor(vector<int>& nums, vector<vector<int>>& queries) {
    sort(nums.begin(), nums.end());
    int qSize = queries.size();

    // Store queries with original indices: {m_i, x_i, query_idx}
    vector<vector<int>> sortedQueries(qSize);
    for (int i = 0; i < qSize; i++) {
        sortedQueries[i] = {queries[i][1], queries[i][0], i};
    }
    sort(sortedQueries.begin(), sortedQueries.end());

    BinaryTrie trie;
    vector<int> result(qSize);
    int numIdx = 0, n = nums.size();

    for (const auto& q : sortedQueries) {
        int m = q[0], x = q[1], origIdx = q[2];
        // Greedily insert all nums <= m into Trie
        while (numIdx < n && nums[numIdx] <= m) {
            trie.insert(nums[numIdx++]);
        }
        result[origIdx] = (numIdx == 0) ? -1 : trie.maxXor(x);
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: For each query [x, m], find max(x XOR nums[i]) where nums[i] <= m (LeetCode 1707).
// - Approach: Offline Query Processing. Sort numbers and queries by threshold `m`; dynamically insert elements.
// - Complexity: Time: O(N log N + Q log Q + 31 * (N + Q)), Space: O(31 * N + Q).


// =========================================================
// 3. MAXIMUM SUBARRAY XOR (GFG / STRIVER SDE)
// =========================================================
int maxSubarrayXOR(const vector<int>& nums) {
    BinaryTrie trie;
    trie.insert(0); // Base case for prefix starting from index 0
    int prefixXor = 0, maxVal = 0;

    for (int x : nums) {
        prefixXor ^= x;
        maxVal = max(maxVal, trie.maxXor(prefixXor));
        trie.insert(prefixXor);
    }
    return maxVal;
}
// Interview Explanation:
// - Problem Statement: Find maximum XOR sum of any contiguous subarray (GFG / Striver SDE #89).
// - Approach: `XOR(arr[i..j]) = pref[j] ^ pref[i-1]`. Maintain prefix XORs in Binary Trie and query maxXor.
// - Complexity: Time: O(31 * N), Space: O(31 * N).


// =========================================================
// 4. COUNT PAIRS WITH XOR IN A RANGE (LEETCODE 1803)
// =========================================================
int countPairsLessThan(const vector<int>& nums, int k) {
    BinaryTrie trie;
    int count = 0;
    for (int x : nums) {
        count += trie.countLessThanK(x, k);
        trie.insert(x);
    }
    return count;
}

int countPairs(vector<int>& nums, int low, int high) {
    // Number of pairs in [low, high] = count(< high + 1) - count(< low)
    return countPairsLessThan(nums, high + 1) - countPairsLessThan(nums, low);
}
// Interview Explanation:
// - Problem Statement: Count pairs (i, j) with `i < j` such that `low <= (nums[i] XOR nums[j]) <= high` (LeetCode 1803).
// - Approach: Use inclusion-exclusion with `countLessThanK` trie queries.
// - Complexity: Time: O(31 * N), Space: O(31 * N).


// =========================================================
// 5. COUNT SUBARRAYS WITH XOR LESS THAN K
// =========================================================
long long countSubarraysXorLessThanK(const vector<int>& nums, int k) {
    BinaryTrie trie;
    trie.insert(0);
    int prefixXor = 0;
    long long totalSubarrays = 0;

    for (int x : nums) {
        prefixXor ^= x;
        totalSubarrays += trie.countLessThanK(prefixXor, k);
        trie.insert(prefixXor);
    }
    return totalSubarrays;
}
// Interview Explanation:
// - Problem Statement: Count contiguous subarrays whose total XOR is strictly less than k.
// - Approach: Accumulate prefix XORs; at each step query `countLessThanK(pref, k)` from the Trie.
// - Complexity: Time: O(31 * N), Space: O(31 * N).


// =========================================================
// 6. MAXIMUM GENETIC DIFFERENCE QUERY (LEETCODE 1938)
// =========================================================
class MaxGeneticDifference {
    struct Query {
        int val;
        int queryIdx;
    };

    void dfs(int u, const vector<vector<int>>& adj, const vector<vector<Query>>& nodeQueries,
             BinaryTrie& trie, vector<int>& ans) {
        trie.insert(u, 1);

        for (const auto& q : nodeQueries[u]) {
            ans[q.queryIdx] = trie.maxXor(q.val);
        }

        for (int v : adj[u]) {
            dfs(v, adj, nodeQueries, trie, ans);
        }

        trie.insert(u, -1); // Backtrack: remove node u when unwinding recursion
    }

public:
    vector<int> maxGeneticDifference(vector<int>& parents, vector<vector<int>>& queries) {
        int n = parents.size(), qCount = queries.size();
        vector<vector<int>> adj(n);
        int root = -1;

        for (int i = 0; i < n; i++) {
            if (parents[i] == -1) root = i;
            else adj[parents[i]].push_back(i);
        }

        vector<vector<Query>> nodeQueries(n);
        for (int i = 0; i < qCount; i++) {
            int node = queries[i][0], val = queries[i][1];
            nodeQueries[node].push_back({val, i});
        }

        vector<int> ans(qCount);
        BinaryTrie trie;
        dfs(root, adj, nodeQueries, trie, ans);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find max(val XOR node) for ancestors of node on tree (LeetCode 1938).
// - Approach: Tree DFS traversal with Backtracking Binary Trie (insert on enter, erase on exit).
// - Complexity: Time: O(31 * (N + Q)), Space: O(31 * N + Q).


// =========================================================
// 7. MINIMUM XOR VALUE PAIR (INTERVIEWBIT / LEETCODE 535)
// =========================================================
int findMinimumXOR(vector<int>& nums) {
    BinaryTrie trie;
    int minAns = INT_MAX;
    for (int x : nums) {
        if (trie.root->count > 0) {
            minAns = min(minAns, trie.minXor(x));
        }
        trie.insert(x);
    }
    return minAns;
}
// Interview Explanation:
// - Problem Statement: Find minimum XOR value pair in an array.
// - Approach: Query `minXor` in 0-1 Trie (greedily following same bit) or sort adjacent XOR.
// - Complexity: Time: O(31 * N), Space: O(31 * N).


// =========================================================
// 8. MAX XOR OF TWO NON-OVERLAPPING SUBARRAYS
// =========================================================
int maxTwoNonOverlappingSubarraysXOR(const vector<int>& nums) {
    int n = nums.size();
    if (n < 2) return 0;

    vector<int> leftMax(n, 0), rightMax(n, 0);

    // Left prefix maximum subarray XOR
    BinaryTrie leftTrie;
    leftTrie.insert(0);
    int pref = 0, currentBest = 0;
    for (int i = 0; i < n; i++) {
        pref ^= nums[i];
        currentBest = max(currentBest, leftTrie.maxXor(pref));
        leftMax[i] = currentBest;
        leftTrie.insert(pref);
    }

    // Right suffix maximum subarray XOR
    BinaryTrie rightTrie;
    rightTrie.insert(0);
    int suff = 0;
    currentBest = 0;
    for (int i = n - 1; i >= 0; i--) {
        suff ^= nums[i];
        currentBest = max(currentBest, rightTrie.maxXor(suff));
        rightMax[i] = currentBest;
        rightTrie.insert(suff);
    }

    int overallMax = 0;
    for (int i = 0; i < n - 1; i++) {
        overallMax = max(overallMax, leftMax[i] + rightMax[i + 1]);
    }
    return overallMax;
}
// Interview Explanation:
// - Problem Statement: Find maximum sum of XORs of two non-overlapping subarrays.
// - Approach: 2-Pass Prefix & Suffix Dynamic Programming with 0-1 Binary Trie.
// - Complexity: Time: O(31 * N), Space: O(31 * N).


// =========================================================
// 9. K-TH SMALLEST ELEMENT IN MULTISET VIA BINARY TRIE
// =========================================================
int findKthSmallest(const vector<int>& nums, int k) {
    BinaryTrie trie;
    for (int x : nums) trie.insert(x);
    return trie.kthSmallest(k);
}
// Interview Explanation:
// - Problem Statement: Find k-th smallest element in dynamic multiset with duplicate support.
// - Approach: Binary Trie subtree rank descent (equivalent to dynamic order-statistic tree).
// - Complexity: Insert: O(31), Query: O(31), Space: O(31 * N).


// =========================================================
// 10. MAXIMUM XOR PATH IN A WEIGHTED TREE
// =========================================================
class TreeMaxXorPath {
    void dfs(int u, int p, int runningXor, const vector<vector<pair<int, int>>>& adj, vector<int>& rootPathXor) {
        rootPathXor[u] = runningXor;
        for (const auto& edge : adj[u]) {
            int v = edge.first, wt = edge.second;
            if (v != p) {
                dfs(v, u, runningXor ^ wt, adj, rootPathXor);
            }
        }
    }

public:
    int maxXorPath(int n, const vector<vector<pair<int, int>>>& adj) {
        vector<int> rootPathXor(n, 0);
        dfs(0, -1, 0, adj, rootPathXor);

        BinaryTrie trie;
        int maxPathXor = 0;
        for (int i = 0; i < n; i++) {
            trie.insert(rootPathXor[i]);
            maxPathXor = max(maxPathXor, trie.maxXor(rootPathXor[i]));
        }
        return maxPathXor;
    }
};
// Interview Explanation:
// - Problem Statement: Find maximum XOR path between any two nodes in an edge-weighted tree.
// - Approach: Path XOR between u and v equals `(rootTo(u) ^ rootTo(v))`. Insert all root path XORs into Binary Trie.
// - Complexity: Time: O(31 * N), Space: O(31 * N).


// =========================================================
// 11. SUBARRAY XOR EQUAL TO K (LEETCODE 560 VARIANT / GFG)
// =========================================================
int subarraysWithXorK(const vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    freq[0] = 1;
    int pref = 0, count = 0;

    for (int x : nums) {
        pref ^= x;
        int target = pref ^ k;
        if (freq.count(target)) {
            count += freq[target];
        }
        freq[pref]++;
    }
    return count;
}
// Interview Explanation:
// - Problem Statement: Count total subarrays having XOR sum exactly equal to k (GFG / Striver SDE #24).
// - Approach: `pref[j] ^ pref[i-1] = k <=> pref[i-1] = pref[j] ^ k`. Track frequencies in hash map.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 12. MINIMIZE XOR (LEETCODE 2429)
// =========================================================
int minimizeXor(int num1, int num2) {
    int targetBits = __builtin_popcount(num2);
    int currentBits = __builtin_popcount(num1);
    int ans = num1;

    // Remove set bits from least significant positions if currentBits > targetBits
    for (int b = 0; b <= 30 && currentBits > targetBits; b++) {
        if ((ans >> b) & 1) {
            ans &= ~(1 << b);
            currentBits--;
        }
    }

    // Add set bits to least significant zero positions if currentBits < targetBits
    for (int b = 0; b <= 30 && currentBits < targetBits; b++) {
        if (!((ans >> b) & 1)) {
            ans |= (1 << b);
            currentBits++;
        }
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find integer x having same number of set bits as num2 such that `x XOR num1` is minimal (LeetCode 2429).
// - Approach: Match high bits of num1 first; fill or unset remaining bits at least significant positions.
// - Complexity: Time: O(31), Space: O(1).


// =========================================================
// 13. MAXIMUM XOR AFTER OPERATIONS (LEETCODE 2317)
// =========================================================
int maximumXOR(vector<int>& nums) {
    int ans = 0;
    for (int x : nums) ans |= x;
    return ans;
}
// Interview Explanation:
// - Problem Statement: Apply operation `nums[i] = nums[i] AND (nums[i] XOR x)` any times to maximize total XOR (LeetCode 2317).
// - Approach: Operation allows arbitrarily turning any set bit to 0 in any element. Thus, any bit set in ANY number can be set in the final XOR.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 14. MINIMUM XOR SUM OF TWO ARRAYS (LEETCODE 1879)
// =========================================================
int minimumXORSum(vector<int>& nums1, vector<int>& nums2) {
    int n = nums1.size();
    int totalMasks = 1 << n;
    vector<int> dp(totalMasks, 1e9);
    dp[0] = 0;

    for (int mask = 0; mask < totalMasks; mask++) {
        int i = __builtin_popcount(mask);
        if (i >= n) continue;

        for (int j = 0; j < n; j++) {
            if (!(mask & (1 << j))) {
                int nextMask = mask | (1 << j);
                dp[nextMask] = min(dp[nextMask], dp[mask] + (nums1[i] ^ nums2[j]));
            }
        }
    }
    return dp[totalMasks - 1];
}
// Interview Explanation:
// - Problem Statement: Rearrange nums2 to minimize sum of `nums1[i] XOR nums2[i]` (LeetCode 1879).
// - Approach: Bitmask DP where mask represents subset of elements chosen from nums2 for prefix of nums1.
// - Complexity: Time: O(N * 2^N), Space: O(2^N).


// =========================================================
// 15. PERSISTENT 0-1 TRIE RANGE QUERY (RANGE [L, R] MAX XOR)
// =========================================================
class PersistentBinaryTrie {
    struct PNode {
        int child[2] = {};
        int count = 0;
    };

    vector<PNode> tree;
    vector<int> versions;

public:
    PersistentBinaryTrie() {
        tree.push_back(PNode()); // Root version 0
        versions.push_back(0);
    }

    int insert(int prevRoot, int val) {
        int newRoot = tree.size();
        tree.push_back(tree[prevRoot]);
        int cur = newRoot;

        for (int b = 30; b >= 0; b--) {
            int bit = (val >> b) & 1;
            int nextNode = tree.size();
            tree.push_back(tree[tree[cur].child[bit]]);
            tree[cur].child[bit] = nextNode;
            cur = nextNode;
            tree[cur].count++;
        }
        return newRoot;
    }

    int queryRangeMaxXor(int lRoot, int rRoot, int x) {
        int leftNode = lRoot, rightNode = rRoot;
        int maxVal = 0;

        for (int b = 30; b >= 0; b--) {
            int bit = (x >> b) & 1;
            int opp = bit ^ 1;
            int oppCount = tree[tree[rightNode].child[opp]].count - tree[tree[leftNode].child[opp]].count;

            if (oppCount > 0) {
                maxVal |= (1 << b);
                leftNode = tree[leftNode].child[opp];
                rightNode = tree[rightNode].child[opp];
            } else {
                leftNode = tree[leftNode].child[bit];
                rightNode = tree[rightNode].child[bit];
            }
        }
        return maxVal;
    }
};
// Interview Explanation:
// - Problem Statement: Find max(x XOR nums[i]) where `L <= i <= R` (Range Maximum XOR Query).
// - Approach: Functional Persistent 0-1 Trie. Version roots store historical prefixes; `count(R) - count(L-1)` validates subtrees in range [L, R].
// - Complexity: Insert: O(31), Range Query: O(31), Space: O(31 * N).

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return 0;
}