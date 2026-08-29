// Link: https://cses.fi/problemset/task/1145
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<long long> tails;

    for (int i = 0; i < n; i++) {
        long long x; cin >> x;
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    cout << tails.size() << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find length of the longest increasing subsequence (LIS) (CSES 1145).
// - Approach: Patience Sorting / Greedy Binary Search (`lower_bound` on `tails` array).
// - Intuition: `tails[len]` stores smallest tail of all increasing subsequences of length `len+1`; updating via binary search runs in $O(N \log N)$.
// - Complexity: Time: O(N \log N), Space: O(N).
