// Link: https://cses.fi/problemset/task/1191
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; long long k; cin >> n >> k;
    vector<long long> a(2 * n);
    for (int i = 0; i < n; i++) { cin >> a[i]; a[i + n] = a[i]; }

    vector<int> nxt(2 * n + 1);
    long long sum = 0; int r = 0;
    for (int l = 0; l < 2 * n; l++) {
        while (r < 2 * n && sum + a[r] <= k) sum += a[r++];
        nxt[l] = r;
        sum -= a[l];
    }
    nxt[2 * n] = 2 * n;

    const int LOG = 20;
    vector<vector<int>> up(LOG, vector<int>(2 * n + 1));
    for (int i = 0; i <= 2 * n; i++) up[0][i] = nxt[i];
    for (int j = 1; j < LOG; j++) {
        for (int i = 0; i <= 2 * n; i++) up[j][i] = up[j - 1][up[j - 1][i]];
    }

    int minBlocks = INT_MAX;
    for (int start = 0; start < n; start++) {
        int cur = start, cnt = 0;
        for (int j = LOG - 1; j >= 0; j--) {
            if (up[j][cur] < start + n) {
                cur = up[j][cur];
                cnt += (1 << j);
            }
        }
        minBlocks = min(minBlocks, cnt + 1);
    }
    cout << minBlocks << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Split a circular array of n elements into minimum number of contiguous subarrays each having sum <= k (CSES 1191).
// - Approach: Doubled Array Sliding Window + Binary Lifting `up[j][i]` jumps.
// - Intuition: Compute next valid jump position for each start index using two pointers; jump $2^j$ steps via binary lifting to span $N$ elements.
// - Complexity: Time: O(N \log N), Space: O(N \log N).