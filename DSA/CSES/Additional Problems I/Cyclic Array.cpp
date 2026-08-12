// Link: https://cses.fi/problemset/task/1191

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    ll k;
    cin >> n >> k;

    vector<ll> a(2 * n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        a[i + n] = a[i];
    }

    // next[i] = first index after the largest valid segment starting at i
    vector<int> nxt(2 * n + 1);

    ll sum = 0;
    int r = 0;

    for (int l = 0; l < 2 * n; l++) {
        while (r < 2 * n && sum + a[r] <= k) {
            sum += a[r];
            r++;
        }
        nxt[l] = r;
        sum -= a[l];
    }
    nxt[2 * n] = 2 * n;

    const int LOG = 21;

    vector<vector<int>> up(LOG, vector<int>(2 * n + 1));

    for (int i = 0; i <= 2 * n; i++)
        up[0][i] = nxt[i];

    for (int j = 1; j < LOG; j++) {
        for (int i = 0; i <= 2 * n; i++) {
            up[j][i] = up[j - 1][up[j - 1][i]];
        }
    }

    int ans = INT_MAX;

    for (int start = 0; start < n; start++) {
        int cur = start;
        int cnt = 0;

        for (int j = LOG - 1; j >= 0; j--) {
            if (up[j][cur] < start + n) {
                cur = up[j][cur];
                cnt += (1 << j);
            }
        }

        ans = min(ans, cnt + 1);
    }

    cout << ans << '\n';
    return 0;
}