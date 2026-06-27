/**
 * LeetCode 37 - Sudoku Solver
 *
 * Description:
 * Write a program to solve a Sudoku puzzle by filling the empty cells. Empty cells are indicated by '.'.
 *
 * Approach:
 * - Backtracking / constraint satisfaction DFS.
 * - Traverse cell by cell. For any empty cell `(r, c)`, try placing digits '1' through '9'.
 * - Check if the placement is valid using `isValid()`, which checks if the digit already exists in 
 *   the same row, same column, or its corresponding 3x3 subgrid.
 * - If valid, place the digit, and recurse to solve the rest of the board. If the recursion succeeds, 
 *   return true. If not, backtrack by resetting the cell to '.' and try the next digit.
 *
 * Time Complexity: O(9^M) where M is the number of empty cells (worst-case scenario, though optimized in practice).
 * Space Complexity: O(M) for the recursion call stack where M is the number of empty cells.
 */

#include <iostream>
#include <vector>
using namespace std;

class Solution {
private:
    bool isValid(int r, int c, char val, const vector<vector<char>>& board) {
        for (int i = 0; i < 9; i++) {
            if (board[r][i] == val) return false; // row check
            if (board[i][c] == val) return false; // column check
        }
        int startRow = (r / 3) * 3;
        int startCol = (c / 3) * 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[startRow + i][startCol + j] == val) return false; // subgrid check
            }
        }
        return true;
    }

    bool backtrack(vector<vector<char>>& board) {
        for (int r = 0; r < 9; r++) {
            for (int c = 0; c < 9; c++) {
                if (board[r][c] != '.') continue;

                for (char val = '1'; val <= '9'; val++) {
                    if (isValid(r, c, val, board)) {
                        board[r][c] = val;
                        if (backtrack(board)) return true;
                        board[r][c] = '.'; // backtrack
                    }
                }
                return false; // dead end
            }
        }
        return true; // all cells filled successfully
    }
public:
    void solveSudoku(vector<vector<char>>& board) {
        backtrack(board);
    }
};

int main() {
    vector<vector<char>> board = {
        {'5','3','.','.','7','.','.','.','.'},
        {'6','.','.','1','9','5','.','.','.'},
        {'.','9','8','.','.','.','.','6','.'},
        {'8','.','.','.','6','.','.','.','3'},
        {'4','.','.','8','.','3','.','.','1'},
        {'7','.','.','.','2','.','.','.','6'},
        {'.','6','.','.','.','.','2','8','.'},
        {'.','.','.','4','1','9','.','.','5'},
        {'.','.','.','.','8','.','.','7','9'}
    };
    Solution solver;
    solver.solveSudoku(board);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    return 0;
}
