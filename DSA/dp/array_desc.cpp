/**
 * CSES Problem Set
 *
 * Problem: CSES Solution Template
 * Link: https://cses.fi/
 * Category: Template
 *
 * Description:
 * A template file containing boilerplate code, fast I/O setup, and common macros/imports for solving CSES problems.
 *
 * Logic/Approach:
 * Provides basic imports, standard namespace, and empty main function with fast I/O configuration.
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

class Solution {
public:
    int size;
    int cols;
    vector<int> input;
    vector<vector<int>> memo;

    int solve(int i, int prev)
    {
        if (i == size)
            return 1;

        if (memo[i][prev + 1] != -1)
            return memo[i][prev + 1];

        long long ans = 0;

        if (input[i] != 0)
        {
            if (prev == -1 || abs(input[i] - prev) <= 1)
                ans = solve(i + 1, input[i]);
        }
        else
        {
            if (prev == -1)
            {
                for (int cur = 1; cur <= cols; cur++)
                {
                    ans = (ans + solve(i + 1, cur)) % MOD;
                }
            }
            else
            {
                for (int cur = max(1, prev - 1); cur <= min(cols, prev + 1); cur++)
                {
                    ans = (ans + solve(i + 1, cur)) % MOD;
                }
            }
        }

        return memo[i][prev + 1] = ans;
    }

    int getArrayDescription(int n, int m, vector<int>& a) {
        size = n;
        cols = m;
        input = a;
        memo.assign(size, vector<int>(cols + 2, -1));
        return solve(0, -1);
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<int> a(n);
    for (int &x : a)
        cin >> x;

    Solution solver;
    long long ans1 = solver.getArrayDescription(n, m, a);
    cout << ans1 << "\n";

    return 0;
}