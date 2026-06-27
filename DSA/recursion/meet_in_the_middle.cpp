/**
 * CSES Problem Set
 * 
 * Problem: Meet in the Middle
 * Link: https://cses.fi/problemset/task/1628
 * Category: Advanced Problems
 * 
 * Description:
 * Count subsets summing up to x.
 * 
 * Logic/Approach:
 * Split array into two halves. Generate all subset sums for both halves, sort, and binary search.
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


const int MOD  = 1e9 + 7;
// ─────────────────────────────────────────────────────────────────────────────
vector<int> generateSums(const vector<int>& arr) {
    vector<int> sums;
    int n = arr.size();
    for (int mask = 0; mask < (1 << n); mask++) {
        int sum = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                sum += arr[i];
            }
        }
        sums.push_back(sum);
    }
    return sums;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,x;
    cin >> n >> x;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    vector<int> left, right;
    left=generateSums(vector<int>(a.begin(), a.begin() + n / 2));
    right=generateSums(vector<int>(a.begin() + n / 2, a.end()));

    sort(right.begin(), right.end());
    map<int,int> rightCount;
    for (int sum : right) rightCount[sum]++;
    int count = 0;
    for (int sum : left) {
        int target = x - sum;
        count += rightCount[target];
    }
    cout << count << "\n";
    return 0;
}
