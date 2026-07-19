// Link: https://cses.fi/problemset/task/3405

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    ll x, a, b, c;
    cin >> x >> a >> b >> c;

    vector<ll> arr(n);
    arr[0] = x;
    for (int i = 1; i < n; i++)
        arr[i] = (a * arr[i - 1] + b) % c;

    ll ans = 0;

    for (int bit = 0; bit < 32; bit++) {
        int cnt = 0;

        // first window
        for (int i = 0; i < k; i++)
            cnt += (arr[i] >> bit) & 1;

        if (cnt)
            ans ^= (1LL << bit);

        // remaining windows
        for (int i = k; i < n; i++) {
            cnt -= (arr[i - k] >> bit) & 1;
            cnt += (arr[i] >> bit) & 1;

            if (cnt)
                ans ^= (1LL << bit);
        }
    }

    cout << ans << '\n';
}