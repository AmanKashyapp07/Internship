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