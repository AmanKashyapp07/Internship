// CSES Problem: <problem name>
// https://cses.fi/problemset/task/<id>

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
#define int long long
#define MOD 1000000007

using namespace std;
// ─────────────────────────────────────────────────────────────────────────────

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,m;
    cin >> n >> m;
    vector<vector<int>> adj(n+1);
    vector<int> in_degree(n+1, 0);
    for(int i=0;i<m;i++) {
        int a,b;
        cin >> a >> b;
        adj[a].push_back(b);
        in_degree[b]++;
    }

    vector<int>ways(n+1,0); // ways vector define the number of ways to reach each node from the starting node (node 1 in this case). Initially, all values are set to 0, except for the starting node which will be set to 1 later on when we process the nodes with zero in-degrees. This vector will be updated as we perform a topological sort of the graph, where for each node, we will add the number of ways to reach it from its predecessors. By the end of the algorithm, ways[n] will give us the total number of ways to reach node n from node 1.
    queue<int> q;
    ways[1]=1; // we set ways[1] to 1 because there is exactly one way to reach the starting node from itself, which is to simply be at that node without taking any steps. This serves as the base case for our dynamic programming approach, allowing us to build up the number of ways to reach other nodes in the graph as we process them in topological order.
    for(int i=1;i<=n;i++) {
        if(in_degree[i]==0) {
          //  ways[i]=1;
            q.push(i);
        }
    }
    while(!q.empty()) {
        int u=q.front();
        q.pop();
        for(int v:adj[u]) {
            ways[v]=(ways[v]+ways[u])%MOD;
            in_degree[v]--;
            if(in_degree[v]==0) {
                q.push(v);
            }
        }
    }
    cout << ways[n] << "\n";


    return 0;
}
