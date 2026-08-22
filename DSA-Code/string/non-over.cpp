#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int helper(vector<int>& nums, int L, int M) {
        int n = nums.size();
        int sumL = 0, sumM = 0;

        for (int i = 0; i < L; ++i) sumL += nums[i];
        for (int i = L; i < L + M; ++i) sumM += nums[i];

        int maxL = sumL;
        int ans = sumL + sumM;

        for (int i = L + M; i < n; ++i) { // we are moving m size window , but l size window will be variable and may not be fixed
            sumM += nums[i] - nums[i - M];
            sumL += nums[i - M] - nums[i - M - L];
            maxL = max(maxL, sumL);
            ans = max(ans, maxL + sumM);
        }

        return ans;
    }

    int maxSumTwoNoOverlap(vector<int>& nums, int firstLen, int secondLen) {
        return max(helper(nums, firstLen, secondLen),
                   helper(nums, secondLen, firstLen));
    }
};