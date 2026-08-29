// Link: https://cses.fi/problemset/task/1111
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

    int maxLen = 0, start = 0;
    for (int i = 0; i < n; i++) {
        int oddLen = 2 * m.d1[i] - 1;
        if (oddLen > maxLen) { maxLen = oddLen; start = i - m.d1[i] + 1; }

        int evenLen = 2 * m.d2[i];
        if (evenLen > maxLen) { maxLen = evenLen; start = i - m.d2[i]; }
    }
    cout << s.substr(start, maxLen) << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the longest palindromic substring of s (CSES 1111).
// - Approach: Manacher's Algorithm ($O(N)$ linear time).
// - Intuition: Radii d1 (odd) and d2 (even) track maximum palindrome extensions around centers in $O(N)$ without re-scanning mirror bounds.
// - Complexity: Time: O(N), Space: O(N).