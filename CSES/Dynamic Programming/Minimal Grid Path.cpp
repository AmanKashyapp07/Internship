// Link: https://cses.fi/problemset/task/3359

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> grid(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }

    string ans;
    ans += grid[0][0];

    vector<pair<int, int>> cur = {{0, 0}};
    vector<vector<bool>> vis(n, vector<bool>(n, false));
    vis[0][0] = true;

    for (int step = 0; step < 2 * n - 2; step++) {
        char best = 'Z' + 1;

        for (auto [x, y] : cur) {
            if (x + 1 < n) best = min(best, grid[x + 1][y]);
            if (y + 1 < n) best = min(best, grid[x][y + 1]);
        } // finding best character in next step

        ans += best; // we add the best character to the answer

        vector<pair<int, int>> nxt;

        for (auto [x, y] : cur) {
            if (x + 1 < n && grid[x + 1][y] == best &&
                !vis[x + 1][y]) {
                vis[x + 1][y] = true;
                nxt.push_back({x + 1, y}); // we add the next position to the queue for the next step
            }

            if (y + 1 < n && grid[x][y + 1] == best &&
                !vis[x][y + 1]) {
                vis[x][y + 1] = true;
                nxt.push_back({x, y + 1}); // we add the next position to the queue for the next step
            }
        }

        cur = move(nxt); // we move the next positions to the current positions for the next step, move is used to avoid copying the vector
    } // 2*n - 2 steps in total because we need to reach the bottom right corner from the top left corner , so n-1 steps down and n-1 steps right

    cout << ans << '\n';
    return 0;
}