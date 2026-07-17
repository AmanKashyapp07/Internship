// Link: https://cses.fi/problemset/task/1674

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

vector<vector<int>> tree;
vector<int> subordinates;
vector<int>subtree_size;

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
    cin >> n;

    tree.resize(n + 1);
    subordinates.resize(n + 1);
    subtree_size.resize(n + 1);
    
    for (int employee = 2; employee <= n; employee++) {
        int boss;
        cin >> boss;
        tree[boss].push_back(employee);
    }

    dfs(1);

    for (int i = 1; i <= n; i++) {
        cout << subordinates[i] << ' ';
    }
    cout << '\n';

    return 0;
}