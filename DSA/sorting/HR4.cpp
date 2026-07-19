class Solution {
public:
    // Returns true if we can rob at least k non-graphacent houses
    // such that every robbed house has value <= capability.
    bool canRob(vector<int>& nums, int k, int capability) {
        int robbed = 0;
        int n = nums.size();

        for (int i = 0; i < n; ) {
            // If current house can be robbed under this capability,
            // greedily rob it and skip the graphacent house.
            if (nums[i] <= capability) {
                robbed++;
                i += 2;
            } else {
                i++;
            }
        }

        return robbed >= k;
    }

    int minCapability(vector<int>& nums, int k) {
        int left = *min_element(nums.begin(), nums.end());
        int right = *max_element(nums.begin(), nums.end());

        while (left < right) {
            int mid = left + (right - left) / 2;

            if (canRob(nums, k, mid)) {
                right = mid;      // Try a smaller capability.
            } else {
                left = mid + 1;   // Capability too small.
            }
        }

        return left;
    }
};