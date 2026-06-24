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
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;

#define all(x)   (x).begin(), (x).end()
#define rall(x)  (x).rbegin(), (x).rend()
#define pb       push_back
#define ff       first
#define ss       second

const int INF  = INT_MAX;
const ll  LINF = LLONG_MAX;
const ll  MOD  = 1e9 + 7;

// ─────────────────────────────────────────────────────────────────────────────

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    vector<int> nums = {2, 0, 2, 1, 1, 0};
    vector<int> nums2 = {1, 0, 2};
    sort(nums.begin(), nums.end());
    sort(nums2.begin(), nums2.end());
    int n = nums.size();
    int m = nums2.size();
    vector<int> merged(n + m);
    int i = 0, j = 0, k = 0;
    while (i < n && j < m) {
        if (nums[i] < nums2[j]) {
            merged[k++] = nums[i++];
        }
        else {            
            merged[k++] = nums2[j++];
        }
    }
    // Copy any remaining elements from either array
    while (i < n) {
        merged[k++] = nums[i++];
    }
    while (j < m) {
        merged[k++] = nums2[j++];
    }

    // Print the merged array
    for (int x : merged) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}
