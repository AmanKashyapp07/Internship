// Link: https://cses.fi/problemset/task/1744

#include <iostream>
#include <algorithm>
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