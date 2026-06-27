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

// ─────────────────────────────────────────────────────────────────────────────

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    sort(all(a));
    int low = 0, high = n - 1;
    int ans = 0;
    int low = 0, high = n - 1;
    int ans = 0;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (mid > 0 && mid < n - 1 && a[mid] != a[mid + 1] &&
            a[mid] != a[mid - 1])
        {
            ans = a[mid];
            break;
        }

        if (mid == 0)
        {
            if (n == 1 || a[mid] != a[mid + 1])
            {
                ans = a[mid];
                break;
            }
            low = mid + 2;
        }
        else if (mid == n - 1)
        {
            if (a[mid] != a[mid - 1])
            {
                ans = a[mid];
                break;
            }
            high = mid - 2;
        }
        else if (a[mid] == a[mid + 1])
        {
            if (mid % 2 == 0)
            {
                low = mid + 2;
            }
            else
            {
                high = mid - 1;
            }
        }
        else if (a[mid] == a[mid - 1])
        {
            if (mid % 2 == 0)
            {
                high = mid - 2;
            }
            else
            {
                low = mid + 1;
            }
        }
    }
    cout << ans << '\n';
    return 0;
    return ans;

    return 0;
}
