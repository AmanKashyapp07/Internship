/**
 * LeetCode 60 - Permutation Sequence
 *
 * Description:
 * Given n and k, return the kth permutation sequence of numbers from 1 to n.
 *
 * Approach:
 * - Mathematical approach using the Factorial Number System.
 * - With n elements, there are n! total permutations. The first digit has blocks of size (n-1)!.
 * - Determine the index of the digit to pick: `idx = k / fact`.
 * - Add `nums[idx]` to the output, erase it from the available list, and update k: `k %= fact`.
 * - Reduce block size: `fact /= remaining_count`.
 *
 * Time Complexity: O(N^2) due to vector element erasure in each step.
 * Space Complexity: O(N) to store the list of available numbers.
 */

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

using namespace std;
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;

#define all(x)   (x).begin(), (x).end()
#define rall(x)  (x).rbegin(), (x).rend()
#define pb       push_back
#define ff       first
#define ss       second

const int INF  = INT_MAX;
const ll  LINF = LLONG_MAX;
const ll  MOD  = 1e9 + 7;

// ─────────────────────────────────────────────────────────────────────────────

string kthPermutation(int n, int k) {
    vector<int> nums;
    int fact = 1;
    for (int i = 1; i <= n; i++) {
        nums.push_back(i);
        if (i < n) fact *= i;   // (n-1)!
    }
    k--;
    string ans;
    while (!nums.empty()) {
        int pos = k / fact;
        ans += to_string(nums[pos]);
        nums.erase(nums.begin() + pos); // erase number at idx pos (0-based)
        if (nums.empty()) break;
        k %= fact;
        fact /= nums.size();
    }

    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    cin >> n >> k;

    cout << kthPermutation(n, k) << '\n';

    return 0;
}
