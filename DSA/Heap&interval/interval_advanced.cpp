/**
 * @file IntervalVariantsMaster.cpp
 * @brief Compressed Global Templates for High-Tier Interval Variants
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

// =========================================================================
// VARIANT 3: INTERVAL COVER (Minimum Intervals to Cover Range)
// =========================================================================
/**
 * Finds the minimum number of intervals needed to completely cover [0, target].
 * Returns -1 if impossible.
 *
 * Time: O(N log N) | Space: O(1)
 */
int intervalCover(vector<pii> &intervals, int target)
{
    sort(intervals.begin(), intervals.end());

    int count = 0, covered = 0, i = 0, n = intervals.size();

    while (covered < target)
    {
        int farthest = covered;

        while (i < n && intervals[i].first <= covered)
        {
            farthest = max(farthest, intervals[i].second);
            i++;
        }

        if (farthest == covered)
            return -1; // Cannot cover

        covered = farthest;
        count++;
    }

    return count;
}

// =========================================================================
// VARIANT 4: SWEEP LINE - MAXIMUM OVERLAP
// =========================================================================
/**
 * Returns the maximum number of overlapping intervals at any point.
 *
 * Time: O(N log N) | Space: O(N)
 */
int maxOverlapCount(const vector<pii> &intervals)
{
    vector<pair<int, int>> events;

    for (const auto &[s, e] : intervals)
    {
        events.emplace_back(s, 1);  // Start event
        events.emplace_back(e, -1); // End event
    }

    // Sort by time; if two events occur at the same time, process
    // start events before end events. This treats intervals as
    // closed [l, r], so [1,2] and [2,3] are considered overlapping.
    sort(events.begin(), events.end(), [](const pii &a, const pii &b)
         { return a.first < b.first ||
                  (a.first == b.first && a.second > b.second); });

    int maxCount = 0, cur = 0;
    for (const auto &[time, delta] : events)
    {
        cur += delta;
        maxCount = max(maxCount, cur);
    }

    return maxCount;
}