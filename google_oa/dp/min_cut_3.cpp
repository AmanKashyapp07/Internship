/**
 * LeetCode 131 - Palindrome Partitioning
 *
 * Description:
 * Given a string s, partition s such that every substring of the partition is a palindrome.
 * Return all possible palindrome partitioning of s.
 *
 * Approach:
 * - Backtracking with precomputed palindrome table.
 * - Precompute `isPal[i][j]` in O(n^2) time.
 * - Perform DFS backtracking: at state `start`, try all possible substrings `s[start...end]` that are palindromes, add to current path, and recurse to `end + 1`.
 *
 * Time Complexity: O(n * 2^n)
 * Space Complexity: O(n^2) for palindrome table, and O(n) recursion stack.
 */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    vector<vector<string>> ans;
    vector<string> path;
    vector<vector<bool>> isPal;
    string str;
    int size;

    void dfs(int start) {
        if (start == size) {
            ans.push_back(path);
            return;
        }

        for (int endVal = start; endVal < size; endVal++) {
            if (isPal[start][endVal]) {
                path.push_back(str.substr(start, endVal - start + 1));
                dfs(endVal + 1);
                path.pop_back();
            }
        }
    }

    vector<vector<string>> partition(string s) {
        str = s;
        size = s.size();
        isPal.assign(size, vector<bool>(size, false));
        for (int i = 0; i < size; i++) {
            isPal[i][i] = true;
        }

        for (int lengthVal = 2; lengthVal <= size; lengthVal++) {
            for (int i = 0; i + lengthVal - 1 < size; i++) {
                int j = i + lengthVal - 1;
                if (lengthVal == 2) {
                    isPal[i][j] = (str[i] == str[j]);
                } else {
                    isPal[i][j] = (str[i] == str[j]) && isPal[i + 1][j - 1];
                }
            }
        }

        ans.clear();
        path.clear();
        dfs(0);
        return ans;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    if (!(cin >> s)) return 0;

    Solution solver;
    vector<vector<string>> partitions = solver.partition(s);

    // Print all partitions
    for (auto& partition : partitions) {
        cout << "[ ";
        for (auto& str : partition) {
            cout << "\"" << str << "\" ";
        }
        cout << "]\n";
    }

    return 0;
}
