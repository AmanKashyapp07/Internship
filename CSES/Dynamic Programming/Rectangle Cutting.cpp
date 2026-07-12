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

#define int long long
using namespace std;

// Replacing the vector with a plain array stops the compiler from crashing
int memo[505][505];


const int INF = 4e18;



signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int a, b;
    cin >> a >> b;
    int dp[a + 1][b + 1];
    for(int i = 0; i <= a; i++)
    {
        for (int j = 0; j <= b; j++)
        {
            if(i == j)
                dp[i][j] = 1;
            else if(i == 0 || j == 0)
                dp[i][j] = 0;
            else
            {
                int ans = INF;
                for(int k = 1; k < i; k++)
                    ans = min(ans, dp[k][j] + dp[i - k][j]);
                for(int k = 1; k < j; k++)
                    ans = min(ans, dp[i][k] + dp[i][j - k]);
                dp[i][j] = ans;
            }
        }
    }
    cout << dp[a][b] - 1 << "\n";
    return 0;
}
