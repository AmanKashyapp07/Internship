#include <bits/stdc++.h>
#define int long long
#define input(v) for (auto &x : v) cin >> x
#define srt(v) sort((v).begin(), (v).end())
#define rev(v) reverse((v).begin(), (v).end())
#define maxx(v) (*max_element((v).begin(), (v).end()))
#define minn(v) (*min_element((v).begin(), (v).end()))
using namespace std;
#define MOD 1000000007
const int MAXN = 1e6 + 5;

void prt(vector<int>&v){
    for(auto c:v) cout<<c<<" ";
    cout<<"\n";
}
/*
int binaryExponentiation(int base, int exponent) {
    int result = 1;
    base %= MOD;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (1LL * result * base) % MOD;
        }
        base = (1LL * base * base) % MOD;
        exponent /= 2;
    }
    return result;
}
*/

/*
vector<int> fact(MAXN), invFact(MAXN);

void initFactorials() {
    fact[0] = 1;
    for (int i = 1; i < MAXN; i++) fact[i] = (fact[i - 1] * i) % MOD;
    invFact[MAXN - 1] = binaryExponentiation(fact[MAXN - 1], MOD - 2);
    for (int i = MAXN - 2; i >= 0; i--) invFact[i] = (invFact[i + 1] * (i + 1)) % MOD;
}

int nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return (((fact[n] * invFact[r]) % MOD) * invFact[n - r]) % MOD;
}
*/

/*
vector<int> primes;
vector<bool> isPrime;

void sieve(int n) {
    isPrime.assign(n + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i <= n; i++) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }
    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) primes.push_back(i);
    }
}
*/

/*
void readGraph(int nodes, int edges, bool directed = false) {
    vector<vector<int>> adj(nodes + 1); // 1-indexed
    for (int i = 0; i < edges; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        if (!directed) adj[v].push_back(u);
    }
}
*/

/*
void dfs(int node, vector<vector<int>> &adj, vector<bool> &visited) {
    visited[node] = true;
    // process node here
    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) dfs(neighbor, adj, visited);
    }
}
*/

/*
void dfsTree(int u, int p, vector<vector<int>> &adj) {
    // before exploring children
    for (int v : adj[u]) {
        if (v == p) continue;
        dfsTree(v, u, adj);
    }
    // after exploring children
}
*/

/*
void bfs(int start, vector<vector<int>> &adj) {
    vector<bool> visited(adj.size(), false);
    queue<int> q;
    q.push(start);
    visited[start] = true;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        // process node here
        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}
*/

/*
int binarySearchSkeleton(int n) {
    int left = 0, right = n - 1;
    int ans = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        // if (condition(mid)) {
        //     ans = mid;
        //     right = mid - 1; // or left = mid + 1
        // } else {
        //     left = mid + 1; // or right = mid - 1
        // }
    }
    return ans;
}
*/

/*
sort(vp.begin(), vp.end(), [](pair<int,int> a, pair<int,int> b) {
    return a.second < b.second;
});
*/

// ---------------------------
// Lower Bound / Upper Bound
// ---------------------------

// Using STL:
/*
int idx1 = lower_bound(v.begin(), v.end(), x) - v.begin(); // first >= x
int idx2 = upper_bound(v.begin(), v.end(), x) - v.begin(); // first > x
*/



// ---------------------------
// Tarjan Bridge Detection
// ---------------------------
/*
int timerCnt;
vector<int> tin, low;
vector<vector<int>> adj;
vector<pair<int,int>> bridges;

void tarjanDFS(int u, int p = -1) {
    tin[u] = low[u] = ++timerCnt;
    for (int v : adj[u]) {
        if (v == p) continue;
        if (!tin[v]) {
            tarjanDFS(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) {
                bridges.push_back({u, v}); // (u,v) is a bridge
            }
        } else {
            low[u] = min(low[u], tin[v]);
        }
    }
}
*/

/*
void findBridges(int n) {
    tin.assign(n + 1, 0);
    low.assign(n + 1, 0);
    timerCnt = 0;
    bridges.clear();
    for (int i = 1; i <= n; i++) {
        if (!tin[i]) tarjanDFS(i);
    }
}
*/


// ---------------------------
// DSU (Disjoint Set Union)
// ---------------------------
/*
struct DSU {
    vector<int> parent, sz;
    DSU(int n) {
        parent.resize(n+1);
        sz.assign(n+1,1);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (x == parent[x]) return x;
        return parent[x] = find(parent[x]);
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a,b);
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }
};
*/

// ---------------------------
// Aman Kashyap
// ---------------------------

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    //initFactorials();
    //sieve(1000000);

    int t=1;
    //cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> ish(n);
        input(ish);
        srt(ish);
        if(n&1){
            int median = ish[n/2];
            int cost=0;
            for(auto v:ish) cost+=abs(v-median);
            cout<<cost<<endl;
        }
        else{
            int median1 = ish[n/2-1];
            int median2 = ish[n/2];
            int cost1=0, cost2=0;
            for(auto v:ish){
                cost1+=abs(v-median1);
                cost2+=abs(v-median2);
            }
            cout<<min(cost1, cost2)<<endl;
        }

        
    }

    return 0;
}