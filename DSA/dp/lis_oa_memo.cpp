#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> input;
    vector<vector<int>> pairList;
    vector<vector<int>> memo;

    // ============================================================================
    // 1. Longest Increasing Subsequence (LIS)
    // ============================================================================

    int solveLIS(int idx, int prev) {
        if (idx == input.size()) return 0;

        if (memo[idx][prev + 1] != -1)
            return memo[idx][prev + 1];

        int skip = solveLIS(idx + 1, prev);

        int take = 0;
        if (prev == -1 || input[idx] > input[prev])
            take = 1 + solveLIS(idx + 1, idx);

        return memo[idx][prev + 1] = max(take, skip);
    }

    int lengthOfLISMemo(vector<int>& arr) {
        input = arr;
        int n = arr.size();
        memo.assign(n, vector<int>(n + 1, -1));
        return solveLIS(0, -1);
    }

    // ============================================================================
    // 2. Maximum Sum Increasing Subsequence
    // ============================================================================

    int solveMaxSum(int idx, int prev) {
        if (idx == input.size()) return 0;

        if (memo[idx][prev + 1] != -1)
            return memo[idx][prev + 1];

        int skip = solveMaxSum(idx + 1, prev);

        int take = 0;
        if (prev == -1 || input[idx] > input[prev])
            take = input[idx] + solveMaxSum(idx + 1, idx);

        return memo[idx][prev + 1] = max(take, skip);
    }

    int maxSumISMemo(vector<int>& arr) {
        input = arr;
        int n = arr.size();
        memo.assign(n, vector<int>(n + 1, -1));
        return solveMaxSum(0, -1);
    }

    // ============================================================================
    // 3. Longest Non-Decreasing Subsequence
    // ============================================================================

    int solveLNDS(int idx, int prev) {
        if (idx == input.size()) return 0;

        if (memo[idx][prev + 1] != -1)
            return memo[idx][prev + 1];

        int skip = solveLNDS(idx + 1, prev);

        int take = 0;
        if (prev == -1 || input[idx] >= input[prev])
            take = 1 + solveLNDS(idx + 1, idx);

        return memo[idx][prev + 1] = max(take, skip);
    }

    int lengthOfLNDSMemo(vector<int>& arr) {
        input = arr;
        int n = arr.size();
        memo.assign(n, vector<int>(n + 1, -1));
        return solveLNDS(0, -1);
    }

    // ============================================================================
    // 4. Longest Chain of Pairs
    // ============================================================================

    int solveChain(int idx, int prev) {
        if (idx == pairList.size()) return 0;

        if (memo[idx][prev + 1] != -1)
            return memo[idx][prev + 1];

        int skip = solveChain(idx + 1, prev);

        int take = 0;
        if (prev == -1 || pairList[idx][0] > pairList[prev][1])
            take = 1 + solveChain(idx + 1, idx);

        return memo[idx][prev + 1] = max(take, skip);
    }

    int findLongestChainMemo(vector<vector<int>>& arr) {
        pairList = arr;
        sort(pairList.begin(), pairList.end());
        int n = pairList.size();
        memo.assign(n, vector<int>(n + 1, -1));
        return solveChain(0, -1);
    }
};