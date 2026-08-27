// Link: https://cses.fi/problemset/task/2186
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; cin >> s;
    vector<bool> present(26, false);
    for (char c : s) present[c - 'a'] = true;

    vector<int> chars;
    for (int i = 0; i < 26; i++) if (present[i]) chars.push_back(i);

    map<vector<int>, long long> mp;
    vector<int> cnt(26, 0), state(chars.size(), 0);
    mp[state] = 1;
    long long ans = 0;

    for (char ch : s) {
        cnt[ch - 'a']++;
        int mn = INT_MAX;
        for (int idx : chars) mn = min(mn, cnt[idx]);
        for (size_t i = 0; i < chars.size(); i++) state[i] = cnt[chars[i]] - mn;
        ans += mp[state];
        mp[state]++;
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count substrings where every character occurring in string s appears an equal number of times (CSES 2186).
// - Approach: Frequency Difference State Vector Hashing.
// - Intuition: Normalize character frequencies by subtracting min frequency across distinct string characters; equal states form valid special substrings.
// - Complexity: Time: O(N \cdot |\Sigma| \log N), Space: O(N \cdot |\Sigma|).