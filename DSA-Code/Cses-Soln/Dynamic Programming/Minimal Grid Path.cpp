// Link: https://cses.fi/problemset/task/3359
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<string> grid(n);
    for (int i = 0; i < n; i++) cin >> grid[i];

    string ans; ans += grid[0][0];
    vector<pair<int, int>> cur = {{0, 0}};
    vector<vector<bool>> vis(n, vector<bool>(n, false));
    vis[0][0] = true;

    for (int step = 0; step < 2 * n - 2; step++) {
        char best = 'z' + 1;
        for (auto [x, y] : cur) {
            if (x + 1 < n) best = min(best, grid[x + 1][y]);
            if (y + 1 < n) best = min(best, grid[x][y + 1]);
        }
        ans += best;

        vector<pair<int, int>> nxt;
        for (auto [x, y] : cur) {
            if (x + 1 < n && grid[x + 1][y] == best && !vis[x + 1][y]) {
                vis[x + 1][y] = true; nxt.push_back({x + 1, y});
            }
            if (y + 1 < n && grid[x][y + 1] == best && !vis[x][y + 1]) {
                vis[x][y + 1] = true; nxt.push_back({x, y + 1});
            }
        }
        cur = move(nxt);
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find lexicographically smallest string formed by walking from top-left to bottom-right in an n x n grid (CSES 3359).
// - Approach: Step-by-Step Level BFS / Greedy Selection.
// - Intuition: At step $k$, find minimum character among all reachable neighbor cells; advance only frontier cells matching `best` character.
// - Complexity: Time: O(N^2), Space: O(N^2).