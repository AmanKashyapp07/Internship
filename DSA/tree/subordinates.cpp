/**
 * Problem: Subordinates
 * Link: https://cses.fi/problemset/task/1674
 * Category: Tree
 * 
 * Description:
 * Find the number of subordinates for each employee.
 * 
 * Logic/Approach:
 * DFS subtree size calculation.
 */

#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> tree;
vector<int> subordinates, subtree_size;

void dfs(int u) {
    subtree_size[u] = 1;
    for (int v : tree[u]) {
        dfs(v);
        subtree_size[u] += subtree_size[v];
    }
    subordinates[u] = subtree_size[u] - 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    tree.resize(n + 1);
    subordinates.resize(n + 1);
    subtree_size.resize(n + 1);
    for (int i = 2; i <= n; i++) {
        int boss;
        cin >> boss;
        tree[boss].push_back(i);
    }
    dfs(1);
    for (int i = 1; i <= n; i++) cout << subordinates[i] << " ";
    cout << "\n";
    return 0;
}
