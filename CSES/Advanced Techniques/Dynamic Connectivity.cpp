#include <bits/stdc++.h>
using namespace std;

struct Bridge {
    int t;
    vector<int> tin, low;
    vector<pair<int,int>> res;

    void dfs(int u,int p,vector<vector<int>>& adj){
        tin[u]=low[u]=++t;
        for(int v:adj[u]){
            if(v==p) continue;
            if(tin[v]) low[u]=min(low[u],tin[v]);
            else{
                dfs(v,u,adj);
                low[u]=min(low[u],low[v]);
                if(low[v]>tin[u])
                    res.push_back({u,v});
            }
        }
    }

    vector<pair<int,int>> get(vector<vector<int>>& adj){
        int n=adj.size();
        t=0;
        tin.assign(n,0);
        low.assign(n,0);
        res.clear();

        for(int i=1;i<n;i++)
            if(!tin[i])
                dfs(i,-1,adj);

        return res;
    }
};

void dfs(int u,int p,vector<vector<int>>& adj,
         vector<bool>& vis,
         vector<int>& comp,
         int id){

    vis[u]=true;
    comp[u]=id;

    for(int v:adj[u]){
        if(v!=p && !vis[v])
            dfs(v,u,adj,vis,comp,id);
    }
}

int main(){

    int n,m,k;
    cin>>n>>m>>k;

    vector<vector<int>> adj(n+1);

    for(int i=0;i<m;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<pair<int,int>> bridges=Bridge().get(adj);

    vector<bool> vis(n+1,false);
    vector<int> comp(n+1);

    int components=0;

    for(int i=1;i<=n;i++){
        if(!vis[i]){
            components++;
            dfs(i,-1,adj,vis,comp,components);
        }
    }

    while(k--){

        int t,a,b;
        cin>>t>>a>>b;

        if(t==1){

            if(comp[a]==comp[b]){
                cout<<components<<"\n";
            }
            else{

                adj[a].push_back(b);
                adj[b].push_back(a);

                int oldComp=comp[b];
                int newComp=comp[a];

                for(int i=1;i<=n;i++){
                    if(comp[i]==oldComp)
                        comp[i]=newComp;
                }

                components--;

                // recompute bridges
                bridges=Bridge().get(adj);

                cout<<components<<"\n";
            }
        }
        else{

            adj[a].erase(find(adj[a].begin(),adj[a].end(),b));
            adj[b].erase(find(adj[b].begin(),adj[b].end(),a));

            // recompute components
            fill(vis.begin(),vis.end(),false);
            components=0;

            for(int i=1;i<=n;i++){
                if(!vis[i]){
                    components++;
                    dfs(i,-1,adj,vis,comp,components);
                }
            }

            // recompute bridges
            bridges=Bridge().get(adj);

            cout<<components<<"\n";
        }
    }
}