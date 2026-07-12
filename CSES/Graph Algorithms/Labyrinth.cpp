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

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> grid(n);

    pair<int, int> start, end;

    for (int i = 0; i < n; i++)
    {
        cin >> grid[i];

        for (int j = 0; j < m; j++)
        {
            if (grid[i][j] == 'A') start = {i, j};
            if (grid[i][j] == 'B') end = {i, j};
        }
    }

    vector<vector<bool>> vis(n, vector<bool>(m, false));
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(m, {-1, -1}));
    vector<vector<char>> move_taken(n, vector<char>(m));

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    char dir[] = {'U', 'D', 'L', 'R'};

    queue<pair<int, int>> q;
    q.push(start);
    vis[start.first][start.second] = true;

    while (!q.empty())
    {
        auto [r, c] = q.front();
        q.pop();
        for (int k = 0; k < 4; k++)
        {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
            if (vis[nr][nc] || grid[nr][nc] == '#') continue;
            vis[nr][nc] = true;
            parent[nr][nc] = {r, c};
            move_taken[nr][nc] = dir[k]; // store the move taken to reach (nr, nc) from (r, c)
            q.push({nr, nc});
        }
    }

    if (!vis[end.first][end.second])
    {
        cout << "NO\n";
        return 0;
    } // if we cannot reach the end, print NO and exit

    string path;

    auto [r, c] = end; // start from the end and backtrack to the start using the parent array, while also recording the moves taken

    while (make_pair(r, c) != start)
    {
        path.push_back(move_taken[r][c]); // append the move taken to reach (r, c) to the path
        auto [pr, pc] = parent[r][c]; // move to the parent cell
        r = pr;
        c = pc;
    }

    reverse(path.begin(), path.end());

    cout << "YES\n";
    cout << path.size() << '\n';
    cout << path << '\n';
}
