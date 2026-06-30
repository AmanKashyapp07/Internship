class Solution {
public:
    void dfs(int u, int parent, const vector<vector<int>>& adj,
             vector<int>& subtreeSize, int& answer) {
        subtreeSize[u] = 1;

        for (int v : adj[u]) {
            if (v != parent) {
                dfs(v, u, adj, subtreeSize, answer);
                subtreeSize[u] += subtreeSize[v];
            }
        }

        int flag=1;
        for(int i=1;i<adj[u].size();i++){
            if(subtreeSize[adj[u][i]]!=subtreeSize[adj[u][0]]){
                flag=0;
                break;
            }
        }
        if(flag==1){
            answer++;
        }
    }

    int countGoodNodes(vector<vector<int>>& edges) {
        int n = edges.size() + 1;
        vector<vector<int>> adj(n);

        for (auto &edge : edges) {
            int u = edge[0];
            int v = edge[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<int> subtreeSize(n, 0);
        int answer = 0;
        dfs(0, -1, adj, subtreeSize, answer);

        return answer;
    }
};