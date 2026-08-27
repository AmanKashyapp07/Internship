// Link: https://cses.fi/problemset/task/1094
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    long long moves = 0, prev; cin >> prev;
    for (int i = 1; i < n; i++) {
        long long curr; cin >> curr;
        if (curr < prev) moves += (prev - curr);
        else prev = curr;
    }
    cout << moves << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum operations to make an array non-decreasing (CSES 1094).
// - Approach: Greedy running prefix maximum tracking.
// - Intuition: If curr < prev, we must increment curr to prev; add (prev - curr) to moves while keeping running max prev.
// - Complexity: Time: O(N), Space: O(1).