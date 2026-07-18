// Link: https://cses.fi/problemset/task/3308


#include <bits/stdc++.h>
using namespace std;

struct ChromaticNumber {
    int n;
    vector<int> adj;
    vector<bool> independent;
    vector<int> dp;

    ChromaticNumber(int _n) : n(_n) {
        adj.assign(n, 0);
    }

    void addEdge(int u, int v) {
        adj[u] |= (1 << v);
        adj[v] |= (1 << u);
    }

    int solve() {
        int N = 1 << n;

        independent.assign(N, false);
        independent[0] = true;

        // Compute all independent sets
        for (int mask = 1; mask < N; mask++) {
            int v = __builtin_ctz(mask);
            int rest = mask ^ (1 << v);
            independent[mask] = independent[rest] && ((adj[v] & rest) == 0);
        }

        const int INF = 1e9;
        dp.assign(N, INF);
        dp[0] = 0;

        // DP over submasks
        for (int mask = 1; mask < N; mask++) {
            for (int sub = mask; sub; sub = (sub - 1) & mask) {
                if (independent[sub]) {
                    dp[mask] = min(dp[mask], dp[mask ^ sub] + 1);
                }
            }
        }

        return dp[N - 1];
    }

    vector<int> assignColour() {
        vector<int> colour(n, -1);

        int mask = (1 << n) - 1;
        int curColour = 1;

        // Backtrack to find the actual color assignments
        while (mask) {
            for (int sub = mask; sub; sub = (sub - 1) & mask) {
                if (independent[sub] && dp[mask] == dp[mask ^ sub] + 1) {

                    for (int i = 0; i < n; i++) {
                        if (sub & (1 << i))
                            colour[i] = curColour;
                    }

                    mask ^= sub;
                    curColour++;
                    break;
                }
            }
        }

        return colour;
    }
};

int main() {
    // Fast I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    ChromaticNumber c(n);

    while (m--) {
        int u, v;
        cin >> u >> v;
        
        // FIX: Convert from 1-based indexing (problem format) 
        // to 0-based indexing (your struct format)
        u--; 
        v--; 
        
        c.addEdge(u, v);
    }

    int chromatic = c.solve();
    cout << chromatic << "\n";

    vector<int> colour = c.assignColour();

    for (int i = 0; i < n; i++) {
        cout << colour[i] << (i == n - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}