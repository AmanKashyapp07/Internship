// Link: https://cses.fi/problemset/task/2137

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <iostream>
#include <bitset>

using namespace std;

const int MAXN = 3000;

// Declaring globally prevents stack overflow
bitset<MAXN> rows[MAXN];

int main() {
    // Fast I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        // Reads '0' and '1's directly into the bitset without string overhead
        cin >> rows[i];
    }

    long long ans = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            // Hardware popcnt makes this extremely fast
            int common = (rows[i] & rows[j]).count();
            ans += 1LL * common * (common - 1) / 2;
        }
    }

    cout << ans << '\n';

    return 0;
}