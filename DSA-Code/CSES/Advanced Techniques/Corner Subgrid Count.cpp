// Link: https://cses.fi/problemset/task/2137
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 3000;
bitset<MAXN> rows[MAXN];

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    for (int i = 0; i < n; i++) cin >> rows[i];

    long long ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            long long common = (rows[i] & rows[j]).count();
            ans += common * (common - 1) / 2;
        }
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count 2x2 subgrids with all corners set to 1 in a binary n x n grid (CSES 2137).
// - Approach: Bitset Row AND + Popcount Pairwise Enumeration.
// - Intuition: For row pair $(i, j)$, the number of column pairs both set to 1 is $\binom{\text{common}}{2}$ = `common*(common-1)/2`.
// - Complexity: Time: O(N^2 \cdot N / 64), Space: O(N^2 / 8).