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

class Solution
{
public:
    int uniqueLetterString(string s)
    {
        int n = s.size();

        vector<int> last(26, -1);       // previous occurrence
        vector<int> secondLast(26, -1); // occurrence before previous

        int ans = 0;

        for (int i = 0; i < n; i++)
        {
            int c = s[i] - 'A';

            if (last[c] == -1 && secondLast[c] == -1)
            {
                // First occurrence
                last[c] = i;
            }
            else if (last[c] != -1 && secondLast[c] == -1)
            {
                // Second occurrence

                ans += (i - last[c]) * (last[c] + 1); // last[c] - (-1)

                secondLast[c] = last[c];
                last[c] = i;
            }
            else
            {
                // Third or later occurrence

                ans += (i - last[c]) * (last[c] - secondLast[c]);

                secondLast[c] = last[c];
                last[c] = i;
            }
        }

        // Contribution of the final occurrence of every character
        for (int c = 0; c < 26; c++)
        {
            if (last[c] == -1)
                continue;

            if (secondLast[c] == -1)
                ans += (n - last[c]) * (last[c] + 1);
            else
                ans += (n - last[c]) * (last[c] - secondLast[c]);
        }

        return ans;
    }
};