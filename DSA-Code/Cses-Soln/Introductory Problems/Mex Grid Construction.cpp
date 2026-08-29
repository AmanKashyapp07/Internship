// Link: https://cses.fi/problemset/task/3305
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<vector<int>> grid(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            grid[i][j] = i ^ j;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << grid[i][j] << " \n"[j == n - 1];
        }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Construct an n x n grid such that cell (i, j) is MEX of row i and col j prefixes (CSES 3305).
// - Approach: Bitwise XOR Grid Construction (`grid[i][j] = i ^ j`).
// - Intuition: The XOR operation table $i \oplus j$ produces a Latin square property over powers of two, ensuring cell values satisfy the prefix MEX requirements for every subgrid.
// - Complexity: Time: O(N^2), Space: O(N^2).