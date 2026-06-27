/**
 * Subset Sums
 *
 * Description:
 * Given a list of N integers, return the sums of all subsets. Output should be sorted.
 *
 * Approach:
 * - Two alternative approaches are implemented:
 *   1. Recursion / Backtracking (`generateSubsetSums` and `generateSubsetSumsBacktracking`):
 *      For each element, we have two choices: either include it in the sum or exclude it.
 *      Recursion is done for both branches.
 *   2. Bit Manipulation (`subsetSumBitmask`):
 *      Each subset is represented by a bitmask from 0 to (2^N - 1). For each mask, we sum up
 *      elements whose corresponding bit is set.
 * - Finally, the resulting sums are sorted.
 *
 * Time Complexity: O(2^N * log(2^N)) where N is the size of the array, dominated by sorting the 2^N subset sums.
 * Space Complexity: O(2^N) to store all subset sums and O(N) for the recursion stack.
 */

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// -----------------------------------------------------------------------------
// Approach 1 : Recursion
// -----------------------------------------------------------------------------

void generateSubsetSums(int idx, int sum, const vector<int>& arr, vector<int>& ans) {
    if (idx == arr.size()) {
        ans.push_back(sum);
        return;
    }

    // Pick current element
    // When we pass the expression directly, no need for a backtracking step as we are passing sum by value,
    // so it will not affect the sum in the next recursive call.
    generateSubsetSums(idx + 1, sum + arr[idx], arr, ans);

    // Don't pick current element
    generateSubsetSums(idx + 1, sum, arr, ans);
}

void generateSubsetSumsBacktracking(int idx, int sum, const vector<int>& arr, vector<int>& ans) {
    if (idx == arr.size()) {
        ans.push_back(sum);
        return;
    }

    // Pick current element
    sum += arr[idx];
    generateSubsetSumsBacktracking(idx + 1, sum, arr, ans);
    sum -= arr[idx]; // backtrack, here it is necessary to backtrack as we are passing sum by reference, so it will affect the sum in the next recursive call

    // Don't pick current element
    generateSubsetSumsBacktracking(idx + 1, sum, arr, ans);
}

vector<int> subsetSumRecursive(const vector<int>& arr) {
    vector<int> ans;
    generateSubsetSums(0, 0, arr, ans);
    sort(ans.begin(), ans.end());
    return ans;
}

// -----------------------------------------------------------------------------
// Approach 2 : Bit Manipulation
// -----------------------------------------------------------------------------

vector<int> subsetSumBitmask(const vector<int>& arr) {
    int n = arr.size();
    vector<int> ans;
    int totalSubsets = 1 << n;

    for (int mask = 0; mask < totalSubsets; mask++) {
        int sum = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                sum += arr[i];
            }
        }
        ans.push_back(sum);
    }

    sort(ans.begin(), ans.end());
    return ans;
}

// -----------------------------------------------------------------------------
// Driver
// -----------------------------------------------------------------------------

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> arr(n);
    for (int &x : arr) {
        cin >> x;
    }

    // Recursive Approach
    vector<int> recAns = subsetSumRecursive(arr);

    cout << "Recursive: ";
    for (int x : recAns) {
        cout << x << ' ';
    }
    cout << '\n';

    // Bitmask Approach
    vector<int> bitAns = subsetSumBitmask(arr);

    cout << "Bitmask: ";
    for (int x : bitAns) {
        cout << x << ' ';
    }
    cout << '\n';

    return 0;
}
