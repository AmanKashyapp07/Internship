// Link: https://cses.fi/problemset/task/3138
#include <bits/stdc++.h>
using namespace std;

struct Manacher {
    vector<int> d1, d2;
    Manacher(const string &s) {
        int n = s.size(); d1.assign(n, 0); d2.assign(n, 0);
        for (int l = 0, r = -1, i = 0; i < n; i++) {
            int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
            while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
            d1[i] = k--; if (i + k > r) { l = i - k; r = i + k; }
        }
        for (int l = 0, r = -1, i = 0; i < n; i++) {
            int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
            while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
            d2[i] = k--; if (i + k > r) { l = i - k - 1; r = i + k; }
        }
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; cin >> s;
    int n = s.size();
    Manacher m(s);
    vector<int> ans(n, 0);

    for (int c = 0; c < n; c++) {
        int r = m.d1[c], R = c + r - 1;
        ans[R] = max(ans[R], 2 * r - 1);
    }
    for (int c = 0; c < n; c++) {
        int r = m.d2[c]; if (r == 0) continue;
        int R = c + r - 1;
        ans[R] = max(ans[R], 2 * r);
    }
    for (int i = n - 1; i > 0; i--) {
        if (ans[i] >= 2) ans[i - 1] = max(ans[i - 1], ans[i] - 2);
    }
    for (int x : ans) cout << x << " ";
    cout << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each position i, find the maximum length of a palindromic substring ending at position i (CSES 3138).
// - Approach: Manacher's Algorithm + Right-endpoint propagation.
// - Intuition: Compute odd and even palindromic radii in $O(N)$ time; propagate maximum lengths backwards (`ans[i-1] = max(ans[i-1], ans[i]-2)`).
// - Complexity: Time: O(N), Space: O(N).