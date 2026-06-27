/**
 * Problem: Maximum Product Subarray
 * Link: https://leetcode.com/problems/maximum-product-subarray/
 * Category: Array / Dynamic Programming
 * 
 * Description:
 * Find the contiguous subarray with the maximum product.
 * 
 * Logic/Approach:
 * Tracks running max and min products to handle negative signs.
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
    vector<int> arr(n);
    for (int i = 0; i < n; i++) cin >> arr[i];
    int maxProduct = arr[0]; // stores max product of subarray ending at current index, this is needed because if we have negative number in array, then max product can become min product if we multiply it with negative number
    int minProduct = arr[0]; // stores min product of subarray ending at current index, this is needed because if we have negative number in array, then min product can become max product if we multiply it with negative number
    int currentProduct = arr[0];
    for (int i = 1; i < n; i++)
    {
        if(arr[i]<0) swap(maxProduct, minProduct); // if we have negative number, then max product can become min product and min product can become max product
        maxProduct = max(arr[i], maxProduct * arr[i]); // max product of sub
        minProduct = min(arr[i], minProduct * arr[i]); // min product of sub
        currentProduct = max(currentProduct, maxProduct); // update current product
    }
    cout << currentProduct << '\n'; // print max product of subarray


    return 0;
}
