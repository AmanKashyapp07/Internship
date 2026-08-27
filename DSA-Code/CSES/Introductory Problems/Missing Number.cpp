// Link: https://cses.fi/problemset/task/1083
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long n; cin >> n;
    long long sum = n * (n + 1) / 2;
    for (int i = 0; i < n - 1; i++) {
        long long x; cin >> x; sum -= x;
    }
    cout << sum << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the single missing number from 1 to n given n-1 numbers (CSES 1083).
// - Approach: Mathematical sum formula diff — n(n+1)/2 minus sum of given elements.
// - Intuition: The difference between expected total sum and actual sum yields the missing integer in O(1) auxiliary space.
// - Complexity: Time: O(N), Space: O(1).