/**
 * CSES Problem Set - Interval Templates
 *
 * 1. Meeting Rooms II
 *    - Minimum rooms required
 *    - Sort by start time + Min-heap of end times
 *
 * 2. Activity Selection
 *    - Maximum number of non-overlapping intervals
 *    - Sort by end time + Greedy
 *
 * 3. Room Allocation (CSES)
 *    - Assign rooms to customers
 *    - Minimize total rooms used
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

// ─────────────────────────────────────────────────────────────────────────────
// MEETING ROOMS II
// Return the minimum number of conference rooms required.
// ─────────────────────────────────────────────────────────────────────────────
int minMeetingRooms(vector<pii> intervals) {
    if (intervals.empty()) return 0;

    // Sort by start time
    sort(all(intervals));

    // Min-heap to store end times of meetings in progress
    priority_queue<int, vector<int>, greater<int>> pq;

    int maxRooms = 0;

    for (auto& interval : intervals) {
        int start = interval.first;
        int end = interval.second;

        // If a room is free, reuse it
        if (!pq.empty() && pq.top() <= start) {
            pq.pop();
        }

        pq.push(end);
        maxRooms = max(maxRooms, (int)pq.size());
    }

    return maxRooms;
}

// ─────────────────────────────────────────────────────────────────────────────
// ACTIVITY SELECTION
// Return maximum number of non-overlapping activities.
// ─────────────────────────────────────────────────────────────────────────────
int maxActivities(vector<pii> intervals) {
    if (intervals.empty()) return 0;

    // Sort by ending time
    sort(all(intervals), [](const pii& a, const pii& b) {
        return a.second < b.second;
    });

    int count = 0;
    int lastEnd = INT_MIN;

    for (auto& interval : intervals) {
        if (interval.first >= lastEnd) {
            count++;
            lastEnd = interval.second;
        }
    }

    return count;
}

// ─────────────────────────────────────────────────────────────────────────────
// CSES ROOM ALLOCATION
// Assign minimum number of rooms to customers (print room numbers).
// ─────────────────────────────────────────────────────────────────────────────
vector<int> assignRooms(vector<tuple<int, int, int>> customers) {
    // customers: {arrival, departure, index}
    int n = customers.size();
    vector<int> assigned(n);

    // Sort by arrival time
    sort(all(customers));

    // Min-heap: {departure time, roomId}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    int rooms = 0;

    for (auto& cust : customers) {
        int arrival, departure, idx;
        tie(arrival, departure, idx) = cust;

        int roomId;

        if (!pq.empty() && pq.top().first < arrival) {
            // Reuse room
            roomId = pq.top().second;
            pq.pop();
        } else {
            // New room
            roomId = ++rooms;
        }

        assigned[idx] = roomId;
        pq.push({departure, roomId});
    }

    return assigned;
}

// ─────────────────────────────────────────────────────────────────────────────

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    /*
    // Example Usage:

    int n;
    cin >> n;
    vector<pii> intervals(n);
    for (int i = 0; i < n; i++) {
        cin >> intervals[i].first >> intervals[i].second;
    }

    cout << minMeetingRooms(intervals) << '\n';
    cout << maxActivities(intervals) << '\n';
    */

    return 0;
}