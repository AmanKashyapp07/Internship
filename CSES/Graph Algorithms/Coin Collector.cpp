/**
 * CSES 1686 - Coin Collector
 *
 * Idea:
 * 1. Find Strongly Connected Components (Kosaraju).
 * 2. Compress every SCC into one node.
 * 3. Weight of each SCC = sum of coins in that SCC.
 * 4. Build the condensed DAG.
 * 5. Find the maximum weighted path in the DAG using DP.
 *
 * Time:  O(N + M)
 * Space: O(N + M)
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class CoinCollector {
private:
    int n, m;

    vector<vector<int>> graph;
    vector<vector<int>> reverseGraph;

    vector<int> component;
    vector<int> finishOrder;
    vector<bool> visited;

    vector<long long> coins;
    vector<long long> componentWeight;

    vector<vector<int>> dag;
    vector<long long> dp;

    //---------------------------------------------------------
    // First DFS (stores finishing order)
    //---------------------------------------------------------
    void dfsFinishOrder(int node) {
        visited[node] = true;

        for (int next : graph[node]) {
            if (!visited[next])
                dfsFinishOrder(next);
        }

        finishOrder.push_back(node);
    }

    //---------------------------------------------------------
    // Second DFS (assign SCC id)
    //---------------------------------------------------------
    void dfsAssignComponent(int node, int id) {
        component[node] = id;

        for (int next : reverseGraph[node]) {
            if (component[next] == 0)
                dfsAssignComponent(next, id);
        }
    }

    //---------------------------------------------------------
    // DP on condensed DAG
    //---------------------------------------------------------
    long long solve(int node) {

        if (dp[node] != -1)
            return dp[node];

        long long best = 0;

        for (int next : dag[node]) {
            best = max(best, solve(next));
        }

        return dp[node] = componentWeight[node] + best;
    } // returns max weighted path starting from node in the condensed DAG
    // condensed means that every SCC is compressed into one node, and the edges between SCCs are preserved in the DAG

public:

    void readInput() {

        cin >> n >> m;

        graph.assign(n + 1, {});
        reverseGraph.assign(n + 1, {});
        coins.assign(n + 1, 0);

        for (int i = 1; i <= n; i++)
            cin >> coins[i];

        while (m--) {

            int u, v;
            cin >> u >> v;

            graph[u].push_back(v);
            reverseGraph[v].push_back(u);
        }
    }

    int buildSCCs() {

        visited.assign(n + 1, false);

        for (int i = 1; i <= n; i++) {
            if (!visited[i])
                dfsFinishOrder(i);
        }

        reverse(finishOrder.begin(), finishOrder.end());

        component.assign(n + 1, 0);

        int sccCount = 0;

        for (int node : finishOrder) {
            if (component[node] == 0)
                dfsAssignComponent(node, ++sccCount);
        }

        return sccCount;
    }

    void computeComponentWeights(int sccCount) {

        componentWeight.assign(sccCount + 1, 0);

        for (int i = 1; i <= n; i++) {
            componentWeight[component[i]] += coins[i];
        }
    }

    void buildCondensedGraph(int sccCount) {

        dag.assign(sccCount + 1, {});

        for (int u = 1; u <= n; u++) {

            for (int v : graph[u]) {

                if (component[u] != component[v]) {
                    dag[component[u]].push_back(component[v]);
                }
            }
        }
    }

    long long maximumCoins(int sccCount) {

        dp.assign(sccCount + 1, -1);

        long long answer = 0;

        for (int i = 1; i <= sccCount; i++) {
            answer = max(answer, solve(i));
        } // finds the maximum weighted path in the condensed DAG starting from any SCC

        return answer;
    }
};

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    CoinCollector solver;

    solver.readInput();

    int sccCount = solver.buildSCCs();

    solver.computeComponentWeights(sccCount);

    solver.buildCondensedGraph(sccCount);

    cout << solver.maximumCoins(sccCount) << '\n';
}