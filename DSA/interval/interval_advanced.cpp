/**
 * @file IntervalVariantsMaster.cpp
 * @brief Compressed Global Templates for High-Tier Interval Variants (Google, Amazon, Meta OA).
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <array>

using namespace std;
using pii = pair<int, int>;



// =========================================================================
// VARIANT 3: INTERVAL COVER (GREEDY RANGE COVER / JUMP GAME II STYLE)
// =========================================================================
/**
 * @brief Finds min intervals to completely cover range [0, target].
 * @note Intuition: Sort by start times. At each boundary edge, greedily pick the 
 * reachable interval extending the furthest forward.
 * Time: O(N log N) | Space: O(1)
 */
int intervalCover(vector<pii>& intervals, int target) {
    sort(intervals.begin(), intervals.end());
    int count = 0, covered = 0, i = 0, n = intervals.size();

    while (covered < target) {
        int farthest = covered;
        // Evaluate all intervals starting at or before current covered boundary
        while (i < n && intervals[i].first <= covered) {
            farthest = max(farthest, intervals[i].second);
            i++;
        }
        if (farthest == covered) return -1; // Gap encountered; cannot bridge to target
        covered = farthest;
        count++;
    }
    return count;
}

// =========================================================================
// VARIANT 4: SWEEP LINE (MAX OVERLAP COUNT & POINT QUERY)
// =========================================================================
/**
 * @brief 4a. Finds peak concurrent overlapping intervals at any given instance.
 * @brief 4b. Finds count of intervals covering a target point.
 * Time: O(N log N) | Space: O(N)
 */
int maxOverlapCount(const vector<pii>& intervals) {
    vector<pair<int, int>> events;
    for (const auto& [s, e] : intervals) {
        events.push_back({s, 1});   // Start: Increment room needed
        events.push_back({e, -1});  // End: Decrement room released
    }
    // Sort chronologically; process starts (+1) before ends (-1) if bounds are inclusive
    sort(events.begin(), events.end(), [](const pii& a, const pii& b) {
        return a.first < b.first || (a.first == b.first && a.second > b.second);
    });
    // if you want [2,4] and [4,6] to be not overlapping then just normal sort will work, if you want them to be overlapping then use the above sort function

    int maxCount = 0, curCount = 0;
    for (const auto& [time, delta] : events) {
        curCount += delta;
        maxCount = max(maxCount, curCount);
    }
    return maxCount;
}

