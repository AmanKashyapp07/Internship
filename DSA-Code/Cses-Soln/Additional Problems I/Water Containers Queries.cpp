// Link: https://cses.fi/problemset/task/3214
#include <bits/stdc++.h>
using namespace std;

void solve() {
    long long a, b, x;
    cin >> a >> b >> x;
    if (x <= a && x % std::gcd(a, b) == 0) cout << "YES\n";
    else cout << "NO\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (cin >> t) {
        while (t--) solve();
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Determine whether x units of water can be measured in container A using capacities a and b (CSES 3214).
// - Approach: Bézout's Identity & Greatest Common Divisor (GCD).
// - Intuition: Any reachable fill level in container A must be an integer multiple of gcd(a, b) and cannot exceed the container's capacity a.
// - Complexity: Time: O(log(min(a, b))) per query, Space: O(1).
