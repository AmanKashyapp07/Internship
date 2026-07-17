// CSES Problem: <problem name>
// https://cses.fi/problemset/task/<id>

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
#define int long long
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────

signed main()
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

    int sum=0;
    int mx=0;
    for (int i = 0; i < n; i++)
    {
        sum += a[i];
        mx = max(mx, a[i]);
    }
    cout << max(2*mx, sum) << endl;
    return 0;
}
