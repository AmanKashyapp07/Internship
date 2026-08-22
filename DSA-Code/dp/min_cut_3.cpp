/**
 * Palindrome Partitioning (LeetCode 131)
 * Return all possible ways to partition string into palindromic substrings.
 * Time: O(n * 2^n), Space: O(n^2)
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    vector<vector<string>> ans;
    vector<string> path;
    vector<vector<bool>> isPal;

    void dfs(const string& s, int start) {
        if (start == s.size()) {
            ans.push_back(path);
            return;
        }

        for (int end = start; end < s.size(); ++end) {
            if (isPal[start][end]) {
                path.push_back(s.substr(start, end - start + 1));
                dfs(s, end + 1);
                path.pop_back();
            }
        }
    }

    vector<vector<string>> partition(string s) {
        int n = s.size();
        isPal.assign(n, vector<bool>(n, false));

        // Precompute palindromes
        for (int i = 0; i < n; ++i) {
            isPal[i][i] = true;
        }
        for (int len = 2; len <= n; ++len) {
            for (int i = 0; i <= n - len; ++i) {
                int j = i + len - 1;
                if (len == 2)
                    isPal[i][j] = (s[i] == s[j]);
                else
                    isPal[i][j] = (s[i] == s[j]) && isPal[i + 1][j - 1];
            }
        }

        ans.clear();
        path.clear();
        dfs(s, 0);
        return ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    Solution solver;
    vector<vector<string>> result = solver.partition(s);

    for (auto& part : result) {
        cout << "[ ";
        for (auto& str : part) {
            cout << "\"" << str << "\" ";
        }
        cout << "]\n";
    }

    return 0;
}