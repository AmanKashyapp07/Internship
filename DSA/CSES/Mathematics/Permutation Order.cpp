// Link: https://cses.fi/problemset/task/3397

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
using vi = vector<int>;
using vll = vector<ll>;
using vvi = vector<vector<int>>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;
const ll P = 31;

vector<ll> fact(21);

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
vector<int> kthPermutation(int n, ll k) {
    k--; // convert to 0-index

    vector<int> unused;
    for (int i = 1; i <= n; i++)
        unused.push_back(i);

    vector<int> ans;

    for (int rem = n; rem >= 1; rem--) {
        ll block = fact[rem - 1];

        int idx = k / block;

        ans.push_back(unused[idx]);
        unused.erase(unused.begin() + idx);

        k %= block;
    }

    return ans;
}

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

int main() {
    fact[0] = 1;
    for (int i = 1; i <= 20; i++)
        fact[i] = fact[i - 1] * i;

    int t;
    cin >> t;

    while (t--) {
        int x;
        cin >> x;

        if (x == 1) {
            int n;
            cin >> n;

            ll k;
            cin >> k;

            vector<int> ans = kthPermutation(n, k);

            for (int i = 0; i < n; i++)
                cout << ans[i] << " ";
            cout << '\n';
        } else {
            int n;
            cin >> n;

            vector<int> p(n);
            for (int i = 0; i < n; i++)
                cin >> p[i];

            ll rank = permutationRank(p);
            cout << rank << '\n';
        }
    }

    return 0;
}