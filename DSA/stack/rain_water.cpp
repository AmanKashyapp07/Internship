/**
 * Monotonic Stack - Trapping Rain Water
 *
 * 1. Trapping Rain Water (1D)        - LC 42 (Hard) — most classic OA question
 * 2. Trapping Rain Water (2D)        - LC 407 (Hard) — BFS + priority queue
 * 3. Container With Most Water       - LC 11 (two pointer variant)
 *
 * LC 42 has THREE approaches worth knowing:
 *   a) Two Pointer         - O(N) time, O(1) space (cleanest)
 *   b) Prefix/Suffix Max   - O(N) time, O(N) space (easiest to understand)
 *   c) Monotonic Stack     - O(N) time, O(N) space (most generalizable pattern)
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <stack>
#include <vector>
#include <string>

using namespace std;
using ll = long long;
using vi = vector<int>;

// ─────────────────────────────────────────────────────────────────────────────
// 1a. TRAPPING RAIN WATER — TWO POINTER APPROACH
// LC 42
//
// Idea:
// - Water trapped above position i = min(maxLeft[i], maxRight[i]) - height[i]
// - Two pointer: track maxLeft and maxRight as we move inward.
// - If maxLeft <= maxRight, the water at `i` is determined by maxLeft (move i++).
// - Else, the water at `j` is determined by maxRight (move j--).
//
// Time: O(N) | Space: O(1)
// ─────────────────────────────────────────────────────────────────────────────
int trap(vi &height) {
        ll n = height.size();
        stack<ll> stk;
        ll water = 0;

        for (ll i = 0; i < n; i++) {
            while (!stk.empty() && height[stk.top()] < height[i]) { 
                // Keep equals (do not pop on equal): water is trapped between strictly taller boundary
                // walls. Equal heights form a flat bottom rather than a container wall.
                ll bottom = stk.top();
                stk.pop();

                if (stk.empty())
                    break;

                // for each height, we are finding PGE (Previous Greater Element) and NGE (Next Greater Element)
                ll leftWall = stk.top();
                ll rightWall = i;
                ll width = rightWall - leftWall - 1;
                ll boundedHeight = min((ll)height[leftWall], (ll)height[rightWall]) - (ll)height[bottom];
                water += width * boundedHeight;
            }

            stk.push(i);
        }

        return (int)water;
    }



// ─────────────────────────────────────────────────────────────────────────────
// 3. CONTAINER WITH MOST WATER
// LC 11
//
// Given heights, find two lines that together with the x-axis forms a container
// that holds the most water.
//
// Idea (Two Pointer):
// - Area = (j - i) * min(height[i], height[j])
// - Always move the pointer with the smaller height inward.
//   Moving the taller pointer would only decrease width without gaining height.
//
// Time: O(N) | Space: O(1)
// ─────────────────────────────────────────────────────────────────────────────

int maxWater(const vi &height)
{
    int i = 0, j = height.size() - 1;
    int maxArea = 0;

    while (i < j)
    {
        int area = (j - i) * min(height[i], height[j]);
        maxArea = max(maxArea, area);

        // Move the shorter wall inward (can only improve by finding a taller wall)
        if (height[i] < height[j])
        {
            i++;
        }
        else
        {
            j--;
        }
    }
    return maxArea;
}

