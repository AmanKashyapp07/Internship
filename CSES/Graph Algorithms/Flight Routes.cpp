/**
 * CSES Problem Set
 *
 * Problem: Flight Routes
 * Link: https://cses.fi/problemset/task/1196
 * Category: Graph Algorithms
 *
 * Description:
 * Find the k shortest route costs from city 1 to city n.
 * Routes may revisit cities multiple times.
 *
 * Logic/Approach:
 * Modified Dijkstra:
 *
 * - Maintain a min-heap storing (distance, node).
 * - Let cnt[u] = number of times node u has been popped.
 * - A node is processed at most k times.
 * - Since states are popped in increasing distance order,
 *   the first k times node n is popped correspond to the
 *   k shortest route costs from 1 to n.
 *
 * Complexity:
 * O(k * m * log(k * m))
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

struct Edge {
    int to;
    ll cost;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, k;
    cin >> n >> m >> k;

    vector<vector<Edge>> graph(n + 1);

    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;

        graph[a].push_back({b, c});
    }

    // (distance, node)
    priority_queue<
        pll,
        vector<pll>,
        greater<pll>
    > pq; // this pq holds (distance, node) pairs, and is a min-heap based on distance, so that we can always process the node with the smallest distance next, which is essential for Dijkstra's algorithm to work correctly, as it ensures that we are always expanding the shortest path first, and since we are looking for k shortest paths, we will allow each node to be processed up to k times, and we will keep track of how many times each node has been processed using the cnt array, and when we pop a node from the priority queue, if it is the destination node n, we will record its distance as one of the k shortest paths, and if we have already recorded k paths, we will stop processing further.

    vector<int> cnt(n + 1, 0);
    vector<ll> answer;

    pq.push({0, 1});

    while (!pq.empty()) {
        auto [dist, node] = pq.top();
        pq.pop();

        // Already processed k shortest paths to this node
        if (cnt[node] >= k) {
            continue;
        }

        cnt[node]++;

        // Record shortest paths reaching destination
        if (node == n) {
            answer.push_back(dist);

            if ((int)answer.size() == k) {
                break;
            }
        }

        for (const auto &edge : graph[node]) {
            pq.push({dist + edge.cost, edge.to});
        }
    }

    for (int i = 0; i < k; i++) {
        cout << answer[i] << " ";
    }
    cout << '\n';

    return 0;
}