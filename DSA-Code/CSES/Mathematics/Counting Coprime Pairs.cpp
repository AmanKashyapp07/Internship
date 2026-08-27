// Link: https://cses.fi/problemset/task/2417
#include <bits/stdc++.h>
using namespace std;

const int MAX_VAL = 1000000;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; if (!(cin >> n)) return 0;
    vector<int> freq(MAX_VAL + 1, 0);
    int max_x = 0;
    for (int i = 0; i < n; i++) {
        int x; cin >> x; freq[x]++;
        max_x = max(max_x, x);
    }

    vector<long long> E(max_x + 1, 0);
    for (int d = max_x; d >= 1; d--) {
        long long mult = 0;
        for (int m = d; m <= max_x; m += d) mult += freq[m];
        E[d] = mult * (mult - 1) / 2;
        for (int m = 2 * d; m <= max_x; m += d) E[d] -= E[m];
    }
    cout << E[1] << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count pairs of elements (a[i], a[j]) that are coprime (gcd(a[i], a[j]) = 1) (CSES 2417).
// - Approach: Frequency Array + Backward Mobius-like Inversion (Inclusion-Exclusion on Exact GCDs).
// - Intuition: Calculate naive pairs P(d) = C(multiples_count, 2); subtract exact pairs E[m] for strict multiples m of d to obtain exact GCD counts E[d]; answer is E[1].
// - Complexity: Time: O(MAX log MAX + N), Space: O(MAX).