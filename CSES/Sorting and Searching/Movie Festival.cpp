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

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    vector<pair<int, int>> intervals(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> intervals[i].first >> intervals[i].second;
    }
    // Sort intervals by their starting point, and if they are same, only store shortest finishing time
    sort(intervals.begin(), intervals.end(), [](const pair<int, int> &a, const pair<int, int> &b)
         {
             if (a.first == b.first)
             {
                 return a.second < b.second; // If starting points are same, sort by finishing time
             }
             return a.first < b.first; // Otherwise, sort by starting point
         });
    map<int, int> mp;

    for (auto &[start, end] : intervals)
    {

        if (!mp.count(start))

            mp[start] = end;

        else

            mp[start] = min(mp[start], end);
    }

    intervals.clear();

    for (auto &[start, end] : mp)
    {
        intervals.push_back({start, end});
    }
    int count=0;
    int ans=0;
    for(auto &[start, end] : intervals)
    {
        if (count == 0 || start >= count)
        {
            count = end;
            ans++;
        }
        else
        {
            count = min(count, end);
        }
    }
    cout << ans << '\n';


    return 0;
}
