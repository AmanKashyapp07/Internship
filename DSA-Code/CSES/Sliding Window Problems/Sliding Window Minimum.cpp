// Link: https://cses.fi/problemset/task/3221
#include <bits/stdc++.h>
using namespace std;

struct Node { int idx; long long val; };

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; cin >> n >> k;
    long long x, a, b, c; cin >> x >> a >> b >> c;

    deque<Node> dq;
    long long answer = 0, current = x;

    for (int i = 0; i < n; i++) {
        long long value = current;
        while (!dq.empty() && dq.back().val >= value) dq.pop_back();
        dq.push_back({i, value});
        while (!dq.empty() && dq.front().idx <= i - k) dq.pop_front();

        if (i >= k - 1) answer ^= dq.front().val;
        current = (a * current + b) % c;
    }
    cout << answer << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Compute the XOR sum of minimum values of all sliding windows of size k in a generated sequence (CSES 3221).
// - Approach: Monotonic Increasing Deque storing `{index, value}`.
// - Intuition: Deque front always stores the index of the minimum value in the active window; larger elements behind are popped as they can never be minimum.
// - Complexity: Time: O(N) linear time, Space: O(K).