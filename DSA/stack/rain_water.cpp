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
#include <queue>
#include <stack>
#include <vector>

using namespace std;
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
// 2. TRAPPING RAIN WATER II (3D version)
// LC 407
//
// Given a 2D height map, compute total volume of trapped water.
//
// Idea:
// - Use a min-heap (priority queue) and BFS from the boundary inward.
// - The water level at any inner cell is bounded by the minimum boundary cell seen so far.
// - Start by pushing all boundary cells into the heap.
// - Pop the smallest cell (potential water level boundary).
// - For each unvisited neighbor, water trapped = max(0, current_boundary - neighbor_height).
//   Then push the neighbor into the heap with height = max(neighbor_height, current_boundary).
//
// Time: O(M * N * log(M * N)) | Space: O(M * N)
// ─────────────────────────────────────────────────────────────────────────────

int trapRainWater2D(vector<vector<int>> &heightMap)
{
    if (heightMap.empty() || heightMap[0].empty())
        return 0;
    int m = heightMap.size(), n = heightMap[0].size();
    if (m < 3 || n < 3)
        return 0;

    // Min-heap: {height, row, col}
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
    vector<vector<bool>> visited(m, vector<bool>(n, false));

    // Push all boundary cells
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == 0 || i == m - 1 || j == 0 || j == n - 1)
            {
                pq.push({heightMap[i][j], i, j});
                visited[i][j] = true;
            }
        }
    }

    int water = 0;
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    while (!pq.empty())
    {
        auto [h, x, y] = pq.top();
        pq.pop();

        for (int d = 0; d < 4; d++)
        {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx < 0 || nx >= m || ny < 0 || ny >= n || visited[nx][ny])
                continue;

            visited[nx][ny] = true;
            // Water trapped above this cell is bounded by current minimum boundary h
            water += max(0, h - heightMap[nx][ny]);
            // Push with max(neighbor height, current boundary) as the new boundary
            pq.push({max(h, heightMap[nx][ny]), nx, ny});
        }
    }
    return water;
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

