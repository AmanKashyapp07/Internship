/**
 * LeetCode 1101 - The Earliest Moment When Everyone Become Friends
 *
 * Description:
 * There are n people and a list of logs containing `[timestamp, personA, personB]`.
 * Friends relationship is transitive. Return the earliest timestamp when everyone is connected.
 *
 * Approach:
 * - Sort the logs chronologically by timestamp.
 * - Initialize a Disjoint Set Union (DSU) of size n.
 * - Keep a counter of connected components, starting at n.
 * - Iterate through logs, unioning the two people.
 * - If a union succeeds, decrement the components count.
 * - Return the timestamp when components count becomes 1.
 *
 * Time Complexity: O(L * log(L) + L * alpha(V)) where L is the number of logs.
 * Space Complexity: O(n)
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct DSU {
    vector<int> p, sz;
    int comp;
    DSU(int n) : p(n), sz(n, 1), comp(n) { iota(p.begin(), p.end(), 0); }

    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }

    bool unite(int a, int b) {
        if ((a = find(a)) == (b = find(b))) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        comp--;
        return true;
    }

    int size(int x) { return sz[find(x)]; }
};

class Solution {
public:
    int earliestAcq(vector<vector<int>>& logs, int n) {
        sort(logs.begin(), logs.end());

        DSU dsu(n);

        for (auto &log : logs) {
            int t = log[0];
            int u = log[1];
            int v = log[2];

            dsu.unite(u, v);

            if (dsu.comp == 1)
                return t;
        }

        return -1;
    }
};