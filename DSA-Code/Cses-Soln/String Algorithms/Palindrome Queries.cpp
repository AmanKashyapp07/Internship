// Link: https://cses.fi/problemset/task/2420
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7, BASE = 911382323;

struct Fenwick {
    int n; vector<long long> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void add(int i, long long val) {
        for (i++; i <= n; i += i & -i) bit[i] = (bit[i] + val % MOD + MOD) % MOD;
    }
    long long query(int i) {
        long long res = 0;
        for (i++; i > 0; i -= i & -i) res = (res + bit[i]) % MOD;
        return res;
    }
    long long queryRange(int l, int r) {
        return (query(r) - (l > 0 ? query(l - 1) : 0) + MOD) % MOD;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    string s; cin >> s;

    vector<long long> pw(n + 1, 1);
    for (int i = 1; i <= n; i++) pw[i] = pw[i - 1] * BASE % MOD;

    Fenwick fw(n), bw(n);
    for (int i = 0; i < n; i++) {
        long long v = s[i] - 'a' + 1;
        fw.add(i, v * pw[i]);
        bw.add(n - 1 - i, v * pw[n - 1 - i]);
    }

    while (m--) {
        int type; cin >> type;
        if (type == 1) {
            int k; char c; cin >> k >> c; k--;
            long long delta = (c - 'a' + 1) - (s[k] - 'a' + 1);
            fw.add(k, delta * pw[k]);
            bw.add(n - 1 - k, delta * pw[n - 1 - k]);
            s[k] = c;
        } else {
            int l, r; cin >> l >> r; l--; r--;
            long long fwd = fw.queryRange(l, r);
            int revL = n - 1 - r, revR = n - 1 - l;
            long long bwd = bw.queryRange(revL, revR);
            if (l < revL) fwd = fwd * pw[revL - l] % MOD;
            else if (revL < l) bwd = bwd * pw[l - revL] % MOD;
            cout << (fwd == bwd ? "YES\n" : "NO\n");
        }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Process dynamic point character edits and substring palindrome queries on s (CSES 2420).
// - Approach: Dual Fenwick Trees tracking forward and reversed rolling hashes.
// - Intuition: Forward BIT tracks `s[i] * BASE^i`, reversed BIT tracks `s[i] * BASE^{n-1-i}`; exponent alignment via power multiplication enables $O(\log N)$ palindrome checks.
// - Complexity: Time: O((N + M) \log N), Space: O(N).