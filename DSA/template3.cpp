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
#include <cstring>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;
const ll P = 31;

// Helper function to build a cycle from a parent array (backtracks from start back to start)
vector<int> buildCycle(int start, const vector<int>& parent) {
    vector<int> cycle;
    int cur = start;
    while (true) {
        cycle.push_back(cur);
        cur = parent[cur];
        if (cur == start) break;
    }
    return cycle;
}

/*=============================================================================
    1. UNDIRECTED GRAPH CYCLE RECONSTRUCTION (1-indexed)
=============================================================================*/
class UndirectedCycle {
private:
    int n, startNode = -1, endNode = -1;
    vector<vector<int>> adj;
    vector<int> vis, parent;

    bool dfs(int u, int p) {
        vis[u] = 1;
        for (int v : adj[u]) {
            if (v == p) continue; 
            if (!vis[v]) {
                parent[v] = u;
                if (dfs(v, u)) return true;
            } else { 
                startNode = v; endNode = u;
                return true;
            }
        }
        return false;
    }

public:
    UndirectedCycle(int n) : n(n), adj(n + 1), vis(n + 1, 0), parent(n + 1, -1) {}
    
    void addEdge(int u, int v) {
        adj[u].push_back(v); adj[v].push_back(u);
    }

    vector<int> getCycle() {
        for (int i = 1; i <= n; i++) {
            if (!vis[i] && dfs(i, -1)) {
                return buildCycle(startNode, parent);
            }
        }
        return {};
    }
};

/*=============================================================================
    2. DIRECTED GRAPH CYCLE RECONSTRUCTION (1-indexed)
=============================================================================*/
class DirectedCycle {
private:
    int n, startNode = -1, endNode = -1;
    vector<vector<int>> adj;
    vector<bool> vis, inPath;
    vector<int> parent;

    bool dfs(int u) {
        vis[u] = inPath[u] = true;
        for (int v : adj[u]) {
            if (!vis[v]) {
                parent[v] = u;
                if (dfs(v)) return true;
            } else if (inPath[v]) {   
                startNode = v; endNode = u;
                return true;
            }
        }
        inPath[u] = false;
        return false;
    }

public:
    DirectedCycle(int n) : n(n), adj(n + 1), vis(n + 1, false), inPath(n + 1, false), parent(n + 1, -1) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    vector<int> getCycle() {
        for (int i = 1; i <= n; i++) {
            if (!vis[i] && dfs(i)) {
                return buildCycle(startNode, parent);
            }
        }
        return {};
    }
};

/*=============================================================================
    3. NEGATIVE WEIGHT CYCLE FINDING (Bellman-Ford, 1-indexed)
=============================================================================*/
struct Edge { 
    int u, v; 
    ll w; 
};

vector<int> findNegativeCycle(int n, const vector<Edge>& edges) {
    vector<ll> dist(n + 1, 0); 
    vector<int> parent(n + 1, -1);
    int lastRelaxedNode = -1;
    
    for (int i = 1; i <= n; i++) {
        lastRelaxedNode = -1;
        for (const auto &e : edges) {
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w; 
                parent[e.v] = e.u; 
                lastRelaxedNode = e.v; 
            }
        }
    }
    if (lastRelaxedNode == -1) return {}; 
    
    for (int i = 0; i < n; i++) {
        lastRelaxedNode = parent[lastRelaxedNode];
    }
    return buildCycle(lastRelaxedNode, parent);
}

/*=============================================================================
    4. SHORTEST CYCLE LENGTH / GIRT (BFS, 0-indexed)
=============================================================================*/
int findShortestCycle(int n, const vector<vector<int>>& adj) {
    int minCycleLen = INT_MAX;
    
    for (int src = 0; src < n; src++) {
        vector<int> dist(n, -1), parent(n, -1); 
        queue<int> q;
        dist[src] = 0; 
        q.push(src);
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1; 
                    parent[v] = u; 
                    q.push(v);
                } else if (parent[u] != v) { 
                    minCycleLen = min(minCycleLen, dist[u] + dist[v] + 1);
                }
            }
        }
    }
    return minCycleLen == INT_MAX ? -1 : minCycleLen;
}

/*=============================================================================
    5. IDENTIFY ALL CYCLIC DEPENDENCY NODES (Kahn's Peeling, 0-indexed)
=============================================================================*/
vector<int> getNodesInCycles(int n, const vector<vector<int>>& adj, vector<int>& indegree) {
    queue<int> q; 
    vector<bool> isAcyclic(n, false); 
    vector<int> cyclicNodes;
    
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) q.push(i);
    }
    
    while (!q.empty()) {
        int u = q.front(); q.pop(); 
        isAcyclic[u] = true;
        for (int v : adj[u]) {
            if (--indegree[v] == 0) q.push(v);
        }
    }
    
    for (int i = 0; i < n; i++) {
        if (!isAcyclic[i]) cyclicNodes.push_back(i);
    }
    return cyclicNodes;
}

bool dfs_bipartite(int u, int color, vector<int>& colors, const vector<vector<int>>& adj) {
    colors[u] = color;
    for (int v : adj[u]) {
        if (colors[v] == -1) {
            if (!dfs_bipartite(v, 1 - color, colors, adj)) return false;
        } else if (colors[v] == color) {
            return false;
        }
    }
    return true;
}

vector<int> shortestPathDAG(int n, vector<vector<pair<int,int>>>& adj, int src) {
    vector<int> indeg(n);

    for (int u = 0; u < n; u++) {
        for (auto [v, wt] : adj[u])
            indeg[v]++;
    }

    vector<int> dist(n, INT_MAX);
    dist[src] = 0;

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (indeg[i] == 0)
            q.push(i);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (auto [v, wt] : adj[u]) {
            if (dist[u] != INT_MAX && dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
            indeg[v]--;
            if (indeg[v] == 0)
                q.push(v);
        }
    }

    return dist;
}

int rangeBitwiseAnd(int left, int right) {
    int shift = 0;
    while (left < right) {
        left >>= 1;
        right >>= 1;
        shift++;
    }
    return left << shift;
}

int totalHammingDistance(const vector<int>& nums) {
    int total = 0, n = nums.size();
    for (int bit = 0; bit < 32; bit++) {
        int countOnes = 0;
        for (int num : nums) {
            if (num & (1 << bit)) countOnes++;
        }
        total += countOnes * (n - countOnes);
    }
    return total;
}

int longestNiceSubarray(const vector<int>& nums) {
    int n = nums.size();
    int left = 0, maxLength = 0;
    int mask = 0;
    for(int i=0; i<n; i++) {
        while((mask & nums[i]) != 0) {
            mask ^= nums[left];
            left++;
        }
        mask |= nums[i];
        maxLength = max(maxLength, i - left + 1);
    }
    return maxLength;
}

int subarrayBitwiseORs(vector<int>& arr) {
        unordered_set<int>s1;
        unordered_set<int>s2;
        for(auto c:arr){
            unordered_set<int>s3;
            s3.insert(c);
            for(auto v:s2) s3.insert(c|v);
            for(auto x:s3) s1.insert(x);
            s2=s3;
        }
        return s1.size();
}

int countTotalSetBits(int n) {
   int total_ones = 0;
    for (int i = 0; i < 60; i++) {
        int lengthCycle = 1LL << (i + 1);
        int totalCycles = (n + 1) / lengthCycle;
        int one = 1LL << i;
        int zeroes = 1LL << i; 
        total_ones += totalCycles * one;
        int remainder = (n + 1) % lengthCycle;
        if (remainder > zeroes) {
            total_ones += (remainder - zeroes);
        }
    }
    cout << total_ones << "\n";
    return total_ones;
}

int CountSubsetsWithSumK(const vector<int>& nums, int k) {
    int n = nums.size();
    vector<int> dp(k + 1, 0);
    dp[0] = 1;

    for (int num : nums) {
        for (int j = k; j >= num; j--) {
            dp[j] += dp[j - num];
        }
    }

    return dp[k];
}

int minSubsetSumDifference(const vector<int>& nums) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    int n = nums.size();
    vector<bool> dp(totalSum / 2 + 1, false);
    dp[0] = true;

    for (int num : nums) {
        for (int j = totalSum / 2; j >= num; j--) {
            dp[j] = dp[j] || dp[j - num];
        }
    }

    for (int j = totalSum / 2; j >= 0; j--) {
        if (dp[j]) {
            return totalSum - 2 * j;
        }
    }

    return totalSum;
}

vector<int> getMoneySums(vector<int>& coins) {
    int n = coins.size();
    if (n == 0) return {};
    
    int total = 0;
    for (int c : coins) {
        total += c;
    }
    
    vector<bool> dp(total + 1, false);
    dp[0] = true;
    
    for (int coin : coins) {
        for (int sum = total; sum >= coin; sum--) {
            if (dp[sum - coin]) {
                dp[sum] = true;
            }
        }
    }
        
    vector<int> possibleSums;
    for (int s = 1; s <= total; s++) {
        if (dp[s]) {
            possibleSums.push_back(s);
        }
    }
        
    return possibleSums;
}

int CountOfLIS(const vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;

    vector<int> length(n, 1), count(n, 1);
    int maxLength = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[i] > nums[j]) {
                if (length[j] + 1 > length[i]) {
                    length[i] = length[j] + 1;
                    count[i] = count[j];
                } else if (length[j] + 1 == length[i]) {
                    count[i] += count[j];
                }
            }
        }
        maxLength = max(maxLength, length[i]);
    }

    int totalCount = 0;
    for (int i = 0; i < n; i++) {
        if (length[i] == maxLength) {
            totalCount += count[i];
        }
    }

    return totalCount;
} // this counts total number of increasing subsequences in an array, time compelxity is 

vector<int> dijkstra(int n, const vector<vector<pair<int,int>>>& adj, int src) {
    vector<int> dist(n, INT_MAX);
    dist[src] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, wt] : adj[u]) {
            if (dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

vector<int> bellman_ford(int n, const vector<Edge>& edges, int src) {
    vector<long long> dist(n + 1, LLONG_MAX);
    dist[src] = 0;

    for (int i = 1; i <= n - 1; i++) {
        for (const auto& e : edges) {
            if (dist[e.u] != LLONG_MAX && dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
            }
        }
    }

    for (const auto& e : edges) {
        if (dist[e.u] != LLONG_MAX && dist[e.u] + e.w < dist[e.v]) {
            throw runtime_error("Graph contains a negative weight cycle");
        }
    }

    return vector<int>(dist.begin() + 1, dist.end());
}

vector<int> shortestPathUnweighted(int n, const vector<vector<int>>& adj, int src) {
    vector<int> dist(n, INT_MAX);
    dist[src] = 0;
    queue<int> q;
    q.push(src);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == INT_MAX) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    return dist;
}

vector<vector<int>> floyd_warshall(int n, const vector<vector<int>>& graph) {
    vector<vector<int>> dist = graph;
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    return dist;
}

bool cycle_detection_floyd(int n, const vector<vector<int>>& graph) {
    vector<vector<int>> dist = graph;
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        if (dist[i][i] < 0) return true;
    }
    return false;
}

vector<int> lexicographical_topo_sort(int n, const vector<vector<int>>& adj, vector<int> indegree) {
    priority_queue<int> pq;

    for (int i = 1; i <= n; i++) {
        if (indegree[i] == 0)
            pq.push(i);
    }

    vector<int> order;

    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();

        order.push_back(u);

        for (int v : adj[u]) {
            if (--indegree[v] == 0)
                pq.push(v);
        }
    }

    if ((int)order.size() != n)
        return {};

    reverse(order.begin(), order.end());
    return order;
}

void DPonDAG(int n, const vector<vector<pair<int,int>>>& adj, int src) {
    vector<int> indeg(n, 0);
    for (int u = 0; u < n; u++) {
        for (auto [v, wt] : adj[u]) {
            indeg[v]++;
        }  
    }
    vector<int> topo_order;
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) q.push(i);
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        topo_order.push_back(u);
        for (auto [v, wt] : adj[u]) {
            if (--indeg[v] == 0) q.push(v);
        }
    }

    vector<int> dist(n, INT_MAX);
    dist[src] = 0;
    for(auto u : topo_order) {
        if(dist[u] != INT_MAX) {
            for(auto [v, wt] : adj[u]) {
                if(dist[u] + wt < dist[v]) {
                    dist[v] = dist[u] + wt;
                }
            }
        }
    }
}

struct GraphMColouring {
    bool isValid(int node, int color, const vector<vector<int>>& adj, const vector<int>& colors) {
        for (int neighbor : adj[node]) {
            if (colors[neighbor] == color) return false;
        }
        return true;
    }
    void dfs(int node, int m, const vector<vector<int>>& adj, vector<int>& colors, int& count) {
        if (node == adj.size()) {
            count++;
            return;
        }
        for (int color = 1; color <= m; color++) {
            if (isValid(node, color, adj, colors)) {
                colors[node] = color;
                dfs(node + 1, m, adj, colors, count);
                colors[node] = 0;
            }
        }
    }
    int countColorings(int n, int m, const vector<vector<int>>& adj) {
        vector<int> colors(n, 0);
        int count = 0;
        dfs(0, m, adj, colors, count);
        return count;
    }
};

int findCheapestPrice(int n, vector<vector<int>> &flights, int src, int dst, int k) {
    vector<vector<pair<int, int>>> adj(n);
    for (auto &f : flights) adj[f[0]].push_back({f[1], f[2]});
    int maxFlights = k + 1;
    vector<vector<int>> dist(n, vector<int>(maxFlights + 1, INT_MAX));
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
    dist[src][0] = 0; pq.push({0, src, 0});
    while (!pq.empty()) {
        auto [cost, node, flightsTaken] = pq.top(); pq.pop();
        if (cost > dist[node][flightsTaken]) continue;
        if (node == dst) return cost;
        for (auto &[next, price] : adj[node]) {
            if (flightsTaken + 1 <= maxFlights && cost + price < dist[next][flightsTaken + 1]) {
                dist[next][flightsTaken + 1] = cost + price;
                pq.push({cost + price, next, flightsTaken + 1});
            }
        }
    }
    return -1;
}

int shortestPathVisitingAllNodes(int n, const vector<vector<int>>& graph) {
    int allVisitedMask = (1 << n) - 1;
    queue<pair<int, int>> q;
    vector<vector<int>> dist(n, vector<int>(1 << n, INT_MAX));
    
    for (int i = 0; i < n; i++) {
        q.push({i, 1 << i});
        dist[i][1 << i] = 0;
    }
    
    while (!q.empty()) {
        auto [node, mask] = q.front(); q.pop();
        int currentDist = dist[node][mask];
        
        if (mask == allVisitedMask) return currentDist;
        
        for (int neighbor : graph[node]) {
            int nextMask = mask | (1 << neighbor);
            if (dist[neighbor][nextMask] > currentDist + 1) {
                dist[neighbor][nextMask] = currentDist + 1;
                q.push({neighbor, nextMask});
            }
        }
    }
    
    return -1;
}

int shortestCycleInUndirectedGraph(int n, const vector<vector<int>>& adj) {
    int minCycleLength = INT_MAX;

    for (int start = 0; start < n; start++) {
        vector<int> dist(n, -1);
        vector<int> parent(n, -1);
        queue<int> q;

        dist[start] = 0;
        q.push(start);

        while (!q.empty()) {
            int u = q.front(); q.pop();

            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                } else if (parent[u] != v) {
                    minCycleLength = min(minCycleLength, dist[u] + dist[v] + 1);
                }
            }
        }
    }

    return minCycleLength == INT_MAX ? -1 : minCycleLength;
}

int ShortestCommonSubsequence(const string& s1, const string& s2) {
    int n = s1.size(), m = s2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    int lcsLength = dp[n][m];
    return n + m - lcsLength;
}

string SCS(const string& s1, const string& s2) {
    int n = s1.size(), m = s2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    string scs;
    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            scs += s1[i - 1];
            i--; j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            scs += s1[i - 1];
            i--;
        } else {
            scs += s2[j - 1];
            j--;
        }
    }

    while (i > 0) {
        scs += s1[i - 1];
        i--;
    }
    while (j > 0) {
        scs += s2[j - 1];
        j--;
    }

    reverse(scs.begin(), scs.end());
    return scs;
}

string minWindowSubsequence(string s, string t) {
    if(t.empty()) return "";
    int n = s.size(), m = t.size();
    int minLen = INT_MAX, startIdx = -1;

    int i=0;
    int j=0;
    while(i < n) {
        if (s[i] == t[j]) {
            j++;
            if (j == m) {
                int end = i + 1;
                j--;
                while (j >= 0) {
                    if (s[i] == t[j]) j--;
                    i--;
                }
                i++; j++;
                if (end - i < minLen) {
                    minLen = end - i;
                    startIdx = i;
                }
            }
        }
        i++;
    }

    return startIdx == -1 ? "" : s.substr(startIdx, minLen);
}

string minWindowSubstring(string s, string t) {
    if(t.empty()) return "";
    unordered_map<char, int> need, window;
    for(char c : t) need[c]++;
    int left = 0, right = 0, valid = 0;
    int start = 0, minLen = INT_MAX;

    while(right < s.size()) {
        char c = s[right];
        right++;
        if(need.count(c)) {
            window[c]++;
            if(window[c] == need[c]) valid++;
        }

        while(valid == need.size()) {
            if(right - left < minLen) {
                start = left;
                minLen = right - left;
            }
            char d = s[left];
            left++;
            if(need.count(d)) {
                if(window[d] == need[d]) valid--;
                window[d]--;
            }
        }
    }

    return minLen == INT_MAX ? "" : s.substr(start, minLen);
}

struct MatrixChainMultiplication{
    int matrixChainOrder(const vector<int>& p) {
        int n = p.size() - 1;
        vector<vector<int>> dp(n, vector<int>(n, 0));

        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                dp[i][j] = INT_MAX;
                for (int k = i; k < j; k++) {
                    int cost = dp[i][k] + dp[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
                    dp[i][j] = min(dp[i][j], cost);
                }
            }
        }

        return dp[0][n - 1];
    }    
};



/*
=========================================================
3. STRING DP — Distinct Subsequences (LeetCode 115)
=========================================================
*/
class DistinctSubsequences {
public:
    int numDistinct(string s, string t) {
        int n = s.size(), m = t.size();
        vector<unsigned long long> dp(m + 1, 0);
        dp[0] = 1;
        
        for (int i = 1; i <= n; i++) {
            for (int j = m; j >= 1; j--) {
                if (s[i - 1] == t[j - 1]) {
                    dp[j] += dp[j - 1];
                }
            }
        }
        return dp[m];
    }
}; // counts total subsequences of string s that equals string t, time complexity is O(n*m), space complexity is O(m)


struct Element{
    int key;
    int height;
    Element* left;
    Element* right;
    Element(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};



struct AVL{
    int height(Element* node) {
        return node ? node->height : 0;
    }
    int balance(Element* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }
    int update(Element* node) {
        if (node) {
            node->height = 1 + max(height(node->left), height(node->right));
        }
        return node ? node->height : 0;
    }
    Element* rightRotate(Element* y) {
        Element* x = y->left;
        Element* T2 = x->right;
        x->right = y;
        y->left = T2;
        update(y);
        update(x);
        return x;
    }
    Element* leftRotate(Element* x) {
        Element* y = x->right;
        Element* T2 = y->left;
        y->left = x;
        x->right = T2;
        update(x);
        update(y);
        return y;
    }
    Element* insert(Element* node, int key) {
        if(!node) return new Element(key);
        if(key < node->key) node->left = insert(node->left, key);
        else if(key > node->key) node->right = insert(node->right, key);
        else return node;
        update(node);
        int balanceFactor = balance(node);
        if(balanceFactor > 1 && key < node->left->key) return rightRotate(node); // RR
        if(balanceFactor < -1 && key > node->right->key) return leftRotate(node); // LL
        if(balanceFactor > 1 && key > node->left->key) { // LR
            node->left = leftRotate(node->left);
            return rightRotate(node);     
        }
        if(balanceFactor < -1 && key < node->right->key) { // RL
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }  
    Element* minValueNode(Element* node) {
        Element* current = node;
        while (current->left) current = current->left;
        return current;
    }
    void inorder(Element *root) {
        if (!root) return;
        inorder(root->left);
        cout << root->key << " ";
        inorder(root->right);

    }

    void preorder(Element *root) {
        if (!root) return;
        cout << root->key << " ";
        preorder(root->left);
        preorder(root->right);
    }

    void postorder(Element *root) {
        if (!root) return;
        postorder(root->left);
        postorder(root->right);
        cout << root->key << " ";
    }
};

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* middleNode(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }
    void deleteNode(ListNode* node) {
        if (node && node->next) { // If the node is not the last node
            node->val = node->next->val;
            ListNode* temp = node->next;
            node->next = node->next->next;
            delete temp;
        }
    }
};

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    int ans = 0;

    tuple<bool, int, int, int> dfs(TreeNode* node) {
        if (!node)
            return {true, INT_MAX, INT_MIN, 0};

        auto [leftBST, leftMin, leftMax, leftSum] = dfs(node->left);
        auto [rightBST, rightMin, rightMax, rightSum] = dfs(node->right);

        if (leftBST && rightBST &&
            node->val > leftMax &&
            node->val < rightMin) {

            int sum = leftSum + rightSum + node->val;
            ans = max(ans, sum);

            return {
                true,
                min(node->val, leftMin),
                max(node->val, rightMax),
                sum
            };
        }

        return {false, 0, 0, 0};
    }

    int maxSumBST(TreeNode* root) {
        dfs(root);
        return ans;
    }
};



//Given a binary tree root and an integer target, delete all the leaf nodes with value target.

// Note that once you delete a leaf node with value target, if its parent node becomes a leaf node and has the value target, it should also be deleted (you need to continue doing that until you cannot).

class Solution {
public:
    TreeNode* removeLeafNodes(TreeNode* root, int target) {
        if (!root) return nullptr;

        root->left = removeLeafNodes(root->left, target);
        root->right = removeLeafNodes(root->right, target);

        if (!root->left && !root->right && root->val == target)
            return nullptr;

        return root;
    }
};

class Solution {
public:
    int n;
    vector<int> dp;
    unordered_set<string> st;

    int solve(int i, string &s) {
        if (i == n)
            return 0;

        if (dp[i] != -1)
            return dp[i];

        // Take current character as extra
        int ans = 1 + solve(i + 1, s);

        string curr = "";

        // Try every substring starting from i
        for (int j = i; j < n; j++) {
            curr += s[j];

            if (st.count(curr)) {
                ans = min(ans, solve(j + 1, s));
            }
        }

        return dp[i] = ans;
    }

    int minExtraChar(string s, vector<string>& dictionary) {
        n = s.size();

        for (string &word : dictionary)
            st.insert(word);

        dp.assign(n, -1);

        return solve(0, s);
    }
}; // You are given a 0-indexed string s and a dictionary of words dictionary. You have to break s into one or more non-overlapping substrings such that each substring is present in dictionary. There may be some extra characters in s which are not present in any of the substrings.

// Return the minimum number of extra characters left over if you break up s optimally.

