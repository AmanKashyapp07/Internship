// Link: https://cses.fi/problemset/task/2428
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; cin >> n >> k;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    unordered_map<int, int> freq;
    long long ans = 0;
    int l = 0;

    for (int r = 0; r < n; r++) {
        freq[a[r]]++;
        while ((int)freq.size() > k) {
            if (--freq[a[l]] == 0) freq.erase(a[l]);
            l++;
        }
        ans += (r - l + 1);
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count subarrays with at most k distinct values (CSES 2428).
// - Approach: Two Pointers / Variable Sliding Window + Frequency Map.
// - Intuition: Expanding right pointer r and shrinking left pointer l when distinct count exceeds k ensures every window $[l..r]$ has at most k distinct elements.
// - Complexity: Time: O(N), Space: O(K).