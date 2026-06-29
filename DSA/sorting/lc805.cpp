#include <iostream>
#include <vector>
#include <numeric>
#include <unordered_set>

using namespace std;

class Solution {
public:
    // Preserved your exact helper: Returns a list of {mask, sum} grouped by subset size
    vector<vector<pair<int, int>>> getSubsetMasksAndSums(const vector<int>& arr) {
        int n = arr.size();
        vector<vector<pair<int, int>>> results(n + 1);
        
        for (int mask = 0; mask < (1 << n); ++mask) {
            int currentSum = 0;
            int count = 0;
            for (int i = 0; i < n; ++i) {
                if (mask & (1 << i)) {
                    currentSum += arr[i];
                    count++;
                }
            }
            results[count].push_back({mask, currentSum});
        }
        return results;
    }

    bool splitArraySameAverage(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return false;

        int totalSum = accumulate(nums.begin(), nums.end(), 0);
        vector<int> A(n);
        for (int i = 0; i < n; ++i) A[i] = nums[i] * n - totalSum;

        int m = n / 2;
        vector<int> leftArr(A.begin(), A.begin() + m);
        vector<int> rightArr(A.begin() + m, A.end());

        // Preserved your exact Meet-in-the-Middle data split
        auto leftData = getSubsetMasksAndSums(leftArr);
        auto rightData = getSubsetMasksAndSums(rightArr);

        // Check combinations using your size-based loops (k1 and k2)
        for (int k1 = 0; k1 <= m; ++k1) {
            for (int k2 = 0; k2 <= (n - m); ++k2) {
                if (k1 + k2 == 0 || k1 + k2 == n) continue;

                // OPTIMIZATION: Instead of a nested loop over rightData[k2],
                // put rightData sums into a hash set for O(1) lookups.
                unordered_set<int> rightSums;
                for (auto& p2 : rightData[k2]) {
                    rightSums.insert(p2.second);
                }

                // Now check if the required complement exists in O(1) time
                for (auto& p1 : leftData[k1]) {
                    int target = -p1.second; 
                    if (rightSums.count(target)) {
                        // Valid split found! 
                        // Original masks can still be reconstructed here if needed:
                        // p1.first and (matching_p2_mask << m)
                        return true;
                    }
                }
            }
        }
        return false;
    }
};