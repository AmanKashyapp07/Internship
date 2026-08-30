// Link: https://cses.fi/problemset/task/1142
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<ll> h(n + 1);
    for (int i = 0; i < n; i++) cin >> h[i];
    h[n] = 0; // Sentinel

    stack<int> st;
    ll maxArea = 0;

    for (int i = 0; i <= n; i++) {
        while (!st.empty() && h[st.top()] > h[i]) {
            ll height = h[st.top()];
            st.pop();
            ll width = st.empty() ? i : i - st.top() - 1;
            maxArea = max(maxArea, height * width);
        }
        st.push(i);
    }

    cout << maxArea << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find largest rectangular area that can be formed in a histogram of N bars (CSES 1142 / LeetCode 84).
// - Approach: Monotonic Stack (Largest Rectangle in Histogram).
// - Intuition: Maintain an increasing stack of bar indices. A bar's width span is bounded on the left by the stack predecessor and on the right by the current smaller bar.
// - Complexity: Time: O(N), Space: O(N).
