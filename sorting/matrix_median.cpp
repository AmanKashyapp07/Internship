/**
 * CSES Problem Set
 * 
 * Problem: Median in a Row Wise Sorted Matrix
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

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int m, n;
    cin >> m >> n;

    vector<vector<int>> mat(m, vector<int>(n));

    int low = INF;
    int high = -INF;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> mat[i][j];
        }

        low = min(low, mat[i][0]);
        high = max(high, mat[i][n - 1]);
    }

    int required = (m * n + 1) / 2;
    int ans = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;

        int cnt = 0;

        for (int i = 0; i < m; i++) {
            cnt += upper_bound(all(mat[i]), mid) - mat[i].begin();
        }

        if(cnt==required) {
            ans = mid;
            high = mid - 1; 
        } else if (cnt < required) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    cout << ans << '\n';

    return 0;
}