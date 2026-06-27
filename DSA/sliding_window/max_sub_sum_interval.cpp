/**
 * ========================================================
 *              Maximum Subarray Sum II
 * ========================================================
 *
 * Problem:
 * https://cses.fi/problemset/task/1644
 *
 * Category:
 * - Prefix Sum
 * - Monotonic Deque
 * - Sliding Window
 *
 * Description:
 * Find the maximum subarray sum whose length is between
 * A and B (inclusive).
 *
 * Idea:
 * Let
 *      pref[i] = sum of first i elements.
 *
 * For each ending index i,
 *
 *      sum(j + 1 ... i) = pref[i] - pref[j]
 *
 * where
 *
 *      i - B <= j <= i - A
 *
 * We need the minimum prefix sum in this range.
 * Maintain it using an increasing monotonic deque.
 *
 * Invariant:
 * 1. Indices are inside [i-B, i-A].
 * 2. Prefix sums are increasing.
 * 3. Front stores the minimum prefix sum.
 *
 * Time  : O(n)
 * Space : O(B)
 * ========================================================
 */

#include <iostream>
#include <vector>
#include <deque>
#include <climits>
using namespace std;

int main()
{
    int n, A, B;
    cin >> n >> A >> B;

    vector<long long> arr(n + 1);
    vector<long long> pref(n + 1, 0);

    for (int i = 1; i <= n; i++)
    {
        cin >> arr[i];
        pref[i] = pref[i - 1] + arr[i];
    }

    deque<int> dq;
    long long ans = LLONG_MIN;

    for (int i = A; i <= n; i++)
    {
        // Valid prefix indices:
        // i - B <= j <= i - A

        int L = i - B;
        int R = i - A;

        // 1. Remove expired indices
        while (!dq.empty() && dq.front() < L)
            dq.pop_front();

        // 2. Insert new candidate (R)
        while (!dq.empty() && pref[dq.back()] >= pref[R])
            dq.pop_back();

        dq.push_back(R);

        // 3. Minimum prefix sum gives maximum subarray ending at i
        ans = max(ans, pref[i] - pref[dq.front()]);
    }

    cout << ans << '\n';
}