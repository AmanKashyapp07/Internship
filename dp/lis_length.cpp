/**
 * Problem: Longest Increasing Subsequence (https://cses.fi/problemset/task/1145)
 * Find the length of the longest increasing subsequence.
 * Time: O(N log N) time, O(N) space.
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n; if (!(cin >> n)) return 0;
    vector<long long> tails;
    // Patience sorting: tails[i] is the smallest tail of increasing subsequences of length i+1
    for (int i = 0; i < n; i++) {
        long long x; cin >> x;
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x; // Overwrite tail to keep elements as small as possible
    }
    cout << tails.size() << '\n';
}
