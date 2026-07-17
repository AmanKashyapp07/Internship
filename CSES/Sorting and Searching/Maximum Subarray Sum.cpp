// Link: https://cses.fi/problemset/task/1643

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;

    long long x;
    cin >> x;

    long long cur = x;
    long long ans = x;

    for (int i = 1; i < n; i++) {
        cin >> x;
        cur = max(x, cur + x);
        ans = max(ans, cur);
    }

    cout << ans << '\n';
    return 0;
}