/**
 * CSES Problem Set
 * 
 * Problem: CSES Solution Template
 * Link: https://cses.fi/
 * Category: Template
 * 
 * Description:
 * A template file containing boilerplate code, fast I/O setup, and common macros/imports for solving CSES problems.
 * 
 * Logic/Approach:
 * Provides basic imports, standard namespace, and empty main function with fast I/O configuration.
 */

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
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;

#define all(x)   (x).begin(), (x).end()
#define rall(x)  (x).rbegin(), (x).rend()
#define pb       push_back
#define ff       first
#define ss       second

const int INF  = INT_MAX;
const ll  LINF = LLONG_MAX;
const ll  MOD  = 1e9 + 7;

// ─────────────────────────────────────────────────────────────────────────────

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,m;
    cin>>n>>m;
    vector<vector<int>> adj(n+1);
    for(int i=0;i<m;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<bool> visited(n+1,false);
    int ans=INF;
    for(int i=1;i<=n;i++){
        queue<pair<int,int>> q;
        q.push({i,-1});
        vector<int> dist(n+1,INF);
        dist[i]=0;
        while(!q.empty()){
            auto [u,p]=q.front();
            q.pop();
            for(int v:adj[u]){
                if(v==p) continue;
                if(dist[v]==INF){
                    dist[v]=dist[u]+1;
                    q.push({v,u});
                }else{
                    ans=min(ans,dist[u]+dist[v]+1); // dist[u] + dist[v] + 1 is the length of the cycle formed by the edge u-v and the paths from i to u and i to v, because 1 is added for the edge u-v itself, and dist[u] and dist[v] are the distances from the starting node i to u and v respectively, so the total length of the cycle is the sum of these three components. How? 
                }
            }
        }
    }

    cout<<(ans==INF?-1:ans)<<endl; // if ans is still INF, it means we never found a cycle, so we print -1, otherwise we print the length of the shortest cycle found.
    return 0;
}
