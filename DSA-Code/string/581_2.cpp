class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        int n = nums.size();

        int start = 0, end = 0;
        int maxSeen = nums[0];
        int minSeen = nums[n - 1];
        int flag=1;
        for(auto i=0;i<n-1;i++){
            if(nums[i]>nums[i+1]){
                flag=0;
                break;
            }
        }
        if(flag==1) return 0;
        // Find right boundary
        for (int i = 1; i < n; i++) {
            maxSeen = max(maxSeen, nums[i]);
            if (nums[i] < maxSeen) end = i; // every new violation will update the right boundary
        }

        // Find left boundary
        for (int i = n - 2; i >= 0; i--) {
            minSeen = min(minSeen, nums[i]);
            if (nums[i] > minSeen) start = i; // every new violation will update the left boundary
        }

        return end - start + 1;
    }
};