/**
 * Problem: Sliding Window Maximum
 * Link: https://leetcode.com/problems/sliding-window-maximum/
 * Category: Sliding Window
 * 
 * Description:
 * Find the maximum element in every sliding window of size k.
 * 
 * Logic/Approach:
 * Monotonic deque maintaining indices of decreasing values.
 */

#include <iostream>
#include <vector>
#include <deque>
using namespace std;

vector<int> maxSlidingWindow(vector<int>& arr, int k)
{
    int n = arr.size();

    deque<int> dq;
    vector<int> ans;

    for (int R = 0; R < n; R++)
    {
        int L = R - k + 1;

        // 1. Remove indices outside the range [L, R]
        while (!dq.empty() && dq.front() < L)
        {
            dq.pop_front();
        }

        // 2. Insert R while maintaining decreasing values
        while (!dq.empty() && arr[dq.back()] <= arr[R]) // for min use arr[dq.back()] >= arr[R], rest is same
        {
            dq.pop_back();
        }

        dq.push_back(R);

        // 3. Window formed
        if (L >= 0)
        {
            int mx = arr[dq.front()];
            ans.push_back(mx);
        }
    }

    return ans;
}

int main()
{
    vector<int> arr = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;

    vector<int> ans = maxSlidingWindow(arr, k);

    for (int x : ans)
    {
        cout << x << " ";
    }

    return 0;
}