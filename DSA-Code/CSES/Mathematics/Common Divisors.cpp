// Link: https://cses.fi/problemset/task/1081
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    const int MAXX = 1'000'000;
    int n; cin >> n;
    vector<int> freq(MAXX + 1, 0);
    while (n--) { int x; cin >> x; freq[x]++; }
    int ans = 1;
    for (int d = 1; d <= MAXX; d++) {
        int cnt = 0;
        for (int m = d; m <= MAXX; m += d) cnt += freq[m];
        if (cnt >= 2) ans = d;
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the maximum GCD shared by at least two numbers in the array (CSES 1081).
// - Approach: Frequency array + Sieve-style divisor counting — take the largest d with ≥2 multiples.
// - Intuition: For each candidate GCD d, count how many input numbers are multiples of d; the largest such d with ≥2 is the answer.
// - Complexity: Time: O(N + MAX log MAX), Space: O(MAX).