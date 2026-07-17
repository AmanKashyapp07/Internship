// Link: https://cses.fi/problemset/task/2422

#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int64 n;
    cin >> n;

    int64 k = (n * n + 1) / 2;

    int64 lo = 1, hi = n * n;

    while (lo < hi) {
        int64 mid = lo + (hi - lo) / 2;

        int64 cnt = 0;
        for (int64 i = 1; i <= n; i++) {
            cnt += min(n, mid / i);
        }

        if (cnt >= k)
            hi = mid;
        else
            lo = mid + 1;
    }

    cout << lo << '\n';
    return 0;
}