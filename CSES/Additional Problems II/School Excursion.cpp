// Link: https://cses.fi/problemset/task/1706

#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, sz;
    int comp;
    DSU(int n) : p(n + 1), sz(n + 1, 1), comp(n) { iota(p.begin(), p.end(), 0); }

    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }

    bool unite(int a, int b) {
        if ((a = find(a)) == (b = find(b))) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b]; comp--;
        return true;
    }
    int size(int x) { return sz[find(x)]; }
};


int main(){
    int n,k;
    cin>>n>>k;
    DSU dsu(n);
    while(k--){
        int a,b;
        cin>>a>>b;
        dsu.unite(a,b); // uniting the two nodes a and b, if they are not already in the same component
    }
    vector<int> component_sizes;
    for(int i=1; i<=n; i++) if(dsu.find(i) == i) component_sizes.push_back(dsu.size(i)); // finding the size of each component

    bitset<100005>dp;
    dp[0] = 1;
    for(int sz: component_sizes) dp |= (dp << sz); // updating the dp
    for(int i=1; i<=n; i++){
        if(dp[i]) cout<<1;
        else cout<<0;
    }
    
}