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

class DSU
{
    vector<int> parent, rank;

public:
    DSU(int n)
    {
        parent.resize(n);
        rank.assign(n, 0);

        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int find(int x)
    {
        if (parent[x] == x)
            return x;
        return parent[x] = find(parent[x]);
    }

    bool unite(int u, int v)
    {
        u = find(u);
        v = find(v);

        if (u == v)
            return false;

        if (rank[u] < rank[v])
            swap(u, v);

        parent[v] = u;

        if (rank[u] == rank[v])
            rank[u]++;

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