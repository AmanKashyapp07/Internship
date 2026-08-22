// Link: https://cses.fi/problemset/task/1670

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string start;
    for (int i = 0; i < 9; i++) {
        int x;
        cin >> x;
        start += char('0' + x);
    }

    const string target = "123456789";

    // Edge case: already solved
    if (start == target) {
        cout << 0 << '\n';
        return 0;
    }

    vector<pair<int, int>> edges = {
        {0,1}, {1,2},
        {3,4}, {4,5},
        {6,7}, {7,8},
        {0,3}, {1,4}, {2,5},
        {3,6}, {4,7}, {5,8}
    };

    // Array of 2 queues and 2 maps:
    // Index 0 tracks the forward BFS (from start)
    // Index 1 tracks the backward BFS (from target)
    unordered_map<string, int> dist[2];
    queue<string> q[2];

    q[0].push(start);
    dist[0][start] = 0;

    q[1].push(target);
    dist[1][target] = 0;

    while (!q[0].empty() && !q[1].empty()) {
        // Optimization: always expand the queue with fewer elements
        // This keeps the branching factor as small as possible
        int dir = q[0].size() <= q[1].size() ? 0 : 1;
        
        string cur = q[dir].front();
        q[dir].pop();
        int d = dist[dir][cur];

        for (auto [u, v] : edges) {
            string nxt = cur;
            swap(nxt[u], nxt[v]);

            // If the opposite search has already visited this state, we've found the shortest path!
            if (dist[1 - dir].count(nxt)) {
                cout << d + 1 + dist[1 - dir][nxt] << '\n';
                return 0;
            }

            // If this state hasn't been visited in our current direction yet
            if (!dist[dir].count(nxt)) {
                dist[dir][nxt] = d + 1;
                q[dir].push(nxt);
            }
        }
    }

    return 0;
}