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
    sort(intervals.begin(), intervals.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.second < b.second; // Sort by end time because we want to select intervals that end earlier to maximize the number of non-overlapping intervals.
    });
    for (auto &p : intervals)
    {
        cin >> p.first >> p.second;
    }

    int keep=1;
    int right=intervals[0].second;
    for(int i=1;i<n;i++){
        if(intervals[i].first>right){
            keep++;
            right=intervals[i].second; 
        }
    }
    cout<<keep<<endl; // This is the maximum number of non-overlapping intervals that can be selected from the given set of intervals., for min number of intervals to remove, it will be n-keep
}
