/**
 * LeetCode 354 - Russian Doll Envelopes
 *
 * Description:
 * Given a 2D array of integers envelopes where envelopes[i] = [wi, hi] represents the width and height of an envelope.
 * One envelope can fit into another if and only if both the width and height of one envelope are greater than the other.
 * Return the maximum number of envelopes you can Russian doll (i.e., put one inside another).
 *
 * Approach:
 * - Sort the envelopes by width in ascending order. If widths are equal, sort heights in descending order.
 * - By sorting heights descending for equal widths, we ensure that we cannot use more than one envelope of the same width (since height sequence would be decreasing and LIS cannot include them).
 * - Find the Longest Increasing Subsequence (LIS) on the height dimension using binary search (O(n log n) tails array approach).
 *
 * Time Complexity: O(n log n)
 * Space Complexity: O(n)
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


class Solution {
public:
    int maxEnvelopes(vector<vector<int>>& envelopes) {
        if (envelopes.empty()) return 0;
        
        // 1. Sort: Width ascending, Height descending when widths match
        sort(envelopes.begin(), envelopes.end(), [](const vector<int>& a, const vector<int>& b) {
            if (a[0] == b[0]) {
                return a[1] > b[1]; // Descending height
            }
            return a[0] < b[0]; // Ascending width
        });
        
        // 2. Perform O(N log N) LIS on heights using binary search
        vector<int> lis;
        
        for (const auto& env : envelopes) {
            int height = env[1];
            
            // Find the insertion point for the current height
            auto it = lower_bound(lis.begin(), lis.end(), height); // height can replace the first element in lis that is >= height
            
            if (it == lis.end()) {
                // If height is greater than all elements in lis, extend the subsequence
                lis.push_back(height);
            } else {
                // Otherwise, replace the element to maintain the smallest possible tail values
                *it = height;
            }
        }
        
        return lis.size();
    }
};


