/**
 * CSES Problem Set
 * 
 * Problem: Reading Books
 * Link: https://cses.fi/problemset/task/1631
 * Category: Sorting and Searching
 * 
 * Description:
 * Find minimum total time for two people to read n books.
 * 
 * Logic/Approach:
 * If max book > sum of others, answer is 2 * max. Otherwise, answer is sum of all books.
 * as if max book time is greater than sum of all other books, then one person will have to read the max book and the other person will read all other books. So total time will be 2 * max book time.
 * otherwise , both can read books in parallel and total time will be sum of all books, that will surely be minimum time, otherwise one person will have to wait for the other to finish reading the max book, that will increase the total time.
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
