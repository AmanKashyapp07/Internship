// Link: https://cses.fi/problemset/task/1628
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; long long x; cin >> n >> x;
    vector<long long> a(n);
    for (long long &v : a) cin >> v;

    auto genSums = [](const vector<long long>& arr) {
        int sz = arr.size();
        vector<long long> sums;
        for (int mask = 0; mask < (1 << sz); mask++) {
            long long s = 0;
            for (int i = 0; i < sz; i++) if (mask & (1 << i)) s += arr[i];
            sums.push_back(s);
        }
        return sums;
    };

    auto left = genSums(vector<long long>(a.begin(), a.begin() + n / 2));
    auto right = genSums(vector<long long>(a.begin() + n / 2, a.end()));
    sort(right.begin(), right.end());

    long long count = 0;
    for (long long s : left) {
        long long target = x - s;
        count += upper_bound(right.begin(), right.end(), target)
               - lower_bound(right.begin(), right.end(), target);
    }
    cout << count << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count number of subsets summing exactly to x (CSES 1628).
// - Approach: Meet in the Middle — enumerate all $2^{N/2}$ sums for each half, sort right half, binary search.
// - Intuition: Full $O(2^N)$ is too slow; splitting into two halves reduces search space to $O(2^{N/2})$ per half, with sorted binary search matching.
// - Complexity: Time: O(2^{N/2} \cdot N/2), Space: O(2^{N/2}).
