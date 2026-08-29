// Link: https://cses.fi/problemset/task/3420
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    unordered_map<int, int> freq;
    long long ans = 0;
    int l = 0;

    for (int r = 0; r < n; r++) {
        freq[a[r]]++;
        while (freq[a[r]] > 1) {
            freq[a[l]]--;
            l++;
        }
        ans += (r - l + 1);
    }
    cout << ans << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Count subarrays where all elements are distinct (CSES 3420).
// - Approach: Two Pointers / Variable Sliding Window.
// - Intuition: Advance right pointer r and advance left pointer l while duplicate $a[r]$ exists; number of valid unique subarrays ending at r equals $(r - l + 1)$.
// - Complexity: Time: O(N), Space: O(N).