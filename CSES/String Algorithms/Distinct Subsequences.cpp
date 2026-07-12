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

int main() {
    // Optimize standard I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;

    int n = s.length();
    long long MOD = 1e9 + 7;

    // dp[i] stores the number of distinct subsequences using the first i characters
    vector<long long> dp(n + 1, 0);
    dp[0] = 1; // Base case: the empty string ""

    // last[ch] stores the 1-based index of the last occurrence of character ch
    vector<int> last(26, 0);

    for (int i = 1; i <= n; i++) {
        char ch = s[i - 1];
        int ch_idx = ch - 'a';

        // Rule 1: Double the number of current choices
        dp[i] = (2 * dp[i - 1]) % MOD;

        // Rule 2: If seen before, subtract the duplicates
        if (last[ch_idx] != 0) {
            int prev_idx = last[ch_idx];
            dp[i] = (dp[i] - dp[prev_idx - 1] + MOD) % MOD;
        }

        // Record the current 1-based index as the most recent sighting
        last[ch_idx] = i;
    }

    // Subtract 1 to exclude the empty string from the final generated count
    long long ans = (dp[n] - 1 + MOD) % MOD;
    
    cout << ans << "\n";

    return 0;
}
