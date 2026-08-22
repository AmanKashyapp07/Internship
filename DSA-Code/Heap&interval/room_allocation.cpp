/**
 * CSES Problem Set
 *
 * Problem: Room Allocation
 * Link: https://cses.fi/problemset/task/1164
 */

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

    int m=3;
    vector<vector<int>> a(n, vector<int>(m));

    for (int i = 0; i < n; i++) {
        cin >> a[i][0] >> a[i][1];
        a[i][2] = i;
    }

    sort(all(a)); // sort by start time, then by end time, then by index

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // min-heap of (end time, room number)
    vector<int> ans(n);

    int rooms = 0;

    for (auto &v : a) {
        int l = v[0], r = v[1], i = v[2];
        int room;

        if (!pq.empty() && pq.top().first < l) { // if the earliest ending room is free before the current meeting starts
            room = pq.top().second; // reuse that room
            pq.pop(); // remove it from the heap since we are going to update its end time
        } else {
            rooms++; // need a new room
            room = rooms; // assign the new room number
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