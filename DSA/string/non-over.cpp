#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    int helper(vector<int> &nums, int L, int M)
    {
        int n = nums.size();

        int sumL = 0, sumM = 0;

        for (int i = 0; i < L; i++)
            sumL += nums[i]; // sum of first L elements
        for (int i = L; i < L + M; i++)
            sumM += nums[i]; // sum of next M elements

        int maxL = sumL;
        int ans = sumL + sumM;

        for (int i = L + M; i < n; i++)
        {
            sumM += nums[i] - nums[i - M]; // sliding window for M-length subarray
            sumL += nums[i - M] - nums[i - M - L]; // sliding window for L-length subarray
            maxL = max(maxL, sumL);                // keep track of the maximum sum of L-length subarray before the current M-length subarray
            ans = max(ans, maxL + sumM); // update the answer with the maximum sum of L-length subarray and current M-length subarray
            // we are always increasing the window for M and adjusting the window for L accordingly, ensuring that they do not overlap
            
        }

        return ans;
    }

    int maxSumTwoNoOverlap(vector<int> &nums, int firstLen, int secondLen)
    {
        return max(helper(nums, firstLen, secondLen),
                   helper(nums, secondLen, firstLen));
    }
};