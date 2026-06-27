/**
 * ========================================================
 *              Sliding Window Maximum
 * ========================================================
 *
 * Problem:
 * https://leetcode.com/problems/sliding-window-maximum/
 *
 * Category:
 * - Sliding Window
 * - Monotonic Deque
 *
 * Description:
 * Find the maximum element in every window of size k.
 *
 * Idea:
 * Maintain a deque of indices whose values are in decreasing order.
 *
 * Invariant:
 * 1. Indices are inside the current window.
 * 2. Values are strictly decreasing.
 * 3. Front always stores the maximum.
 *
 * Time  : O(n)
 * Space : O(k)
 *
 * Note:
 * For Sliding Window Minimum:
 * Replace
 *      arr[dq.back()] <= arr[R]
 * with
 *      arr[dq.back()] >= arr[R]
 * Everything else remains the same.
 * ========================================================
 */

#include <iostream>
#include <vector>
#include <deque>
using namespace std;

vector<int> maxSlidingWindow(vector<int>& arr, int k)
{
    int n = arr.size();

    deque<int> dq;      // Stores indices
    vector<int> ans;

    for (int R = 0; R < n; R++)
    {
        int L = R - k + 1;

        // 1. Remove expired indices
        while (!dq.empty() && dq.front() < L)
            dq.pop_front();

        // 2. Remove smaller elements
        while (!dq.empty() && arr[dq.back()] <= arr[R])
            dq.pop_back();

        // 3. Insert current index
        dq.push_back(R);

        // 4. Window becomes valid
        if (R >= k - 1)
            ans.push_back(arr[dq.front()]);
    }

    return ans;
}

int main()
{
    vector<int> arr = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;

    vector<int> ans = maxSlidingWindow(arr, k);

    for (int x : ans)
        cout << x << " ";

    return 0;
}