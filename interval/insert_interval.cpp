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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> intervals(n);
    for (auto &p : intervals)
    {
        cin >> p.first >> p.second;
    }

    pair<int, int> newInterval;
    cin >> newInterval.first >> newInterval.second;

    // sort(intervals.begin(), intervals.end());

    vector<pair<int, int>> result;

    for (auto &interval : intervals)
    {
        // completely before
        if (interval.second < newInterval.first)
        {
            result.push_back(interval);
        }

        // completely after
        else if (interval.first > newInterval.second)
        {
            result.push_back(newInterval);
            newInterval = interval; // in future, this will not affect other intervals, as in question it is given that intervals are non-overlapping and sorted. we can also stop here
        }

        // overlap
        else
        {
            newInterval.first = min(newInterval.first, interval.first);
            newInterval.second = max(newInterval.second, interval.second);
        }
    }

    result.push_back(newInterval);
}
