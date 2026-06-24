#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<vector<string>> ans;
vector<string> path;

void dfs(int start, string& s, vector<vector<bool>>& isPal) {
    int n = s.size();

    if (start == n) {
        ans.push_back(path);
        return;
    }

    for (int end = start; end < n; end++) {
        if (isPal[start][end]) {

            path.push_back(s.substr(start, end - start + 1));

            dfs(end + 1, s, isPal);

            path.pop_back();
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int n = s.size();

    // Precompute palindrome table
    vector<vector<bool>> isPal(n, vector<bool>(n, false));

    for (int i = 0; i < n; i++) {
        isPal[i][i] = true;
    }

    for (int len = 2; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {

            int j = i + len - 1;

            if (len == 2) {
                isPal[i][j] = (s[i] == s[j]);
            } else {
                isPal[i][j] =
                    (s[i] == s[j]) && isPal[i + 1][j - 1];
            }
        }
    }

    dfs(0, s, isPal);

    // Print all partitions
    for (auto& partition : ans) {
        cout << "[ ";
        for (auto& str : partition) {
            cout << "\"" << str << "\" ";
        }
        cout << "]\n";
    }

    return 0;
}