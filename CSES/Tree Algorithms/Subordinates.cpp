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
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

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
