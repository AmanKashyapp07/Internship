// Link: https://cses.fi/problemset/task/1146

#include <bits/stdc++.h>
using namespace std;

int main() {
    long long n;
    cin >> n;

    long long ans = 0;

    for (int k = 0; k < 60; k++) {
        long long h = 1LL << k;
        long long b = h << 1;

        long long full_blocks = (n + 1) / b;
        ans += full_blocks * h;

        long long rem = (n + 1) % b;
        ans += max(0LL, rem - h);
    }

    cout << ans << '\n';
}