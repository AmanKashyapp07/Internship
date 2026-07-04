class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        int n = nums.size();

        int start = -1, end = -2;   // Handles already sorted array
        int maxSeen = nums[0];
        int minSeen = nums[n - 1];

        // Find right boundary
        for (int i = 1; i < n; i++) {
            maxSeen = max(maxSeen, nums[i]);
            if (nums[i] < maxSeen)
                end = i;
        }

        // Find left boundary
        for (int i = n - 2; i >= 0; i--) {
            minSeen = min(minSeen, nums[i]);
            if (nums[i] > minSeen)
                start = i;
        }

        return end - start + 1;
    }
};