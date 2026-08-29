// Link: https://cses.fi/problemset/task/1715
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

long long pw(long long a, long long b) {
    long long r = 1; a %= MOD;
    for (; b; b >>= 1) { if (b & 1) r = r * a % MOD; a = a * a % MOD; }
    return r;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; cin >> s;
    int n = s.size();
    vector<long long> fact(n + 1), inv(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i % MOD;
    inv[n] = pw(fact[n], MOD - 2);
    for (int i = n - 1; i >= 0; i--) inv[i] = inv[i + 1] * (i + 1) % MOD;

    vector<int> freq(26, 0);
    for (char c : s) freq[c - 'a']++;

    long long ans = fact[n];
    for (int f : freq) ans = ans * inv[f] % MOD;
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count distinct permutations of a string with repeating characters (CSES 1715).
// - Approach: Multinomial coefficient formula — n! / (f1! * f2! * ... * fk!) mod 10^9+7.
// - Intuition: Total permutations divided by factorials of character frequencies to eliminate duplicate arrangements.
// - Complexity: Time: O(N), Space: O(N).