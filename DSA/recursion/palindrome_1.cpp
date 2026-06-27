
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

vector<vector<bool>> pal;
vector<vector<string>> ans;
vector<string> cur;
string s;
int n;

void dfs(int idx) {
    if (idx == n) {
        ans.pb(cur);
        return;
    }

    for (int j = idx; j < n; j++) {
        if (!pal[idx][j]) continue;

        cur.pb(s.substr(idx, j - idx + 1));
        dfs(j + 1);
        cur.pop_back();
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> s;
    n = s.size();

    pal.assign(n, vector<bool>(n, false));

    // length = 1
    for (int i = 0; i < n; i++)
        pal[i][i] = true;

    // length >= 2
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;

            if (s[i] != s[j]) continue;

            if (len <= 3)
                pal[i][j] = true;
            else
                pal[i][j] = pal[i + 1][j - 1];
        }
    }

    dfs(0);

    for (auto &partition : ans) {
        cout << "[ ";
        for (auto &part : partition)
            cout << part << ' ';
        cout << "]\n";
    }

    return 0;
}