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

    // (n-1)!
    int fact = 1;
    for (int i = 1; i < n; i++)
        fact *= i;

    // Available numbers
    for (int i = 1; i <= n; i++)
        nums.pb(i);

    k--; // convert to 0-based indexing

    string ans;

    while (!nums.empty()) {
        int idx = k / fact;

        // Pick outer number
        ans += to_string(nums[idx]);

        nums.erase(nums.begin() + idx); // takes o(n)

        if (nums.empty())
            break;

        // Move inside selected block
        k %= fact; // this is crucial, if k=16 and fact=6, then remaining k will be 16%6=4, so we need to move inside the selected block, so that we can find the next number in the permutation sequence, if we don't do this, then we will always be in the same block and will not be able to find the next number in the permutation sequence

        // Next block size = (remaining-1)!
        fact /= nums.size(); // earlier it was (n-1)! but now we have one less number, so it becomes (n-2)! and so on, so we divide by the size of the remaining numbers to get the new block size, or we can use count of remaining numbers to get the new block size, as we are removing one number from the available numbers, so the size of the remaining numbers decreases by 1, so we divide by the size of the remaining numbers to get the new block size
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
