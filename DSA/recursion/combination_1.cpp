/**
 * LeetCode 39 - Combination Sum
 *
 * Description:
 * Given an array of distinct integers candidates and a target integer target, return a list of all 
 * unique combinations of candidates where the chosen numbers sum to target. The same number may be 
 * chosen from candidates an unlimited number of times.
 *
 * Approach:
 * - Backtracking / Recursion.
 * - At each step (index `idx`), we have two choices:
 *   1. Include the current element `nums[idx]`, reducing the target and staying at the same index (`idx`) for reuse.
 *   2. Exclude the current element and move to the next index (`idx + 1`).
 * - Base cases: If target is 0, a valid combination is found. If target < 0 or index is out of bounds, prune.
 *
 * Time Complexity: O(2^(target/min_val)) in the worst case where min_val is the smallest candidate.
 * Space Complexity: O(target/min_val) for the recursion stack and path tracking.
 */

#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> ans;
vector<int> curr;

void backtrack(int idx, int target, const vector<int>& nums) {
    if (target == 0) {
        ans.push_back(curr);
        return;
    }

    if (idx == nums.size() || target < 0)
        return;

    // Take current element
    curr.push_back(nums[idx]);
    backtrack(idx, target - nums[idx], nums); // same index -> reuse allowed
    curr.pop_back();

    // Skip current element
    backtrack(idx + 1, target, nums);
}
// whenever path reconstruction is needed, we need to backtrack after each recursive call to explore other possibilities, we cannot avoid backtracking step, as we are modifying the curr vector in place, so we need to backtrack after each recursive call to explore other possibilities, if just want count , we can avoid backtracking step, as we are not modifying any global variable in place, so we can just return the count from each recursive call and add them up, but here we need to store the path in curr vector, so we need to backtrack after each recursive call to explore other possibilities
int main() {
    int n, target;
    cin >> n >> target;

    vector<int> nums(n);
    for (int i = 0; i < n; i++)
        cin >> nums[i];

    backtrack(0, target, nums);

    for (auto &comb : ans) {
        cout << "[ ";
        for (int x : comb)
            cout << x << " ";
        cout << "]\n";
    }

    return 0;
}