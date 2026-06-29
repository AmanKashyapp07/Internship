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

class DSU{
    vector<int> parent, rank;
public:
    DSU(int n) : parent(n), rank(n, 1) {
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    void union_sets(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            if (rank[rootX] < rank[rootY]) {
                swap(rootX, rootY);
            }
            parent[rootY] = rootX;  
        }
    }
};

class Solution {
public:
    bool isSimilar(const string& a, const string& b) {
        int diffCount = 0;
        for (int i = 0; i < a.size(); ++i) {
            if (a[i] != b[i]) {
                diffCount++;
                if (diffCount > 2) return false;
            }
        }
        return diffCount == 0 || diffCount == 2;
    }
    int numSimilarGroups(vector<string>& strs) {
        DSU dsu(strs.size());

        for(int i = 0; i < strs.size(); ++i) {
            for(int j = i + 1; j < strs.size(); ++j) {
                if(isSimilar(strs[i], strs[j])) {
                    dsu.union_sets(i, j);
                }
            }
        }
        
        unordered_set<int> groups;
        for(int i = 0; i < strs.size(); ++i) {
            groups.insert(dsu.find(i));
        }
        return groups.size();
    }
};