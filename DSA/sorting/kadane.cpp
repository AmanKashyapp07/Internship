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

// ─────────────────────────────────────────────────────────────────────────────

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n))
        return 0;

    vector<int> arr(n);
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    int sum = 0, ans = INT_MIN;
    int i = 0, l = 0, r = 0;

    for (int j = 0; j < n; j++)
    {
        sum += arr[j];

        if (sum > ans)
        {
            ans = sum;
            l = i;
            r = j;
        }

        if (sum < 0) // if sum becomes negative, we reset the sum and move the starting index to the next element
        {
            sum = 0;
            i = j + 1;
        }
    }

    cout<<"max sum is: "<<ans<<"\n";
    cout<<"starting and end point of subarray is: "<<l<<" "<<r<<"\n";

    return 0;
}