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

    int n;
    cin >> n;

    vector<array<int, 3>> a(n);

    for (int i = 0; i < n; i++) {
        cin >> a[i][0] >> a[i][1];
        a[i][2] = i;
    }

    sort(all(a));

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> ans(n);

    int rooms = 0;

    for (auto &[l, r, i] : a) {
        int room;

        if (!pq.empty() && pq.top().first < l) {
            room = pq.top().second;
            pq.pop();
        } else {
            rooms++;
            room = rooms;
        }

        ans[i] = room;
        pq.push({r, room});
    }

    cout << rooms << '\n';

    for (int x : ans) {
        cout << x << ' ';
    }

    return 0;
}
