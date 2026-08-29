// Link: https://cses.fi/problemset/task/3360
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; if (!(cin >> n >> k)) return 0;
    vector<string> grid(n);
    for (int i = 0; i < n; i++) cin >> grid[i];

    for (int ci = 0; ci < k; ci++) {
        char target = 'A' + ci;
        bool found = false;
        vector<vector<bool>> seen(n, vector<bool>(n, false));

        for (int r = 0; r < n && !found; r++) {
            vector<int> cols;
            for (int c = 0; c < n; c++) if (grid[r][c] == target) cols.push_back(c);

            for (int i = 0; i < (int)cols.size() && !found; i++) {
                for (int j = i + 1; j < (int)cols.size() && !found; j++) {
                    int c1 = cols[i], c2 = cols[j];
                    if (seen[c1][c2]) found = true;
                    else seen[c1][c2] = true;
                }
            }
        }
        cout << (found ? "YES\n" : "NO\n");
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: For each of k letters, check if an all-corner 2x2 subgrid exists with that letter at all four corners (CSES 3360).
// - Approach: Column Pair Seen Tracking per Row.
// - Intuition: For letter $c$, a 2x2 corner subgrid exists iff some column pair $(c_1, c_2)$ both have letter $c$ in two separate rows; tracking `seen[c1][c2]` detects this.
// - Complexity: Time: O(k \cdot N^2), Space: O(N^2).