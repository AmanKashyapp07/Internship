class Solution {
public:
    vector<vector<int>> result;

    void dfs(int node, vector<vector<int>>& graph, vector<int>& path) {
        path.push_back(node);

        if (node == graph.size() - 1) {
            result.push_back(path);
        } else {
            for (int nei : graph[node]) {
                dfs(nei, graph, path);
            }
        }

        path.pop_back();
    }

    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        vector<int> path;
        dfs(0, graph, path);
        return result;
    }
};