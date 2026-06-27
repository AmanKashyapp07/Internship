/**
 * LeetCode 51 - N-Queens
 *
 * Description:
 * The n-queens puzzle is the problem of placing n queens on an n x n chessboard such that no two
 * queens attack each other. Given an integer n, return all distinct solutions to the n-queens puzzle.
 *
 * Approach:
 * - Backtracking / DFS.
 * - Place queens row by row.
 * - Maintain three lookup sets/arrays to check conflicts in O(1) time:
 *   1. Column conflict: `cols`
 *   2. Diagonal (top-left to bottom-right) conflict: `diag1` (where row - col is constant)
 *   3. Anti-diagonal (top-right to bottom-left) conflict: `diag2` (where row + col is constant)
 * - If placing a queen is safe, mark the sets, recurse to the next row, and backtrack by unmarking them.
 *
 * Time Complexity: O(N!) since we place at most one queen per row with pruning.
 * Space Complexity: O(N) for column and diagonal tracking sets and recursion call stack.
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
using namespace std;

class Solution {
private:
    vector<string> buildBoard(int n, const vector<int>& queens) {
        vector<string> board(n, string(n, '.'));
        for (int r = 0; r < n; r++) {
            board[r][queens[r]] = 'Q';
        }
        return board;
    }

    void backtrack(int row, int n, vector<int>& queens, unordered_set<int>& cols,
                   unordered_set<int>& diag1, unordered_set<int>& diag2,
                   vector<vector<string>>& result) {
        if (row == n) {
            result.push_back(buildBoard(n, queens));
            return;
        }

        for (int col = 0; col < n; col++) {
            int d1 = row - col;
            int d2 = row + col;

            if (cols.count(col) || diag1.count(d1) || diag2.count(d2)) {
                continue; // under attack
            }

            queens[row] = col;
            cols.insert(col);
            diag1.insert(d1);
            diag2.insert(d2);

            backtrack(row + 1, n, queens, cols, diag1, diag2, result);

            queens[row] = -1;
            cols.erase(col);
            diag1.erase(d1);
            diag2.erase(d2);
        }
    }
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> result;
        vector<int> queens(n, -1);
        unordered_set<int> cols, diag1, diag2;
        backtrack(0, n, queens, cols, diag1, diag2, result);
        return result;
    }
};

int main() {
    Solution solver;
    int n = 4;
    vector<vector<string>> result = solver.solveNQueens(n);
    for (const auto& board : result) {
        for (const string& row : board) {
            cout << row << "\n";
        }
        cout << "\n";
    }
    return 0;
}
