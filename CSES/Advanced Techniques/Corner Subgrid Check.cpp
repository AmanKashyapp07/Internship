// Link: https://cses.fi/problemset/task/3360

#include <iostream>
#include <vector>
#include <string>

using namespace std;

void solve() {
    int n, k;
    if (!(cin >> n >> k)) return;

    vector<string> grid(n);
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
    }

    // Process each letter from 'A' up to the k-th uppercase letter
    for (int char_idx = 0; char_idx < k; ++char_idx) {
        char target = 'A' + char_idx;
        bool found = false;
        
        // seen[c1][c2] will store if we've seen a row where both columns c1 and c2 have 'target'
        // Using a flat vector or a 2D vector. A 2D vector is clean and easy to reset.
        vector<vector<bool>> seen(n, vector<bool>(n, false));

        for (int r = 0; r < n; ++r) {
            // Find all columns in this row that match our target letter
            vector<int> cols;
            for (int c = 0; c < n; ++c) {
                if (grid[r][c] == target) {
                    cols.push_back(c);
                }
            }

            // Check all pairs of these columns
            int num_cols = cols.size();
            for (int i = 0; i < num_cols; ++i) {
                for (int j = i + 1; j < num_cols; ++j) {
                    int c1 = cols[i];
                    int c2 = cols[j];

                    if (seen[c1][c2]) {
                        found = true;
                        break;
                    }
                    seen[c1][c2] = true;
                }
                if (found) break;
            }
            if (found) break;
        }

        if (found) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    solve();
    
    return 0;
}