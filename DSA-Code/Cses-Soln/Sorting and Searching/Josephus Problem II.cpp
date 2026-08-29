// Link: https://cses.fi/problemset/task/2163
#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n; vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void add(int i, int v) { for (i++; i <= n; i += i & -i) bit[i] += v; }
    int query(int i) { int s = 0; for (i++; i > 0; i -= i & -i) s += bit[i]; return s; }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k; cin >> n >> k;
    Fenwick bit(n);
    for (int i = 0; i < n; i++) bit.add(i, 1);

    int cur = 0;
    for (int rem = n; rem > 0; rem--) {
        cur = (cur + k) % rem;
        int need = cur + 1;
        int low = 0, high = n - 1, ans = n - 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (bit.query(mid) >= need) { ans = mid; high = mid - 1; }
            else low = mid + 1;
        }
        cout << ans + 1 << (rem == 1 ? '\n' : ' ');
        bit.add(ans, -1);
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Output removal order of n children in a circle where every k-th child is removed (CSES 2163).
// - Approach: Binary Search on Fenwick Tree prefix sums ($O(N \log^2 N)$).
// - Intuition: Modular arithmetic finds ordinal index of target child among remaining; Fenwick tree prefix sum locates actual 0-indexed position.
// - Complexity: Time: O(N \log^2 N), Space: O(N).