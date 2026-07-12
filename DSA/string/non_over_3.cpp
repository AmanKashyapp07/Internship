#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> maxSumOfThreeSubarrays(vector<int>& nums, int k) {
        int n = nums.size();

        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) prefix[i + 1] = prefix[i] + nums[i];

        int m = n - k + 1;
        vector<int> subSum(m);

        for (int i = 0; i < m; i++) subSum[i] = prefix[i + k] - prefix[i];

        vector<int> left(m); // left[i] holds the index of the maximum subarray sum in [0 ... i]
        left[0] = 0; // the first subarray is the only option for the leftmost subarray

        for (int i = 1; i < m; i++) {
            if (subSum[i] > subSum[left[i - 1]])
                left[i] = i;
            else
                left[i] = left[i - 1];
        }

        // right[i] = index of maximum subarray in [i ... m-1]
        vector<int> right(m);
        right[m - 1] = m - 1;

        for (int i = m - 2; i >= 0; i--) {
            if (subSum[i] >= subSum[right[i + 1]])
                right[i] = i;
            else
                right[i] = right[i + 1];
        }

        vector<int> ans(3);
        int best = -1;

        // Try every possible middle subarray
        for (int mid = k; mid <= m - k - 1; mid++) {
            int l = left[mid - k];
            int r = right[mid + k];

            int total = subSum[l] + subSum[mid] + subSum[r];

            if (total > best) {
                best = total;
                ans = {l, mid, r};
            }
        }

        return ans;
    }
};