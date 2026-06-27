/**
 * Minimum Insertions to Make a String Palindrome
 * Using LCS (Recursion + Memoization)
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
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

int lcs(int i, int j, const string &s1, const string &s2,
        vector<vector<int>> &dp) {

    if (i == 0 || j == 0)
        return 0;

    if (dp[i][j] != -1)
        return dp[i][j];

    if (s1[i - 1] == s2[j - 1]) {
        return dp[i][j] = 1 + lcs(i - 1, j - 1, s1, s2, dp);
    }

    return dp[i][j] = max(
        lcs(i - 1, j, s1, s2, dp),
        lcs(i, j - 1, s1, s2, dp)
    );
}

int minInsertionsToMakePalindrome(const string &s) {
    string reversed = s;
    reverse(reversed.begin(), reversed.end());

    int n = s.size();

    vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));

    int lpsLength = lcs(n, n, s, reversed, dp);

    return n - lpsLength;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    // logic is answer is n - lps(s) where lps is longest palindromic subsequence, and lps(s) = lcs(s, reverse(s)), so we can use lcs to find lps and then find answer
    cout << minInsertionsToMakePalindrome(s) << '\n';

    return 0;
}