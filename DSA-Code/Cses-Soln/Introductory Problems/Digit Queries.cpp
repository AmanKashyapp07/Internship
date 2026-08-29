// Link: https://cses.fi/problemset/task/2431
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int q; cin >> q;
    while (q--) {
        long long k; cin >> k;
        long long len = 1, count = 9, start = 1;
        while (k > len * count) {
            k -= len * count;
            len++;
            count *= 10;
            start *= 10;
        }
        start += (k - 1) / len;
        string s = to_string(start);
        cout << s[(k - 1) % len] << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the digit at 1-based index k in the infinite string "123456789101112..." (CSES 2431).
// - Approach: Digit Length Range Decompositon + Arithmetic Offset Indexing.
// - Intuition: Determine number length range (1-digit: 9, 2-digit: 90, 3-digit: 900...), subtract full buckets, compute target number and exact digit offset.
// - Complexity: Time: O(log_10 K) per query, Space: O(1).