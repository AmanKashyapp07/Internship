// Link: https://cses.fi/problemset/task/1641
#include <bits/stdc++.h>
using namespace std;

struct Number { long long val; int idx; };

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; long long x; cin >> n >> x;
    vector<Number> a(n);
    for (int i = 0; i < n; i++) { cin >> a[i].val; a[i].idx = i + 1; }

    sort(a.begin(), a.end(), [](const Number& p, const Number& q) { return p.val < q.val; });

    for (int i = 0; i < n; i++) {
        long long target = x - a[i].val;
        int l = i + 1, r = n - 1;
        while (l < r) {
            long long sum = a[l].val + a[r].val;
            if (sum == target) {
                cout << a[i].idx << ' ' << a[l].idx << ' ' << a[r].idx << '\n';
                return 0;
            }
            if (sum < target) l++;
            else r--;
        }
    }
    cout << "IMPOSSIBLE\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find 3 distinct indices such that $a[i] + a[j] + a[k] = x$ (CSES 1641).
// - Approach: Sort + Fix 1 Element + Two Pointers for Remaining Pair ($O(N^2)$).
// - Intuition: Fixing $a[i]$ reduces problem to 2-Sum target $(x - a[i])$ solved in $O(N)$ with two pointers.
// - Complexity: Time: O(N^2), Space: O(N).