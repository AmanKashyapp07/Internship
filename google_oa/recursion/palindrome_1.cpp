/**
 * LeetCode 131 - Palindrome Partitioning
 *
 * Description:
 * Given a string s, partition s such that every substring of the partition is a palindrome.
 * Return all possible palindrome partitionings of s.
 *
 * Approach:
 * - Precompute palindrome flags using Dynamic Programming (`pal[i][j]` stores whether `s[i...j]` is a palindrome).
 * - DFS / Backtracking.
 * - From current index `idx`, try to form a palindromic substring ending at `j`. If `pal[idx][j]` is true,
 *   add `s[idx...j]` to the current path, recurse to `dfs(j + 1)`, and then backtrack.
 * - Base Case: When `idx == n`, the entire string has been successfully partitioned.
 *
 * Time Complexity: O(N * 2^N) where N is the length of the string. There are 2^(N-1) possible partition splits.
 * Space Complexity: O(N^2) for the palindrome table and O(N) for recursion depth.
 */

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;

#define all(x)   (x).begin(), (x).end()
#define rall(x)  (x).rbegin(), (x).rend()
#define pb       push_back
#define ff       first
#define ss       second

const int INF  = INT_MAX;
const ll  LINF = LLONG_MAX;
const ll  MOD  = 1e9 + 7;

// ─────────────────────────────────────────────────────────────────────────────

vector<vector<bool>> pal;
vector<vector<string>> ans;
vector<string> cur;
string s;
int n;

void dfs(int idx) {
    if (idx == n) {
        ans.pb(cur);
        return;
    }

    for (int j = idx; j < n; j++) {
        if (!pal[idx][j]) continue;
        cur.pb(s.substr(idx, j - idx + 1)); // s.substr(a,b) gives substring starting at index a of length b
        dfs(j + 1);
        cur.pop_back();
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> s;
    n = s.size();

    pal.assign(n, vector<bool>(n, false));

    // length = 1
    for (int i = 0; i < n; i++)
        pal[i][i] = true;

    // length >= 2
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i + len <= n; i++) {
            int j = i + len - 1;
            if (s[i] == s[j]) {
                pal[i][j] = (len <= 3) || pal[i + 1][j - 1];
            }
        }
    }

    dfs(0);

    for (auto &partition : ans) {
        cout << "[ ";
        for (auto &part : partition)
            cout << part << ' ';
        cout << "]\n";
    }

    return 0;
}
