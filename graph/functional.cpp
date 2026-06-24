#include <bits/stdc++.h>
using namespace std;

vector<int> nxt;
vector<int> state; // 0 = unvisited, 1 = visiting, 2 = processed
int cycleStart = -1;

bool dfs(int u) {
    state[u] = 1;

    int v = nxt[u];

    if (state[v] == 0) {
        if (dfs(v)) return true;
    }
    else if (state[v] == 1) {
        cycleStart = v;
        return true;
    }

    state[u] = 2;
    return false;
} // used for detecting cycle in a functional graph (each node has exactly one outgoing edge). The idea is to use DFS to traverse the graph and keep track of the state of each node. If we encounter a node that is currently being visited (state 1), we have found a cycle. We can then backtrack to find the starting point of the cycle.

vector<int> findCycle(int n) {
    for (int i = 1; i <= n; i++) {
        if (state[i] == 0 && dfs(i)) { // if we find a cycle starting from node i, we can reconstruct the cycle by following the next pointers from the starting node until we reach the starting node again.

            vector<int> cycle;
            int cur = cycleStart;

            while(cur!= cycleStart || cycle.empty()) {
                cycle.push_back(cur);
                cur = nxt[cur];
            }

            return cycle;
        }
    }

    return {};
}

int main() {
    int n;
    cin >> n;

    nxt.resize(n + 1);
    state.assign(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        cin >> nxt[i];
    }

    vector<int> cycle = findCycle(n);

    for (int x : cycle) {
        cout << x << " ";
    }
}