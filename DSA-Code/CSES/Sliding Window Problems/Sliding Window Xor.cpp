// Link: https://cses.fi/problemset/task/3426

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, k;
    cin >> n >> k;

    long long x, a, b, c;
    cin >> x >> a >> b >> c;

    deque<long long> window;

    long long curXor = 0;

    // Build first window
    long long cur = x;
    for (long long i = 1; i <= k; i++) {
        window.push_back(cur);
        curXor ^= cur;

        if (i < n) {
            cur = (a * cur + b) % c;
        }
    }

    long long answer = curXor;

    // Process remaining elements
    for (long long i = k + 1; i <= n; i++) {
        long long nextVal = cur;

        curXor ^= window.front(); // remove
        window.pop_front();

        curXor ^= nextVal;        // add
        window.push_back(nextVal);

        answer ^= curXor;

        if (i < n) {
            cur = (a * cur + b) % c;
        }
    }

    cout << answer << '\n';
    return 0;
}