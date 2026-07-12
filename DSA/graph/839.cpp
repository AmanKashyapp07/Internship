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

class DSU {
    vector<int> p, sz;
public:
    DSU(int n) {
        p.resize(n);
        sz.assign(n, 1);
        iota(p.begin(), p.end(), 0);
    }

    int find(int x) {
        return p[x] == x ? x : p[x] = find(p[x]);
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
    }
};

class Solution {
public:
    bool isSimilar(string &a, string &b) {
        int diff = 0;
        for (int i = 0; i < a.size(); i++) {
            if (a[i] != b[i]) {
                diff++;
                if (diff > 2) return false;
            }
        }
        return diff == 0 || diff == 2;
    }

    int numSimilarGroups(vector<string>& strs) {
        int n = strs.size();
        DSU dsu(n);

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (isSimilar(strs[i], strs[j])) {
                    dsu.unite(i, j);
                }
            }
        }

        unordered_set<int> groups;
        for (int i = 0; i < n; i++) {
            groups.insert(dsu.find(i));
        }

        return groups.size();
    }
};