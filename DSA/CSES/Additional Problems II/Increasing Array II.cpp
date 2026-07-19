// Link: https://cses.fi/problemset/task/2132

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Max heap stores the current candidate medians.
    priority_queue<ll> medianHeap;

    ll answer = 0;

    for (int i = 0; i < n; i++) {
        ll x;
        cin >> x;

        // -------------------------------------------------------
        // Step 1:
        // Every new number becomes a candidate median.
        // -------------------------------------------------------
        medianHeap.push(x);

        // -------------------------------------------------------
        // Step 2:
        // If the largest candidate median is greater than x,
        // the monotonicity constraint is violated.
        //
        // We must merge two blocks.
        // -------------------------------------------------------
        ll currentMedian = medianHeap.top();

        if (currentMedian > x) {

            // Amount we are forced to pay.
            answer += currentMedian - x;

            // The old median is no longer valid.
            medianHeap.pop();

            // x becomes the new median after merging.
            medianHeap.push(x);
        }
    }

    cout << answer << '\n';
}