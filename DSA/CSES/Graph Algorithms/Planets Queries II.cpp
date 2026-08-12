// Link: https://cses.fi/problemset/task/1160

#include <bits/stdc++.h>
using namespace std;

#define int long long
using vi = vector<int>;


// Returns empty vector if 'start' leads into an already processed component
vi getCycleFloyd(const vi &to, const vi &comp, int start)
{
    int slow = start, fast = start;

    while (true)
    {
        slow = to[slow];
        fast = to[to[fast]];

        if (comp[slow] != -1)
            return {};

        if (slow == fast)
            break;
    }

    slow = start;
    while (slow != fast)
    {
        slow = to[slow];
        fast = to[fast];
    }

    vi cycle;
    int cur = slow;
    while (true)
    {
        cycle.push_back(cur);
        cur = to[cur];
        if (cur == slow)
            break;
    }

    return cycle;
}

struct FunctionalGraph
{
    int n;
    static const int LOG = 20;

    vi dist, cyc, comp, pos;
    vector<vi> up;

    void dfs(int u, const vi &to, const vector<vi> &radj)
    {
        for (int v : radj[u])
        {
            if (dist[v] != -1)
                continue; // already visited (cycle or processed)

            dist[v] = dist[u] + 1;
            comp[v] = comp[u];

            dfs(v, to, radj);
        }
    }

    FunctionalGraph(const vi &to)
    {
        n = to.size();

        dist.assign(n, -1);
        cyc.assign(n, -1);
        comp.assign(n, -1);
        pos.assign(n, -1);

        up.assign(n, vi(LOG));

        for (int i = 0; i < n; i++)
            up[i][0] = to[i];

        for (int j = 1; j < LOG; j++)
            for (int i = 0; i < n; i++)
                up[i][j] = up[up[i][j - 1]][j - 1];

        vector<vi> radj(n);
        for (int i = 0; i < n; i++)
            radj[to[i]].push_back(i);

        int comp_id = 0;

        for (int i = 0; i < n; i++)
        {
            if (comp[i] != -1)
                continue;

            vi cycle = getCycleFloyd(to, comp, i);

            if (cycle.empty())
                continue;

            int len = cycle.size();

            for (int j = 0; j < len; j++)
            {
                int u = cycle[j];
                dist[u] = 0;
                cyc[u] = len;
                comp[u] = comp_id;
                pos[u] = j;
            }

            // process reverse trees
            for (int u : cycle)
                dfs(u, to, radj);

            comp_id++;
        }
    }

    int jump(int u, int k)
    {
        for (int j = 0; j < LOG; j++)
            if ((k >> j) & 1)
                u = up[u][j];

        return u;
    }

    int dis_cycle_nodes(int a, int b)
    {
        if (comp[a] != comp[b])
            return -1;

        if (dist[a] != 0 || dist[b] != 0)
            return -1;

        return (pos[b] - pos[a] + cyc[a]) % cyc[a];
    }

    int query(int a, int b)
    {
        if (comp[a] != comp[b])
            return -1;

        bool ac = (dist[a] == 0);
        bool bc = (dist[b] == 0);

        if (ac && bc)
            return dis_cycle_nodes(a, b);

        if (ac && !bc)
            return -1;

        if (!ac && !bc)
        {
            if (dist[a] < dist[b])
                return -1;

            int d = dist[a] - dist[b];

            return (jump(a, d) == b) ? d : -1;
        }

        int entry = jump(a, dist[a]);
        return dist[a] + dis_cycle_nodes(entry, b);
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vi to(n);

    for (int i = 0; i < n; i++)
    {
        cin >> to[i];
        --to[i];
    }

    FunctionalGraph fg(to);

    while (q--)
    {
        int a, b;
        cin >> a >> b;
        --a;
        --b;

        cout << fg.query(a, b) << '\n';
    }
}