#include <bits/stdc++.h>
using namespace std;

struct Hierholzer {
    int n;
    vector<vector<int>> g;
    vector<int> path;

    Hierholzer(int n) : n(n), g(n) {}

    void addEdge(int u, int v) {
        g[u].push_back(v);
    }

    void dfs(int u) {
        while (!g[u].empty()) {
            int v = g[u].back();
            g[u].pop_back();
            dfs(v);
        }
        path.push_back(u);
    }

    vector<int> getEulerianCircuit(int start) {
        path.clear();
        dfs(start);
        reverse(path.begin(), path.end());
        return path;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n == 1) {
        cout << "01\n";
        return 0;
    }

    int V = 1 << (n - 1); // why n-1? Because we are considering all possible (n-1)-bit strings as vertices
    int mask = V - 1; // if n is 4, V is 8 (1000 in binary), mask is 7 (0111 in binary). This will help us to keep only the last (n-1) bits of the number.

    Hierholzer h(V);

    for (int u = 0; u < V; u++) {
        h.addEdge(u, (u << 1) & mask);           // append 0 , u<<1 will shift left and & mask will keep only the last (n-1) bits
        h.addEdge(u, ((u << 1) & mask) | 1);     // append 1 , u<<1 will shift left and & mask will keep only the last (n-1) bits, then we set the last bit to 1 using | 1
    }

    vector<int> path = h.getEulerianCircuit(0);
    int len = path.size();
    string ans(n - 1, '0');

    for (int i = 1; i < len; i++) // start from 1 because the first vertex is already represented by the initial string of '0's
        ans += char('0' + (path[i] & 1)); // path[i] & 1 will give us the last bit of the vertex, which is the bit we appended to get to this vertex.

    cout << ans << '\n';
}