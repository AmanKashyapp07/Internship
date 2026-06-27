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
// - If maxLeft <= maxRight, the water at `left` is determined by maxLeft (move left++).
// - Else, the water at `right` is determined by maxRight (move right--).
//
// Time: O(N) | Space: O(1)
// ─────────────────────────────────────────────────────────────────────────────

int trapTwoPointer(const vi& height) {
    int left = 0, right = height.size() - 1;
    int maxLeft = 0, maxRight = 0;
    int water = 0;

    while (left < right) {
        if (height[left] <= height[right]) {
            // Right side is taller; left side determines water level
            if (height[left] >= maxLeft) {
                maxLeft = height[left]; // Update left max boundary
            } else {
                water += maxLeft - height[left]; // Water trapped above left
            }
            left++;
        } else {
            // Left side is taller; right side determines water level
            if (height[right] >= maxRight) {
                maxRight = height[right]; // Update right max boundary
            } else {
                water += maxRight - height[right]; // Water trapped above right
            }
            right--;
        }
    }
    return water;
}

// ─────────────────────────────────────────────────────────────────────────────
// 1b. TRAPPING RAIN WATER — MONOTONIC STACK APPROACH
// LC 42
//
// Idea:
// - Maintain a monotonic decreasing stack of indices (left walls).
// - When we find a bar taller than the stack top, we found a "valley":
//     - Bottom of valley: the just-popped element (lowest point).
//     - Left wall: the new stack top after popping.
//     - Right wall: the current element i.
//   Width  = i - left_wall_index - 1
//   Height = min(height[left_wall], height[i]) - height[bottom]
//   Water  += width * height
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

int trapStack(const vi& height) {
    int n = height.size();
    stack<int> stk; // Monotonic decreasing stack of indices
    int water = 0;

    for (int i = 0; i < n; i++) {
        while (!stk.empty() && height[i] > height[stk.top()]) {
            int bottom = stk.top();
            stk.pop();

            if (stk.empty()) break; // No left wall, skip

            int leftWall = stk.top();
            int width = i - leftWall - 1;
            int boundedHeight = min(height[leftWall], height[i]) - height[bottom];
            water += width * boundedHeight;
        }
        stk.push(i);
    }
    return water;
}

// ─────────────────────────────────────────────────────────────────────────────
// 1c. TRAPPING RAIN WATER — PREFIX/SUFFIX MAX APPROACH
// LC 42
//
// Idea:
// - Precompute prefixMax[i] = max height from 0..i
// - Precompute suffixMax[i] = max height from i..n-1
// - Water at i = min(prefixMax[i], suffixMax[i]) - height[i]
//
// Time: O(N) | Space: O(N)
// ─────────────────────────────────────────────────────────────────────────────

int trapPrefixSuffix(const vi& height) {
    int n = height.size();
    vi prefixMax(n), suffixMax(n);

    prefixMax[0] = height[0];
    for (int i = 1; i < n; i++) {
        prefixMax[i] = max(prefixMax[i - 1], height[i]);
    }

    suffixMax[n - 1] = height[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        suffixMax[i] = max(suffixMax[i + 1], height[i]);
    }

    int water = 0;
    for (int i = 0; i < n; i++) {
        water += min(prefixMax[i], suffixMax[i]) - height[i];
    }
    return water;
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

int trapRainWater2D(vector<vector<int>>& heightMap) {
    if (heightMap.empty() || heightMap[0].empty()) return 0;
    int m = heightMap.size(), n = heightMap[0].size();
    if (m < 3 || n < 3) return 0;

    // Min-heap: {height, row, col}
    priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<>> pq;
    vector<vector<bool>> visited(m, vector<bool>(n, false));

    // Push all boundary cells
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 || i == m-1 || j == 0 || j == n-1) {
                pq.push({heightMap[i][j], i, j});
                visited[i][j] = true;
            }
        }
    }

    int water = 0;
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    while (!pq.empty()) {
        auto [h, x, y] = pq.top();
        pq.pop();

        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx < 0 || nx >= m || ny < 0 || ny >= n || visited[nx][ny]) continue;

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
// - Area = (right - left) * min(height[left], height[right])
// - Always move the pointer with the smaller height inward.
//   Moving the taller pointer would only decrease width without gaining height.
//
// Time: O(N) | Space: O(1)
// ─────────────────────────────────────────────────────────────────────────────

int maxWater(const vi& height) {
    int left = 0, right = height.size() - 1;
    int maxArea = 0;

    while (left < right) {
        int area = (right - left) * min(height[left], height[right]);
        maxArea = max(maxArea, area);

        // Move the shorter wall inward (can only improve by finding a taller wall)
        if (height[left] < height[right]) {
            left++;
        } else {
            right--;
        }
    }
    return maxArea;
}

// ─────────────────────────────────────────────────────────────────────────────

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    /*
        int n; cin >> n;
        vi height(n);
        for (int& x : height) cin >> x;

        cout << trapTwoPointer(height) << '\n';   // Two pointer O(1) space
        cout << trapStack(height) << '\n';         // Monotonic stack approach
        cout << trapPrefixSuffix(height) << '\n';  // Prefix/suffix max approach
        cout << maxWater(height) << '\n';          // Container with most water
    */

    /*
        int m, n; cin >> m >> n;
        vector<vector<int>> hmap(m, vector<int>(n));
        for (auto& row : hmap)
            for (int& x : row) cin >> x;
        cout << trapRainWater2D(hmap) << '\n';
    */

    return 0;
}
