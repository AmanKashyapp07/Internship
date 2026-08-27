// Link: https://cses.fi/problemset/task/3222
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; cin >> n >> k;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    unordered_map<int, int> freq;
    int distinct = 0;

    for (int i = 0; i < k; i++) {
        if (++freq[a[i]] == 1) distinct++;
    }
    cout << distinct;

    for (int right = k; right < n; right++) {
        int outgoing = a[right - k], incoming = a[right];
        if (--freq[outgoing] == 0) distinct--;
        if (++freq[incoming] == 1) distinct++;
        cout << ' ' << distinct;
    }
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the number of distinct values in every sliding window of size k (CSES 3222).
// - Approach: Fixed Sliding Window of size k with frequency map.
// - Intuition: Sliding right adds incoming element and removes outgoing element; distinct counter increments when frequency goes 0 $\to$ 1 and decrements when 1 $\to$ 0.
// - Complexity: Time: O(N), Space: O(K).