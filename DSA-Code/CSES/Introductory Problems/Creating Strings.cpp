// Link: https://cses.fi/problemset/task/1622
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; cin >> s;
    sort(s.begin(), s.end());
    vector<string> res;
    do { res.push_back(s); } while (next_permutation(s.begin(), s.end()));
    cout << res.size() << '\n';
    for (const string& str : res) cout << str << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Generate all unique permutations of a string in lexicographical order (CSES 1622).
// - Approach: Sorting + `std::next_permutation`.
// - Intuition: Sorting string first ensures start at lexicographically smallest permutation; `next_permutation` generates all distinct permutations automatically.
// - Complexity: Time: O(K! \cdot K) where K <= 8, Space: O(K!).