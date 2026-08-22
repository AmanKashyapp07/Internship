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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> intervals(n);
    for (auto &p : intervals) {
        cin >> p.first >> p.second;
    }

    pair<int, int> newInt;
    cin >> newInt.first >> newInt.second;

    vector<pair<int, int>> result;

    for (auto &interval : intervals) {
        if (interval.second < newInt.first) {
            // No overlap, add as is
            result.push_back(interval);
        } 
        else if (interval.first > newInt.second) {
            // New interval ends before this one starts
            result.push_back(newInt);
            newInt = interval;
        } 
        else {
            // Overlapping - merge
            newInt.first = min(newInt.first, interval.first);
            newInt.second = max(newInt.second, interval.second);
        }
    }

    result.push_back(newInt);

    // Output the result
    for (auto &p : result) {
        cout << p.first << " " << p.second << "\n";
    }

    return 0;
}