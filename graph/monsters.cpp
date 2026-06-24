

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



int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> g(n);
    for (auto &row : g) cin >> row;

    const int INF = 1e9;
    vector<vector<int>> md(n, vector<int>(m, INF));
    queue<pair<int,int>> q;

    pair<int,int> start;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (g[i][j] == 'M') {
                md[i][j] = 0;
                q.push({i, j});
            }
            if (g[i][j] == 'A')
                start = {i, j};
        }
    }

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    // Multi-source BFS from monsters
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k];
            int nc = c + dc[k];

            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
            if (g[nr][nc] == '#') continue;
            if (md[nr][nc] != INF) continue;

            md[nr][nc] = md[r][c] + 1;
            q.push({nr, nc});
        }
    }

    vector<vector<int>> dist(n, vector<int>(m, INF));
    vector<vector<pair<int,int>>> par(n, vector<pair<int,int>>(m, {-1, -1}));
    vector<vector<char>> moveDir(n, vector<char>(m));

    q.push(start);
    dist[start.first][start.second] = 0;

    pair<int,int> end = {-1, -1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (r == 0 || r == n - 1 || c == 0 || c == m - 1) {
            end = {r, c};
            break;
        }

        char dir[] = {'U', 'D', 'L', 'R'};

        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k];
            int nc = c + dc[k];

            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
            if (g[nr][nc] == '#') continue;
            if (dist[nr][nc] != INF) continue;

            int nd = dist[r][c] + 1;

            if (nd >= md[nr][nc]) continue; // If the monster can reach this cell before or at the same time as the player, skip it.

            dist[nr][nc] = nd;
            par[nr][nc] = {r, c};
            moveDir[nr][nc] = dir[k];
            q.push({nr, nc});
        }
    }

    if (end.first == -1) {
        cout << "NO\n";
        return 0;
    }

    string path;
    auto [r, c] = end;

    while (make_pair(r, c) != start) {
        path.push_back(moveDir[r][c]);
        auto [pr, pc] = par[r][c];
        r = pr;
        c = pc;
    }

    reverse(path.begin(), path.end());

    cout << "YES\n";
    cout << path.size() << '\n';
    cout << path << '\n';
}