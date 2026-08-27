// Link: https://cses.fi/problemset/task/1073
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> towers;

    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        auto it = upper_bound(towers.begin(), towers.end(), x);
        if (it == towers.end()) towers.push_back(x);
        else *it = x;
    }
    cout << towers.size() << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum number of towers needed to place blocks in order such that higher blocks are smaller than lower blocks (CSES 1073).
// - Approach: Greedy Binary Search (`upper_bound`) / Patience Sorting (LIS Variant).
// - Intuition: Always place current block on the smallest tower top that is strictly greater than the block (`upper_bound`), otherwise create a new tower.
// - Complexity: Time: O(N \log N), Space: O(N).