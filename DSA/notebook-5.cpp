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
using vvl = vector<vector<int>>;
using vvl = vector<vector<ll>>;
using vi = vector<int>;
using vl = vector<ll>;
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

ll power(ll a, ll b, ll mod) {
    ll res = 1;
    a %= mod;
    while (b > 0) {
        if (b & 1) res = (res * a) % mod;
        a = (a * a) % mod;
        b >>= 1;
    }
    return res;
}

// Multiply two k x k matrices
vvl multiply(const vvl& A, const vvl& B, int k) {
    vvl C(k, vl(k, 0));
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            for (int m = 0; m < k; ++m) {
                C[i][j] = (C[i][j] + A[i][m] * B[m][j]) % MOD;
            }
        }
    }
    return C;
}

// Compute (A^p) % MOD in O(k^3 log p)
vvl power(vvl A, long long p, int k) {
    // Initialize identity matrix
    vvl res(k, vl(k, 0));
    for (int i = 0; i < k; ++i) res[i][i] = 1;

    while (p > 0) {
        if (p & 1) res = multiply(res, A, k);
        A = multiply(A, A, k);
        p >>= 1;
    }
    return res;
} // here k is the size of the square matrix A and p is the exponent. The function returns A raised to the power p modulo MOD.

ll countPathOfLengthK(vvl& adj, int src, int dest, long long k) {
    int n = adj.size();
    vvl res = power(adj, k, n);
    return res[src][dest];
} // this function counts the number of paths of length k from src to dest in a directed graph represented by its adjacency matrix adj. The result is returned modulo MOD.

int calculateNthFiboacci(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    vvl F = {{1, 1}, {1, 0}};
    vvl res = power(F, n - 1, 2);
    return res[0][0];
} // this function calculates the nth Fibonacci number using matrix exponentiation. The result is returned modulo MOD.

long long maxSubarraySumAtMostK(vector<int>& a, int k) {
    int n = a.size();

    vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; i++)
        prefix[i + 1] = prefix[i] + a[i];

    deque<int> dq;
    dq.push_back(0);

    long long ans = LLONG_MIN;

    for (int i = 1; i <= n; i++) {

        // Remove prefixes that are too old
        while (!dq.empty() && dq.front() < i - k)
            dq.pop_front();

        ans = max(ans, prefix[i] - prefix[dq.front()]);

        // Maintain increasing prefix sums
        while (!dq.empty() && prefix[dq.back()] >= prefix[i])
            dq.pop_back();

        dq.push_back(i);
    }

    return ans;
}

vi kthPermutation(int n, int k) {
    vector<int> fact(n + 1, 1);
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i;

    vector<int> nums(n);
    iota(nums.begin(), nums.end(), 1);

    k--; // Convert to 0-based index
    vector<int> result;

    for (int i = n; i >= 1; i--) {
        int idx = k / fact[i - 1];
        result.push_back(nums[idx]);
        nums.erase(nums.begin() + idx);
        k %= fact[i - 1];
    }

    return result;

}

// ------------------------------------------------------------
// kthPermutation(n, k)
//
// Mathematical Idea:
//
// There are (n-1)! permutations beginning with every possible
// first element.
//
// Example (n = 4):
//
// 1 _ _ _   -> 3! permutations
// 2 _ _ _   -> 3! permutations
// 3 _ _ _   -> 3! permutations
// 4 _ _ _   -> 3! permutations
//
// Therefore, after converting k to 0-index:
//
// index = k / (n-1)!
//
// tells us which unused number should be placed first.
//
// After fixing that number, only the permutations inside that
// block remain, so:
//
// k %= (n-1)!
//
// Repeat the same process for the remaining numbers.
// ------------------------------------------------------------

// ------------------------------------------------------------
// permutationRank(permutation)
//
// Mathematical Idea:
//
// We count how many permutations appear before the given one.
//
// Suppose current element is x.
//
// Among the unused numbers, let
//
// smaller = number of unused numbers smaller than x.
//
// If we place any of those 'smaller' numbers here, the remaining
// positions can be arranged in:
//
// (remaining-1)!
//
// ways.
//
// Hence they contribute:
//
// smaller × (remaining-1)!
//
// permutations before the current permutation.
//
// Add this contribution, erase x from the unused numbers,
// and continue.
//
// The computed value is 0-indexed, so finally return rank + 1.
// ------------------------------------------------------------
ll permutationRank(vector<int>& p) {
    int n = p.size();

    vector<int> unused;
    for (int i = 1; i <= n; i++)
        unused.push_back(i);

    ll rank = 0;

    for (int i = 0; i < n; i++) {
        int idx = find(unused.begin(), unused.end(), p[i]) - unused.begin();

        rank += 1LL * idx * fact[n - i - 1];

        unused.erase(unused.begin() + idx);
    }

    return rank + 1;
}

vi next_permutation(vi& p) {
    int n = p.size();
    int i = n - 2;

    while (i >= 0 && p[i] >= p[i + 1]) i--;

    if (i >= 0) {
        int j = n - 1;
        while (p[j] <= p[i]) j--;
        swap(p[i], p[j]);
    }

    reverse(p.begin() + i + 1, p.end());
    return p;
}

// total number of swaps in bubble sort is equal to the number of inversions in the array. An inversion is a pair of indices (i, j) such that i < j and a[i] > a[j]. The number of inversions can be counted using a modified merge sort algorithm in O(n log n) time.
// total numbs of rounds in bubble sort is equal to max(original index - sorted index) + 1


ll getLCM(vi& nums) {
    unordered_map<int, int> maxPower;

    for (int x : nums) {
        int temp = x;

        for (int p = 2; p * p <= temp; p++) {
            if (temp % p == 0) {
                int cnt = 0;
                while (temp % p == 0) {
                    temp /= p;
                    cnt++;
                }
                maxPower[p] = max(maxPower[p], cnt);
            }
        }

        if (temp > 1)
            maxPower[temp] = max(maxPower[temp], 1);
    }

    ll lcm = 1;

    for (auto [prime, power] : maxPower)
        lcm = lcm * modPow(prime, power) % MOD;

    return lcm;
}

int permutationRounds(int n, vi& a) {
    vector<bool> vis(n + 1, false);
    vi cycleLengths;

    for (int i = 1; i <= n; i++) {
        if (vis[i]) continue;

        int cur = i;
        int len = 0;

        while (!vis[cur]) {
            vis[cur] = true;
            cur = a[cur];
            len++;
        }

        cycleLengths.push_back(len);
    }

    return getLCM(cycleLengths);
}

/*---------------------------------------------------------------
1. Minimum adjacent swaps = Number of inversions
Time: O(n log n)
----------------------------------------------------------------*/

struct Fenwick {
    int n;
    vector<int> bit;

    Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void add(int idx, int val) {
        while (idx <= n) {
            bit[idx] += val;
            idx += idx & -idx;
        }
    }

    int sum(int idx) {
        int res = 0;
        while (idx > 0) {
            res += bit[idx];
            idx -= idx & -idx;
        }
        return res;
    }
};

ll minAdjacentSwaps(vector<int> &a) {
    int n = a.size();
    Fenwick ft(n);

    ll inv = 0;

    for (int i = n - 1; i >= 0; i--) {
        inv += ft.sum(a[i] - 1);
        ft.add(a[i], 1);
    }

    return inv;
}

/*---------------------------------------------------------------
2. Minimum arbitrary swaps = n - number of cycles
Time: O(n)
----------------------------------------------------------------*/

int minAnySwaps(vector<int> &a) {
    int n = a.size();
    vector<bool> vis(n, false);

    int cycles = 0;

    for (int i = 0; i < n; i++) {
        if (vis[i]) continue;

        cycles++;

        int cur = i;
        while (!vis[cur]) {
            vis[cur] = true;
            cur = a[cur] - 1; // why -1? because the array a is 1-indexed, so we need to subtract 1 to get the correct index in the 0-indexed array.
        }
    }

    return n - cycles;
}

/*---------------------------------------------------------------
3. Minimum move-to-any-position operations = n - LIS
Time: O(n log n)
----------------------------------------------------------------*/

int minMoveAnywhere(vector<int> &a) {
    vector<int> lis;

    for (int x : a) {
        auto it = lower_bound(lis.begin(), lis.end(), x);

        if (it == lis.end())
            lis.push_back(x);
        else
            *it = x;
    }

    return (int)a.size() - (int)lis.size();
}

/*---------------------------------------------------------------
4. Minimum move-to-front operations
Time: O(n)
----------------------------------------------------------------*/

int minMoveToFront(vector<int> &a) {
    int n = a.size();

    int need = n;

    for (int i = n - 1; i >= 0; i--) {
        if (a[i] == need)
            need--;
    }

    return need;
}

bool repeatedSubstringPattern(string s) {
    int n = s.size();
    string ss = s + s; // append the string to itself
    return ss.substr(1, 2 * n - 2).find(s) != string::npos; // then check if the original string exists in the substring of the new string that excludes the first and last characters. If it does, then the original string can be formed by repeating a substring.
}

int countSubarrays(vi nums, int k){
    int sum=0;
    int cnt=1;
    for(int i=0;i<nums.size();i++){
        sum+=nums[i];
        if(sum>k){
            sum=nums[i];
            cnt++;
        }
        else if(sum==k){
            sum=0;
            cnt++;
        }
    }
    return cnt;
}

int quickSelect(vector<int>& nums, int k) {
        int l = 0, r = nums.size() - 1;

        while (true) {
            int pivot = nums[r];
            int p = l;

            for (int i = l; i < r; i++) {
                if (nums[i] < pivot)
                    swap(nums[p++], nums[i]);
            }

            swap(nums[p], nums[r]);

            if (p == k) return nums[p];
            if (p < k)
                l = p + 1;
            else
                r = p - 1;
        }
}


// Helper to generate all valid next states from the current state
vector<string> get_neighbors(const string& cur) {
    vector<string> neighbors;
    // Define transition logic here (e.g., swapping adjacent characters)
    return neighbors;
}

int bidirectional_bfs(const string& start, const string& target) {
    // 1. Edge Case: Start is already the target
    if (start == target) return 0;

    // 2. Use two sets to hold the current frontier for forward and backward search
    unordered_set<string> forward_set = {start};
    unordered_set<string> backward_set = {target};

    // 3. Global visited set to prevent loops across both directions
    unordered_set<string> visited = {start, target};

    int steps = 0;

    // 4. Continue while both frontiers have nodes to explore
    while (!forward_set.empty() && !backward_set.empty()) {
        
        // OPTIMIZATION: Always expand the smaller frontier to keep branching low
        if (forward_set.size() > backward_set.size()) {
            swap(forward_set, backward_set);
        }

        unordered_set<string> next_set;
        steps++;

        // 5. Expand all nodes at the current level
        for (const string& cur : forward_set) {
            for (const string& nxt : get_neighbors(cur)) {
                
                // INTERSECTION FOUND: The two searches met!
                if (backward_set.count(nxt)) {
                    return steps;
                }

                // If not visited yet, add to the next level
                if (!visited.count(nxt)) {
                    visited.insert(nxt);
                    next_set.insert(nxt);
                }
            }
        }

        // Move to the next level
        forward_set = move(next_set);
    }

    // Return -1 if no path exists
    return -1; 
}
