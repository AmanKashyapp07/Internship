// Link: https://cses.fi/problemset/task/1141
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    set<int> window;
    int left = 0, maxLen = 0;
    for (int right = 0; right < n; right++) {
        while (window.count(a[right])) {
            window.erase(a[left]);
            left++;
        }
        window.insert(a[right]);
        maxLen = max(maxLen, right - left + 1);
    }
    cout << maxLen << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find maximum length of a sequence of unique consecutive songs (CSES 1141).
// - Approach: Two Pointers Sliding Window + Set Tracking.
// - Intuition: Shrink left pointer l while $a[right]$ exists in window set; update max length at each step.
// - Complexity: Time: O(N \log N), Space: O(N).