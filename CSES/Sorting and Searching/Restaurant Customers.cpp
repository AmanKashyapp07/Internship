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
    int n;
    cin >> n;

    vector<pair<int, int>> events;

    for (int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;

        events.push_back({a, 1});   // arrival
        events.push_back({b, -1});  // leaving
    }

    sort(events.begin(), events.end());

    int current = 0;
    int answer = 0;

    for (auto [time, change] : events) {
        current += change;
        answer = max(answer, current);
    }

    cout << answer << '\n';
    return 0;
}
