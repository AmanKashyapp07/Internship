// Link: https://cses.fi/problemset/task/3426
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long n, k; cin >> n >> k;
    long long x, a, b, c; cin >> x >> a >> b >> c;

    deque<long long> window;
    long long curXor = 0, cur = x;

    for (long long i = 1; i <= k; i++) {
        window.push_back(cur);
        curXor ^= cur;
        if (i < n) cur = (a * cur + b) % c;
    }
    long long answer = curXor;

    for (long long i = k + 1; i <= n; i++) {
        curXor ^= window.front(); window.pop_front();
        curXor ^= cur; window.push_back(cur);
        answer ^= curXor;
        if (i < n) cur = (a * cur + b) % c;
    }
    cout << answer << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Compute overall XOR combination of bitwise XOR sums across all sliding windows of size k (CSES 3426).
// - Approach: Sliding Window Deque XOR Maintenance (`curXor ^= incoming ^ outgoing`).
// - Intuition: XORing out outgoing element and XORing in incoming element updates sliding window XOR state in $O(1)$ time.
// - Complexity: Time: O(N), Space: O(K).