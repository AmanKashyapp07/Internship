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
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
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

#define int long long
#define input(v) for (auto & x: v) cin >> x
#define srt(v) sort((v).begin(), (v).end())
#define rev(v) reverse((v).begin(), (v).end())
#define maxx(v)( * max_element((v).begin(), (v).end()))
#define minn(v)( * min_element((v).begin(), (v).end()))
using namespace std;
#define MOD 1000000007
const int MAXN = 1e6 + 5;

void prt(vector < int > & v) {
    for (auto c: v) cout << c << " ";
    cout << "\n";
}

// ---------------------------
// Aman Kashyap - Forward Top-Down DP (0-indexed)
// ---------------------------

int helper(vector < pair < pair < int, int > , int >> & ish, vector < int > & dp, int idx, int n) {
    if (idx == n) return 0;
    if (dp[idx] != -1) return dp[idx];

    // Option 1: Skip the current project and move to the next one
    int exclude_project = helper(ish, dp, idx + 1, n);
    auto [L, R] = ish[idx].first;
    int val = ish[idx].second;
    auto it = upper_bound(ish.begin() + idx + 1, ish.end(), R, [](int value, const auto & element) {
        return value < element.first.first;
    });

    int next_idx = distance(ish.begin(), it);
    int include_project = val + helper(ish, dp, next_idx, n);
    return dp[idx] = max(exclude_project, include_project);
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    //cin >> t;
    while (t--) {
        int n;
        cin >> n;

        vector < pair < pair < int, int > , int >> ish(n);

        for (int i = 0; i < n; i++) {
            int L, R, val;
            cin >> L >> R >> val;

            ish[i] = {
                {
                    L,
                    R
                },
                val
            };
        }

        // Sort by starting times (L) ascending, and R ascending if L is the same
        sort(ish.begin(), ish.end(), [](const auto & a, const auto & b) {
            if (a.first.first != b.first.first) return a.first.first < b.first.first; // Sort by L ascending
            return a.first.second > b.first.second; // If L is the same, sort by R ascending
        });
        
        // Initialize DP table with -1
        vector < int > dp(n, -1);
        
        // Start the recursion from the 0-th index
        cout << helper(ish, dp, 0, n) << "\n";
    }

    return 0;
}
