/**
 * LeetCode 78 - Subsets
 *
 * Description:
 * Given an integer array nums of unique elements, return all possible subsets (the power set).
 * The solution set must not contain duplicate subsets. Return the solution in any order.
 *
 * Approach:
 * - Backtracking / DFS.
 * - At each step, add the current subset (`current`) to the result.
 * - Iterate `i` from `start` to `nums.size() - 1`, include `nums[i]` in the subset, recurse to `i + 1`, 
 *   and then backtrack by popping `nums[i]` out of the subset.
 *
 * Time Complexity: O(2^N * N) because there are 2^N subsets and copying each takes O(N) time.
 * Space Complexity: O(N) for recursion stack and current path.
 */

#include <iostream>
#include <vector>
using namespace std;

class Solution {
private:
    void backtrack(int start, const vector<int>& nums, vector<int>& current, vector<vector<int>>& result) {
        result.push_back(current);
        for (int i = start; i < nums.size(); i++) {
            current.push_back(nums[i]);
            backtrack(i + 1, nums, current, result);
            current.pop_back();
        }
    }
    // to avoid duplicate subsets, we can sort the input array and skip duplicates during the iteration. However, since the problem states that the input array contains unique elements, this is not necessary here.
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> current;
        backtrack(0, nums, current, result);
        return result;
    }
};

int main() {
    Solution solver;
    vector<int> nums = {1, 2, 3};
    vector<vector<int>> result = solver.subsets(nums);
    for (const auto& subset : result) {
        cout << "[ ";
        for (int x : subset) cout << x << " ";
        cout << "]\n";
    }
    return 0;
}
