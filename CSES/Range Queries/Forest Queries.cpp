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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<vector<int>> pref(n + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= n; i++) {
        string s;
        cin >> s;

        for (int j = 1; j <= n; j++) {
            int tree = (s[j - 1] == '*');

            pref[i][j] = tree
                       + pref[i - 1][j]
                       + pref[i][j - 1]
                       - pref[i - 1][j - 1];
        }
    }

    while (q--) {
        int y1, x1, y2, x2;
        cin >> y1 >> x1 >> y2 >> x2;

        int ans = pref[y2][x2]
                - pref[y1 - 1][x2]
                - pref[y2][x1 - 1]
                + pref[y1 - 1][x1 - 1];

        cout << ans << '\n';
    }

    return 0;
}
