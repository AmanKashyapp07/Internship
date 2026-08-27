// Link: https://cses.fi/problemset/task/1670
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string start = "";
    for (int i = 0; i < 9; i++) { int x; cin >> x; start += char('0' + x); }
    const string target = "123456789";
    if (start == target) { cout << 0 << '\n'; return 0; }

    vector<pair<int, int>> edges = {
        {0,1},{1,2},{3,4},{4,5},{6,7},{7,8},
        {0,3},{1,4},{2,5},{3,6},{4,7},{5,8}
    };

    unordered_map<string, int> dist[2];
    queue<string> q[2];
    q[0].push(start); dist[0][start] = 0;
    q[1].push(target); dist[1][target] = 0;

    while (!q[0].empty() && !q[1].empty()) {
        int dir = q[0].size() <= q[1].size() ? 0 : 1;
        string cur = q[dir].front(); q[dir].pop();
        int d = dist[dir][cur];

        for (auto [u, v] : edges) {
            string nxt = cur; swap(nxt[u], nxt[v]);
            if (dist[1 - dir].count(nxt)) {
                cout << d + 1 + dist[1 - dir][nxt] << '\n';
                return 0;
            }
            if (!dist[dir].count(nxt)) {
                dist[dir][nxt] = d + 1;
                q[dir].push(nxt);
            }
        }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum number of adjacent swaps to transform a 3x3 grid into 1..9 order (CSES 1670).
// - Approach: Bidirectional BFS meeting in the middle.
// - Intuition: Expanding smaller queue frontier simultaneously from start and target reduces search space branch factor exponentially.
// - Complexity: Time: O(9! / 2) state visits, Space: O(9!).