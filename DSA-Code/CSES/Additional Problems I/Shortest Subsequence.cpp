// Link: https://cses.fi/problemset/task/1087
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; cin >> s;
    string ans = "";
    set<char> seen;

    for (char c : s) {
        seen.insert(c);
        if (seen.size() == 4) {
            ans += c;
            seen.clear();
        }
    }

    for (char c : {'A', 'C', 'G', 'T'}) {
        if (!seen.count(c)) { ans += c; break; }
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the shortest DNA string over alphabet {'A', 'C', 'G', 'T'} that is NOT a subsequence of s (CSES 1087).
// - Approach: Greedy Epoch / Block Partitioning.
// - Intuition: Partition s into max complete blocks containing all 4 characters; appending the 4th character of each block forces any generated sequence to span past that block.
// - Complexity: Time: O(N), Space: O(1).