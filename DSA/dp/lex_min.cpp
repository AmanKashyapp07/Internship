/**
 * Problem: Lexicographically Minimal Grid Path
 *
 * Description:
 * Find the lexicographically smallest path from top-left to bottom-right in an n x n grid of characters.
 *
 * Approach:
 * - Use a BFS/layer-by-layer traversal.
 * - At step `d` (from `0` to `2*n-2`), maintain a list of valid reachable cells.
 * - Find the minimum character among all neighbors to the right and down.
 * - Filter the next queue layer to only include cells containing this minimal character.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(n^2)
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class Solution {
public:
    int size;
    vector<string> matrix;
    string ans;
    vector<pair<int, int>> cur;
    vector<vector<bool>> visited;

    string getLexicographicallyMinimalPath(int n, vector<string>& grid) {
        size = n;
        matrix = grid;
        ans = "";
        ans += matrix[0][0];
        cur = {{0, 0}};
        visited.assign(size, vector<bool>(size, false));
        visited[0][0] = true;

        // Step-by-step BFS: path takes exactly 2*n - 2 steps to reach the destination
        for (int step = 0; step < 2 * size - 2; step++) {
            char best = 'z' + 1;
            for (auto [x, y] : cur) {
                if (x + 1 < size) best = min(best, matrix[x + 1][y]);
                if (y + 1 < size) best = min(best, matrix[x][y + 1]);
            }
            ans += best;
            vector<pair<int, int>> nxt;
            for (auto [x, y] : cur) {
                if (x + 1 < size && matrix[x + 1][y] == best && !visited[x + 1][y]) {
                    visited[x + 1][y] = true;
                    nxt.push_back({x + 1, y});
                }
                if (y + 1 < size && matrix[x][y + 1] == best && !visited[x][y + 1]) {
                    visited[x][y + 1] = true;
                    nxt.push_back({x, y + 1});
                }
            }
            cur = move(nxt); // Keep only the lexicographically optimal path endpoints
        }
        return ans;
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n;
    if (!(cin >> n)) return 0;
    vector<string> grid(n);
    for (int i = 0; i < n; i++) cin >> grid[i];

    Solution solver;
    cout << solver.getLexicographicallyMinimalPath(n, grid) << '\n';
    return 0;
}