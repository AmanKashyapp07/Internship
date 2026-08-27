// Link: https://cses.fi/problemset/task/2107
#include <bits/stdc++.h>
using namespace std;

vector<int> getZ(const string &s) {
    int n = s.size();
    vector<int> z(n, 0);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] - 1 > r) { l = i; r = i + z[i] - 1; }
    }
    return z;
}

vector<int> getPi(const string &s) {
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
    string s; cin >> s;
    vector<int> z = getZ(s);
    vector<int> pi = getPi(s);

    for (int x : z) cout << x << ' ';
    cout << '\n';
    for (int x : pi) cout << x << ' ';
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Compute the Z-array and the KMP prefix function ($\pi$-array) for string s (CSES 2107).
// - Approach: Z-Algorithm ($O(N)$) and KMP Prefix Function ($O(N)$).
// - Intuition: Z[i] stores longest common prefix between s and suffix s[i..n-1]; $\pi$[i] stores length of longest proper prefix of s[0..i] that is also a suffix.
// - Complexity: Time: O(N), Space: O(N).