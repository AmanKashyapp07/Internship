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

// ============================================================
// HELPER UTILITIES
// ============================================================

ll power(ll a, ll b, ll mod = MOD) {
    ll res = 1; a %= mod;
    while (b > 0) {
        if (b & 1) res = (res * a) % mod;
        a = (a * a) % mod;
        b >>= 1;
    }
    return res;
}

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | Max Subarray Sum of Length at Most K        | Monotonic Deque on Prefix Sums    | O(N)     | O(N)     |
 | 2  | Lexicographical Rank of a Permutation       | Factorial Positional Weighting    | O(N^2)   | O(N)     |
 | 3  | LCM of Array Elements Modulo MOD            | Prime Factorization + Max Powers  | O(NsqrtV)| O(U)     |
 | 4  | Permutation Rounds (LCM of Cycles)          | Disjoint Cycle Decomp + LCM       | O(N+sqrt)| O(N)     |
 | 5  | Min Move-to-Anywhere Operations             | Longest Increasing Subseq (N-LIS) | O(N logN)| O(N)     |
 | 6  | Min Move-to-Front Operations                | Greedy Backwards Suffix Matching  | O(N)     | O(1)     |
 | 7  | Min Subarrays with Sum <= K                 | Greedy Running Sum Partitioning   | O(N)     | O(1)     |
 | 8  | Bidirectional BFS Template                  | Two-Ended BFS (Smaller Frontier)  | O(B^(D/2)| O(B^(D/2)|
 ====================================================================================================
*/

// ============================================================
// 1. MAX SUBARRAY SUM OF LENGTH AT MOST K
// ============================================================

ll maxSubarraySumAtMostK(const vi &a, int k) {
    int n = a.size();
    vl pref(n + 1, 0);
    for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + a[i];
    deque<int> dq = {0};
    ll ans = -1e18;
    for (int i = 1; i <= n; i++) {
        while (!dq.empty() && dq.front() < i - k) dq.pop_front();
        ans = max(ans, pref[i] - pref[dq.front()]);
        while (!dq.empty() && pref[dq.back()] >= pref[i]) dq.pop_back();
        dq.push_back(i);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Find the maximum subarray sum of length at most K (CSES Maximum Subarray Sum II).
// - Approach: Monotonic Deque over prefix sums (Sliding Window Minimum).
// - Intuition: Max subarray sum ending at i is pref[i] - min(pref[j]) for i - k <= j < i; deque maintains candidate minimum prefix sums.
// - Complexity: Time: O(N) each index pushed/popped at most once, Space: O(N) for prefix array and deque.


// ============================================================
// 2. LEXICOGRAPHICAL RANK OF A PERMUTATION
// ============================================================

ll permutationRank(const vi &p) {
    int n = p.size();
    vl fact(n + 1, 1);
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i;
    vi unused(n);
    iota(unused.begin(), unused.end(), 1);
    ll rank = 0;
    for (int i = 0; i < n; i++) {
        int idx = find(unused.begin(), unused.end(), p[i]) - unused.begin();
        rank += idx * fact[n - i - 1];
        unused.erase(unused.begin() + idx);
    }
    return rank + 1;
}
// Interview Explanation:
// - Problem Statement: Find the 1-based lexicographical rank of a given permutation.
// - Approach: Factorial Positional Weighting (Lexicographical Rank).
// - Intuition: For each position i, count remaining unused elements strictly smaller than p[i] and multiply by (N - 1 - i)!.
// - Complexity: Time: O(N^2) using vector search and erase, Space: O(N).


// ============================================================
// 3. LCM OF ARRAY ELEMENTS MODULO MOD
// ============================================================

ll getLCM(const vi &nums) {
    unordered_map<int, int> max_pwr;
    for (int x : nums) {
        int temp = x;
        for (int p = 2; p * p <= temp; p++) {
            if (temp % p == 0) {
                int cnt = 0;
                while (temp % p == 0) { temp /= p; cnt++; }
                max_pwr[p] = max(max_pwr[p], cnt);
            }
        }
        if (temp > 1) max_pwr[temp] = max(max_pwr[temp], 1);
    }
    ll lcm = 1;
    for (auto [prime, pwr] : max_pwr) lcm = (lcm * power(prime, pwr, MOD)) % MOD;
    return lcm;
}
// Interview Explanation:
// - Problem Statement: Compute the LCM of an array of numbers modulo MOD via prime factorization.
// - Approach: Prime Factorization with Maximum Exponent Tracking.
// - Intuition: LCM equals the product of each prime factor raised to its maximum multiplicity across all numbers.
// - Complexity: Time: O(N \sqrt{\max(A)}), Space: O(U) distinct prime factors.


// ============================================================
// 4. PERMUTATION ROUNDS (LCM OF CYCLES)
// ============================================================

int permutationRounds(int n, const vi &a) {
    vector<bool> vis(n + 1, false);
    vi cycle_len;
    for (int i = 1; i <= n; i++) {
        if (vis[i]) continue;
        int cur = i, len = 0;
        while (!vis[cur]) {
            vis[cur] = true;
            cur = a[cur];
            len++;
        }
        cycle_len.push_back(len);
    }
    return getLCM(cycle_len);
}
// Interview Explanation:
// - Problem Statement: Find minimum rounds for a permutation to return to original order (LCM of cycle lengths - CSES 3398).
// - Approach: Disjoint Cycle Decomposition + LCM of Cycle Lengths.
// - Intuition: Any permutation decomposes into disjoint cycles; full array aligns at LCM of cycle lengths.
// - Complexity: Time: O(N + \sum \sqrt{\text{len}}), Space: O(N).


// ============================================================
// 5. MIN MOVE-TO-ANYWHERE OPERATIONS
// ============================================================

int minMoveAnywhere(const vi &a) {
    vi lis;
    for (int x : a) {
        auto it = lower_bound(lis.begin(), lis.end(), x);
        if (it == lis.end()) lis.push_back(x);
        else *it = x;
    }
    return (int)a.size() - (int)lis.size();
}
// Interview Explanation:
// - Problem Statement: Find the minimum move-to-anywhere operations needed to sort an array.
// - Approach: Longest Increasing Subsequence (LIS) via Patience Sorting.
// - Intuition: Elements in LIS stay in place; minimum moves required is N - LIS.
// - Complexity: Time: O(N \log N), Space: O(N).


// ============================================================
// 6. MIN MOVE-TO-FRONT OPERATIONS
// ============================================================

int minMoveToFront(const vi &a) {
    int n = a.size(), need = n;
    for (int i = n - 1; i >= 0; i--) if (a[i] == need) need--;
    return need;
}
// Interview Explanation:
// - Problem Statement: Find the minimum move-to-front operations needed to sort a permutation of size N.
// - Approach: Greedy Backwards Suffix Matching from N down to 1.
// - Intuition: Scan right to left tracking expected value (N, N-1, ...); suffix elements never move.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================
// 7. MIN SUBARRAYS WITH SUM <= K
// ============================================================

int countSubarrays(const vi &nums, int k) {
    int sum = 0, cnt = 1;
    for (int x : nums) {
        sum += x;
        if (sum > k) { sum = x; cnt++; }
        else if (sum == k) { sum = 0; cnt++; }
    }
    return cnt;
}
// Interview Explanation:
// - Problem Statement: Find the minimum continuous subarrays needed with sum not exceeding K.
// - Approach: Greedy Running Sum Partitioning.
// - Intuition: Greedily extend subarray while sum <= K; split when sum exceeds K.
// - Complexity: Time: O(N), Space: O(1).


// ============================================================
// 8. BIDIRECTIONAL BFS TEMPLATE
// ============================================================

vector<string> getNeighbors(const string &cur) { return {}; }

int bidirectionalBFS(const string &start, const string &target) {
    if (start == target) return 0;
    unordered_set<string> fwd = {start}, bwd = {target}, vis = {start, target};
    int steps = 0;
    while (!fwd.empty() && !bwd.empty()) {
        if (fwd.size() > bwd.size()) swap(fwd, bwd);
        unordered_set<string> nxt_set;
        steps++;
        for (const string &cur : fwd) {
            for (const string &nxt : getNeighbors(cur)) {
                if (bwd.count(nxt)) return steps;
                if (!vis.count(nxt)) {
                    vis.insert(nxt);
                    nxt_set.insert(nxt);
                }
            }
        }
        fwd = std::move(nxt_set);
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find the shortest transformation path between two strings using Bidirectional BFS.
// - Approach: Bidirectional Breadth-First Search (Two-Ended BFS).
// - Intuition: Expand simultaneously from start and target; always expand the smaller frontier set to reduce search space to O(B^(D/2)).
// - Complexity: Time: O(B^(D/2)), Space: O(B^(D/2)).
