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
// problem is saying to maximize the number of meetings that can be held in a single room, given the start and finish times of each meeting. The approach is to sort the meetings by their finish times and then select meetings that start after the last selected meeting's finish time. This greedy approach ensures that we can accommodate the maximum number of meetings in the room.
    for (int i = 0; i < n; i++) {
        meetings.push_back({finish[i], start[i], i + 1});
    }

    sort(all(meetings)); // sort meetings by finish time, if finish time is same then by start time, if start time is also same then by index, because we want to select meetings that finish earlier so that we can accommodate more meetings in the room.

    vector<int> ans; // represents the indices of the selected meetings that can be held in the room. We will iterate through the sorted meetings and select those that start after the last selected meeting's finish time, ensuring that we maximize the number of meetings in the room.

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