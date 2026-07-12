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

class DSU {

    vector<int> p, sz;

public:

    DSU(int n) : p(n), sz(n, 1) {
        for (int i = 0; i < n; i++) p[i] = i;
    }

    int find(int x) {

        return p[x] == x ? x : p[x] = find(p[x]);

    }

    bool unite(int a, int b) {

        a = find(a), b = find(b);

        if (a == b) return false;

        if (sz[a] < sz[b]) swap(a, b);

        p[b] = a;

        sz[a] += sz[b];

        return true;

    }

};

class Solution
{
public:
    int earliestAcq(vector<vector<int>> &logs, int n)
    {
        sort(logs.begin(), logs.end());

        DSU dsu(n);
        int components = n;

        for (auto &log : logs)
        {
            int time = log[0];
            int u = log[1];
            int v = log[2];

            if (dsu.unite(u, v))
            {
                components--;

                if (components == 1)
                    return time;
            }
        }

        return -1;
    }
};