/**
 * Problem: Set Matrix Zeroes
 * Link: https://leetcode.com/problems/set-matrix-zeroes/
 *
 * Approach:
 * 1. If a row contains a zero, mark all its non-zero elements as -1.
 * 2. If a column contains a zero, mark all its non-zero elements as -1.
 * 3. Convert all marked cells (-1) to 0.
 *
 * Note:
 * This approach assumes -1 is not a valid matrix value.
 */

#include <iostream>
#include <vector>

using namespace std;

void setMatrixZeroes(vector<vector<int>>& matrix)
{
    int rows = matrix.size();
    int cols = matrix[0].size();

    // Mark rows
    for (int r = 0; r < rows; r++)
    {
        bool hasZero = false;

        for (int c = 0; c < cols; c++)
        {
            if (matrix[r][c] == 0)
            {
                hasZero = true;
                break;
            }
        }

        if (hasZero)
        {
            for (int c = 0; c < cols; c++)
            {
                if (matrix[r][c] != 0)
                {
                    matrix[r][c] = -1;
                }
            }
        }
    }

    // Mark columns
    for (int c = 0; c < cols; c++)
    {
        bool hasZero = false;

        for (int r = 0; r < rows; r++)
        {
            if (matrix[r][c] == 0)
            {
                hasZero = true;
                break;
            }
        }

        if (hasZero)
        {
            for (int r = 0; r < rows; r++)
            {
                if (matrix[r][c] != 0)
                {
                    matrix[r][c] = -1;
                }
            }
        }
    }

    // Convert marked cells to zero
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (matrix[r][c] == -1)
            {
                matrix[r][c] = 0;
            }
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> matrix(n, vector<int>(m));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> matrix[i][j];
        }
    }

    setMatrixZeroes(matrix);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << matrix[i][j] << ' ';
        }
        cout << '\n';
    }

    return 0;
}