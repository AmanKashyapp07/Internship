/**
 * CSES Problem Set
 *
 * Interval Templates
 *
 * 1. Meeting Rooms II
 *    - Minimum rooms required
 *    - Sort by start time
 *    - Min Heap of ending times
 *
 * 2. Activity Selection
 *    - Maximum non-overlapping intervals
 *    - Sort by ending time
 *    - Greedy
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
//
// Given intervals, return minimum rooms required.
//
// Idea:
// - Sort by start time.
// - Heap stores ending times of active meetings.
// - If earliest ending meeting finishes before current starts,
//   reuse that room.
// - Heap size = rooms currently occupied.
// - Maximum heap size encountered = answer.
//
// Time : O(n log n)
// Space: O(n)
// ─────────────────────────────────────────────────────────────────────────────

int minMeetingRooms(vector<pii> &intervals)
{
    sort(all(intervals)); // this already sort by start time because pair is sorted by first element by default

    // Min-heap storing:
    // {departure time of the room currently in use, room id}
    //
    // The room that becomes free the earliest is always on top.
    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>>
        pq;

    vector<int> assigned(n); // assigned[i] = room number given to interval i
    int rooms = 0;           // total number of rooms created

    for (auto &cur : v)
    {
        int roomId;

        // Check whether the earliest finishing room is available.
        // For CSES Room Allocation, departure and arrival days are inclusive,
        // so a room can only be reused if:
        //
        //     previous_departure < current_arrival
        //
        if (!pq.empty() && pq.top().first < cur.start)
        {
            // Reuse the room that became free first.
            roomId = pq.top().second;
            pq.pop();
        }
        else
        {
            // No existing room is available.
            // Create a new room.
            roomId = ++rooms;
        }

        // Assign the selected room to the current customer.
        assigned[cur.idx] = roomId;

        // Mark this room as occupied until cur.end.
        pq.push({cur.end, roomId});
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// ACTIVITY SELECTION
//
// Given intervals, return maximum number of
// non-overlapping intervals.
//
// Idea:
// - Sort by ending time.
// - Always pick interval that ends earliest.
// - Greedy proof:
//   leaves maximum space for future intervals.
//
// Time : O(n log n)
// Space: O(1)
// ─────────────────────────────────────────────────────────────────────────────

int maxActivities(vector<pii> &intervals)
{
    sort(all(intervals),
         [](const pii &a, const pii &b)
         {
             return a.second < b.second;
         });

    int cnt = 0;
    int lastEnd = -INF;

    for (auto &[start, end] : intervals)
    {
        if (start >= lastEnd)
        {
            cnt++;
            lastEnd = end;
        }
    }

    return cnt;
}

// ─────────────────────────────────────────────────────────────────────────────

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    /*
        Example:

        int n;
        cin >> n;

        vector<pii> intervals(n);

        for(auto &p : intervals)
            cin >> p.first >> p.second;

        cout << minMeetingRooms(intervals) << '\n';
        cout << maxActivities(intervals) << '\n';
    */

    return 0;
}