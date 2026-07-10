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
 * Both approaces have same time complexity of O(2^N * log(2^N)) due to sorting the subset sums.
 * and same space complexity of O(2^N) to store all subset sums.
 *
 * Time Complexity: O(2^N * log(2^N)) where N is the size of the array, dominated by sorting the 2^N subset sums.
 * Space Complexity: O(2^N) to store all subset sums and O(N) for the recursion stack.
 */

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> arr(n);
    for (int &x : arr) {
        cin >> x;
    }

    // Bitmask Approach
    vector<int> bitAns = subsetSumBitmask(arr);

    cout << "Bitmask: ";
    for (int x : bitAns) {
        cout << x << ' ';
    }
    cout << '\n';

    return 0;
}
