#include <bits/stdc++.h>
using namespace std;

#define int long long

struct maxWeightonPath{
    static const int N = 200005;
    static const int LOG = 20;

    vector<pair<int,int>> g[N];
    int depth[N], up[N][LOG], mx[N][LOG];

    void dfs(int u, int p, int w){
        up[u][0] = p;
        mx[u][0] = w;

        for(auto [v,wt] : g[u]){
            if(v==p) continue;
            depth[v]=depth[u]+1;
            dfs(v,u,wt);
        }
    }

    void build(int n){
        for(int j=1;j<LOG;j++){
            for(int i=1;i<=n;i++){
                up[i][j]=up[up[i][j-1]][j-1];
                mx[i][j]=max(mx[i][j-1],mx[up[i][j-1]][j-1]);
            }
        }
    }

    int maxEdge(int u,int v){
        int ans=0;

        if(depth[u]<depth[v]) swap(u,v);

        int d=depth[u]-depth[v];

        for(int j=LOG-1;j>=0;j--){
            if(d&(1LL<<j)){
                ans=max(ans,mx[u][j]);
                u=up[u][j];
            }
        }

        if(u==v) return ans;

        for(int j=LOG-1;j>=0;j--){
            if(up[u][j]!=up[v][j]){
                ans=max(ans,mx[u][j]);
                ans=max(ans,mx[v][j]);
                u=up[u][j];
                v=up[v][j];
            }
        }

        return max({ans,mx[u][0],mx[v][0]});
    }
};

struct DSU{
    vector<int> p,sz;
    int comp;

    DSU(int n):p(n+1),sz(n+1,1),comp(n){
        iota(p.begin(),p.end(),0);
    }

    int find(int x){
        return p[x]==x?x:p[x]=find(p[x]);
    }

    bool unite(int a,int b){
        if((a=find(a))==(b=find(b))) return false;
        if(sz[a]<sz[b]) swap(a,b);
        p[b]=a;
        sz[a]+=sz[b];
        comp--;
        return true;
    }
};

struct Edge{
    int u,v,w,id;
    bool inMST=false;

    bool operator<(const Edge& other)const{
        return w<other.w;
    }
};

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,m;
    cin>>n>>m;

    vector<Edge> edges(m);

    for(int i=0;i<m;i++){
        cin>>edges[i].u>>edges[i].v>>edges[i].w;
        edges[i].id=i;
    }

    vector<Edge> ord=edges;
    sort(ord.begin(),ord.end());

    DSU dsu(n);
    maxWeightonPath LCA;

    int mst=0;

    for(auto &e:ord){
        if(dsu.unite(e.u,e.v)){
            mst+=e.w;
            e.inMST=true;

            LCA.g[e.u].push_back({e.v,e.w});
            LCA.g[e.v].push_back({e.u,e.w});
        }
    }

    vector<int> isMST(m,0);

    for(auto &e:ord)
        if(e.inMST)
            isMST[e.id]=1;

    LCA.depth[1]=0;
    LCA.dfs(1,0,0);
    LCA.build(n);

    vector<int> ans(m);

    for(auto &e:edges){
        if(isMST[e.id]){
            ans[e.id]=mst;
        }else{
            int mx=LCA.maxEdge(e.u,e.v);
            ans[e.id]=mst+e.w-mx;
        }
    }

    for(int x:ans)
        cout<<x<<"\n";
}