// Link: https://cses.fi/problemset/task/1069
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; cin >> s;
    int maxLen = 1, curLen = 1;
    for (size_t i = 1; i < s.size(); i++) {
        if (s[i] == s[i - 1]) curLen++;
        else curLen = 1;
        maxLen = max(maxLen, curLen);
    }
    cout << maxLen << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the length of the longest substring of identical adjacent characters (CSES 1069).
// - Approach: Single pass sliding window / counter.
// - Intuition: Increment counter if current char equals previous char, reset to 1 otherwise; update max length dynamically.
// - Complexity: Time: O(N), Space: O(1) beyond input string.