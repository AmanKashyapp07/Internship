// Link: https://cses.fi/problemset/task/3308
#include <bits/stdc++.h>
using namespace std;

struct ChromaticNumber {
    int n;
    vector<int> graph;
    vector<bool> independent;
    vector<int> dp;

    ChromaticNumber(int n) : n(n), graph(n, 0) {}

    void addEdge(int u, int v) { graph[u] |= (1 << v); graph[v] |= (1 << u); }

    int solve() {
        int N = 1 << n;
        independent.assign(N, false); independent[0] = true;
        for (int mask = 1; mask < N; mask++) {
            int v = __builtin_ctz(mask), rest = mask ^ (1 << v);
            independent[mask] = independent[rest] && ((graph[v] & rest) == 0);
        }
        dp.assign(N, 1e9); dp[0] = 0;
        for (int mask = 1; mask < N; mask++) {
            for (int sub = mask; sub; sub = (sub - 1) & mask) {
                if (independent[sub]) dp[mask] = min(dp[mask], dp[mask ^ sub] + 1);
            }
        }
        return dp[N - 1];
    }

    vector<int> assignColour() {
        vector<int> colour(n, -1);
        int mask = (1 << n) - 1, curColour = 1;
        while (mask) {
            for (int sub = mask; sub; sub = (sub - 1) & mask) {
                if (independent[sub] && dp[mask] == dp[mask ^ sub] + 1) {
                    for (int i = 0; i < n; i++) if (sub & (1 << i)) colour[i] = curColour;
                    mask ^= sub; curColour++; break;
                }
            }
        }
        return colour;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; if (!(cin >> n >> m)) return 0;
    ChromaticNumber c(n);
    while (m--) { int u, v; cin >> u >> v; c.addEdge(u - 1, v - 1); }

    cout << c.solve() << "\n";
    vector<int> col = c.assignColour();
    for (int i = 0; i < n; i++) cout << col[i] << (i == n - 1 ? "" : " ");
    cout << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find chromatic number (minimum colors) and assign colors to vertices (CSES 3308).
// - Approach: Bitmask DP over Submasks of Independent Sets ($O(3^N)$).
// - Intuition: Precomputing independent submasks allows dynamic programming state transitions `dp[mask] = min(dp[mask ^ sub] + 1)` for independent submask `sub`.
// - Complexity: Time: O(3^N), Space: O(2^N).