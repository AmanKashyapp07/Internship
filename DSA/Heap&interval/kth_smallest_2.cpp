// Merge two sorted arrays of sums and keep only the smallest k sums.
//
// Suppose:
// a = smallest sums formed using the first i rows
// b = next row of the matrix
//
// We want the smallest k values of
//
//      a[i] + b[j]
//
// without generating all |a| * |b| combinations.

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


vector<int> merge(vector<int>& a, vector<int>& b, int k) {

    // Min-heap:
    // {current_sum, {index_in_a, index_in_b}}
    priority_queue<
        pair<int, pair<int,int>>,
        vector<pair<int, pair<int,int>>>,
        greater<>
    > pq;

    vector<int> ans;

    // Initially pair every element of a with ONLY the first element of b.
    //
    // Example:
    // a = [3,5,8]
    // b = [2,4,6]
    //
    // Push:
    // (3+2)
    // (5+2)
    // (8+2)
    //
    // Why?
    // Since b is sorted,
    // 2 is always the smallest partner for every element of a.
    for (int i = 0; i < min((int)a.size(), k); i++) {
        pq.push({a[i] + b[0], {i, 0}});
    }

    // Extract the smallest sums one by one.
    while (!pq.empty() && ans.size()!=k) {

        auto [sum, pos] = pq.top();
        pq.pop();

        auto [i, j] = pos;

        // This is currently the smallest unseen sum.
        ans.push_back(sum);

        // We have already used
        //
        //      a[i] + b[j]
        //
        // Now try the next element in row b.
        //
        //      a[i] + b[j+1]
        //
        // Example:
        //
        // 3+2  <-- popped
        // 3+4  <-- push next
        //
        // We never need to push
        // 3+6 immediately.
        if (j + 1 < b.size()) {
            pq.push({a[i] + b[j + 1], {i, j + 1}});
        }
    }

    return ans;
}

int kthSmallest(vector<vector<int>>& mat, int k) {

    // Initially the smallest sums are simply the first row itself.
    vector<int> cur = mat[0];

    // We only care about the first k values.
    if (cur.size() > k) cur.resize(k);

    // Merge rows one by one.
    //
    // After merging row i,
    // cur contains the smallest k sums
    // using rows [0...i].
    for (int i = 1; i < mat.size(); i++) {
        cur = merge(cur, mat[i], k);
    }

    // The kth smallest overall sum.
    return cur[k - 1];
}