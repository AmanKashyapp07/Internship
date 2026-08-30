// Link: https://cses.fi/problemset/task/3213
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

struct Edge {
    int to_u, to_v;
    long long cost;
    string op;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b, x;
    if (!(cin >> a >> b >> x)) return 0;

    if (x > a || x % std::gcd(a, b) != 0) {
        cout << -1 << "\n";
        return 0;
    }

    vector<vector<long long>> dist(a + 1, vector<long long>(b + 1, INF));
    vector<vector<int>> moves_count(a + 1, vector<int>(b + 1, 1e9));
    vector<vector<pair<int, int>>> parent(a + 1, vector<pair<int, int>>(b + 1, {-1, -1}));
    vector<vector<string>> parent_op(a + 1, vector<string>(b + 1, ""));

    using Element = tuple<long long, int, int, int>; // {cost, num_moves, u, v}
    priority_queue<Element, vector<Element>, greater<Element>> pq;

    dist[0][0] = 0;
    moves_count[0][0] = 0;
    pq.push({0, 0, 0, 0});

    int target_u = -1, target_v = -1;

    while (!pq.empty()) {
        auto [d, steps, u, v] = pq.top();
        pq.pop();

        if (d > dist[u][v]) continue;

        if (u == x) {
            target_u = u;
            target_v = v;
            break;
        }

        vector<Edge> transitions;
        if (u < a) transitions.push_back({a, v, a - u, "FILL A"});
        if (v < b) transitions.push_back({u, b, b - v, "FILL B"});
        if (u > 0) transitions.push_back({0, v, u, "EMPTY A"});
        if (v > 0) transitions.push_back({u, 0, v, "EMPTY B"});
        if (u > 0 && v < b) {
            int pour = min(u, b - v);
            transitions.push_back({u - pour, v + pour, pour, "MOVE A B"});
        }
        if (v > 0 && u < a) {
            int pour = min(v, a - u);
            transitions.push_back({u + pour, v - pour, pour, "MOVE B A"});
        }

        for (const auto& edge : transitions) {
            if (edge.cost <= 0) continue;
            long long next_d = d + edge.cost;
            int next_steps = steps + 1;

            if (next_d < dist[edge.to_u][edge.to_v] || 
               (next_d == dist[edge.to_u][edge.to_v] && next_steps < moves_count[edge.to_u][edge.to_v])) {
                dist[edge.to_u][edge.to_v] = next_d;
                moves_count[edge.to_u][edge.to_v] = next_steps;
                parent[edge.to_u][edge.to_v] = {u, v};
                parent_op[edge.to_u][edge.to_v] = edge.op;
                pq.push({next_d, next_steps, edge.to_u, edge.to_v});
            }
        }
    }

    if (target_u == -1) {
        cout << -1 << "\n";
        return 0;
    }

    vector<string> path;
    int curr_u = target_u, curr_v = target_v;
    while (!(curr_u == 0 && curr_v == 0)) {
        path.push_back(parent_op[curr_u][curr_v]);
        auto p = parent[curr_u][curr_v];
        curr_u = p.first;
        curr_v = p.second;
    }
    reverse(path.begin(), path.end());

    cout << path.size() << " " << dist[target_u][target_v] << "\n";
    for (const string& op : path) cout << op << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find minimum water moved to achieve target x in container A and print sequence of operations (CSES 3213).
// - Approach: State Space Dijkstra on Graph (u, v) with Parent Operation Tracking.
// - Intuition: States (u, v) represent current fill levels; transitions represent pouring, filling, and emptying with edge costs equal to water moved.
// - Complexity: Time: O(A * B log(A * B)), Space: O(A * B).
