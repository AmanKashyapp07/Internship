// Link: https://cses.fi/problemset/task/1755
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; cin >> s;
    vector<int> cnt(26, 0);
    for (char c : s) cnt[c - 'A']++;

    int oddCnt = 0, oddIdx = -1;
    for (int i = 0; i < 26; i++) {
        if (cnt[i] % 2 != 0) { oddCnt++; oddIdx = i; }
    }

    if (oddCnt > 1) { cout << "NO SOLUTION\n"; return 0; }

    string half = "";
    for (int i = 0; i < 26; i++) half.append(cnt[i] / 2, 'A' + i);
    string ans = half;
    if (oddIdx != -1) ans.push_back('A' + oddIdx);
    reverse(half.begin(), half.end());
    ans += half;

    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Reorder characters of string s into a palindrome if possible (CSES 1755).
// - Approach: Frequency Counting + Symmetric Half Mirroring.
// - Intuition: A palindrome can have at most one character with odd frequency; construct left half alphabetically, place odd char (if any) in middle, append reversed left half.
// - Complexity: Time: O(N), Space: O(N).