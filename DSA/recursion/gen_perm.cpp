/**
 * LeetCode 46 - Permutations
 *
 * Description:
 * Given an array of distinct integers, return all the possible permutations. You can return
 * the answer in any order.
 *
 * Approach:
 * - Backtracking using two methods:
 *   1. Bitmask-based Backtracking (`solve`): Uses a bitmask to track which elements are currently
 *      present in the permutation.
 *   2. Swap-based Backtracking (`solve2`): Swaps elements in place starting from index `idx` to
 *      generate permutations.
 *
 * Time Complexity: O(N! * N) to generate all N! permutations, where N is the size of the array.
 * Space Complexity: O(N) for the recursion stack and path tracking.
 */

#include <iostream>
#include <vector>
using namespace std;

vector<int> arr = {1, 2, 3};
vector<int> perm;
vector<vector<int>> ans1;
vector<vector<int>> ans2;

void solve(int mask) {
    if (perm.size() == arr.size()) {
        ans1.push_back(perm);
        return;
    }

    for (int i = 0; i < arr.size(); i++) {
        if (mask & (1 << i)) continue;

        perm.push_back(arr[i]);
        solve(mask | (1 << i));
        perm.pop_back();
    }
} // bitmasking , time compelxity is O(n!*n) , as we are generating all permutations of n elements, and for each permutation, we are printing it which takes O(n) time, so total time complexity is O(n!*n), space complexity is O(n) for the recursion stack and O(n) for the perm vector, so total space complexity is O(n)

void solve2(int idx) {
    if (idx == arr.size()) {
        ans2.push_back(perm);
        return;
    }

    for (int i = idx; i < arr.size(); i++) {
        swap(arr[idx], arr[i]);
        perm.push_back(arr[idx]);
        solve2(idx + 1);
        perm.pop_back();
        swap(arr[idx], arr[i]);
    }
} // backtracking , time complexity is O(n!*n) , as we are generating all permutations of n elements, and for each permutation, we are printing it which takes O(n) time, so total time complexity is O(n!*n), space complexity is O(n) for the recursion stack and O(n) for the perm vector, so total space complexity is O(n)

int main() {
    solve(0);
}
