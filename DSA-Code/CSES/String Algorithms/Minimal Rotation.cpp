// Link: https://cses.fi/problemset/task/1110
#include <bits/stdc++.h>
using namespace std;

string boothAlgorithm(string s) {
    string t = s + s;
    int n = s.size();
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        if (t[i + k] == t[j + k]) k++;
        else if (t[i + k] < t[j + k]) { j += k + 1; if (i == j) j++; k = 0; }
        else { i += k + 1; if (i == j) i++; k = 0; }
    }
    int start = min(i, j);
    return t.substr(start, n);
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; cin >> s;
    cout << boothAlgorithm(s) << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the lexicographically smallest rotation of string s (CSES 1110).
// - Approach: Booth's Lexicographically Smallest Rotation Algorithm ($O(N)$).
// - Intuition: Two-pointer comparison on doubled string $s+s$ with prefix offset $k$; jumping candidates by $k+1$ when a mismatch occurs yields $O(N)$ runtime.
// - Complexity: Time: O(N), Space: O(N).