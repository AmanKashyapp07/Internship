// Link: https://cses.fi/problemset/task/1753
#include <bits/stdc++.h>
using namespace std;

vector<int> buildPi(const string &s) {
    int n = s.size();
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s, p; if (!(cin >> s >> p)) return 0;
    int n = s.size(), m = p.size();
    if (m > n) { cout << 0 << '\n'; return 0; }

    string concat = p + "#" + s;
    vector<int> pi = buildPi(concat);

    int count = 0;
    for (size_t i = m + 1; i < concat.size(); i++) {
        if (pi[i] == m) count++;
    }
    cout << count << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count all occurrences of pattern p in string s (CSES 1753).
// - Approach: KMP Pattern Matching via concatenated string `p + "#" + s`.
// - Intuition: Prefix function values `pi[i] == |p|` in concatenated string demarcate complete matches of pattern p.
// - Complexity: Time: O(N + M), Space: O(N + M).