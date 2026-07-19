// CSES Problem: Ferris Wheel
// https://cses.fi/problemset/task/1090

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
    
    int n, x;
    cin >> n >> x;
    
    multiset<int> ms;
    for (int i = 0; i < n; i++)
    {
        int weight;
        cin >> weight;
        ms.insert(weight); // Elements are automatically sorted
    }
    
    int gondolas = 0;

    while (!ms.empty())
    {
        // 1. Take the heaviest available person
        auto it = prev(ms.end());
        int heaviest = *it;
        ms.erase(it); 

        // 2. Find the largest remaining person who can pair with them
        int target = x - heaviest;
        auto match = ms.upper_bound(target);

        // 3. If a valid pair exists, remove them from the available pool
        if (match != ms.begin())
        {
            match--; // Step back to get the largest element <= target
            ms.erase(match);
        }

        // Regardless of whether they are paired or alone, they use one gondola
        gondolas++;
    }

    cout << gondolas << '\n';
    return 0;
}