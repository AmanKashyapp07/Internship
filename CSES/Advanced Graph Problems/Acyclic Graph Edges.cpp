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

void dfs(int u, int p, vector<vector<int>>& adj, vector<bool>& visited) {
    visited[u] = true;

    for (int v : adj[u]) {
        if (v == p) continue; // Skip the parent node
        if (visited[v]) {
            cout<<v<<" "<<u<<endl;
            //stop this dfs path
            return;
        }
        cout<<u<<" "<<v<<endl;
        dfs(v, u, adj, visited);
    }
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n,m;
    cin >> n >> m;
    vector<vector<int>> adj(n+1);
    // take input of undirected graph
    for(int i=0;i<m;i++){
        int u,v;
        cin >> u >> v;
        if(u<v){
            cout<<u<<" "<<v<<endl;
        }
        else{
            cout<<v<<" "<<u<<endl;
        }
        adj[u].pb(v);
        adj[v].pb(u);
    }
    

    return 0;
}
