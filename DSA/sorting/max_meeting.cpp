/**
 * Maximum Meetings in One Room
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

    vector<int> start(n), finish(n);

    for (int i = 0; i < n; i++) cin >> start[i];
    for (int i = 0; i < n; i++) cin >> finish[i];

    vector<array<int, 3>> meetings;

    for (int i = 0; i < n; i++) {
        meetings.push_back({finish[i], start[i], i + 1});
    }

    sort(all(meetings));

    vector<int> ans;

    int lastEnd = -1;

    for (auto &[end, startTime, idx] : meetings) {
        if (startTime > lastEnd) {
            ans.push_back(idx);
            lastEnd = end;
        }
    }

    cout << ans.size() << '\n';

    for (int idx : ans) {
        cout << idx << ' ';
    }

    cout << '\n';

    return 0;
}