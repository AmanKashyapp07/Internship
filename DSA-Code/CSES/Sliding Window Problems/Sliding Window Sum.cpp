// Link: https://cses.fi/problemset/task/3220
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; cin >> n >> k;
    long long x, a, b, c; cin >> x >> a >> b >> c;

    vector<long long> window(k);
    long long windowSum = 0, answer = 0, current = x;

    for (int i = 0; i < k; i++) {
        window[i] = current;
        windowSum += current;
        current = (a * current + b) % c;
    }
    answer ^= windowSum;

    int pos = 0;
    for (int i = k; i < n; i++) {
        windowSum -= window[pos];
        windowSum += current;
        window[pos] = current;
        pos = (pos + 1) % k;

        answer ^= windowSum;
        current = (a * current + b) % c;
    }
    cout << answer << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Compute XOR sum of all sliding window sums of size k in a generated sequence (CSES 3220).
// - Approach: Circular Buffer Sliding Window Sum (`windowSum += incoming - outgoing`).
// - Intuition: Circular buffer array of size k tracks outgoing elements in $O(1)$ space and time per sliding window transition.
// - Complexity: Time: O(N), Space: O(K).