// Link: https://cses.fi/problemset/task/2216
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> pos(n + 1);
    for (int i = 0; i < n; i++) { int x; cin >> x; pos[x] = i + 1; }

    int rounds = 1;
    for (int i = 1; i < n; i++) {
        if (pos[i] > pos[i + 1]) rounds++;
    }
    cout << rounds << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the number of left-to-right passes to collect numbers 1 to n in ascending order (CSES 2216).
// - Approach: Position Array Inversion Count (`pos[x] > pos[x+1]`).
// - Intuition: A new round is required whenever the index of number $x+1$ appears before the index of number $x$.
// - Complexity: Time: O(N), Space: O(N).
