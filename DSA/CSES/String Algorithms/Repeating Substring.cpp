// Link: https://cses.fi/problemset/task/2106

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MOD1 = 1000000007;
const ll MOD2 = 1000000009;
const ll P = 911382323;

struct FastHash {
    vector<ll> p1, p2, h1, h2;

    FastHash(string s) {
        int n = s.size();
        p1.assign(n + 1, 1);
        p2.assign(n + 1, 1);
        h1.assign(n + 1, 0);
        h2.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            p1[i + 1] = p1[i] * P % MOD1;
            p2[i + 1] = p2[i] * P % MOD2;

            h1[i + 1] = (h1[i] * P + (s[i] - 'a' + 1)) % MOD1;
            h2[i + 1] = (h2[i] * P + (s[i] - 'a' + 1)) % MOD2;
        }
    }

    pair<ll,ll> get(int l, int r) {
        ll x1 = (h1[r + 1] - h1[l] * p1[r - l + 1] % MOD1 + MOD1) % MOD1;
        ll x2 = (h2[r + 1] - h2[l] * p2[r - l + 1] % MOD2 + MOD2) % MOD2;
        return {x1, x2};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int n = s.size();
    FastHash H(s);

    auto check = [&](int len) {
        map<pair<ll,ll>, int> mp;

        for (int i = 0; i + len <= n; i++) {
            auto h = H.get(i, i + len - 1);

            if (mp.count(h))
                return i;

            mp[h] = i;
        }

        return -1;
    };

    int lo = 1, hi = n;
    int bestLen = 0, bestPos = -1;

    while (lo <= hi) {
        int mid = (lo + hi) / 2;

        int pos = check(mid);

        if (pos != -1) {
            bestLen = mid;
            bestPos = pos;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    if (bestLen == 0)
        cout << -1 << '\n';
    else
        cout << s.substr(bestPos, bestLen) << '\n';
}