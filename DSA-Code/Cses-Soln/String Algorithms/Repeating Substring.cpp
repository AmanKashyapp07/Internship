// Link: https://cses.fi/problemset/task/2106
#include <bits/stdc++.h>
using namespace std;

const long long MOD1 = 1e9 + 7, MOD2 = 1e9 + 9, P = 31;

struct FastHash {
    vector<long long> p1, p2, h1, h2;
    FastHash(const string &s) {
        int n = s.size();
        p1.assign(n + 1, 1); p2.assign(n + 1, 1);
        h1.assign(n + 1, 0); h2.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            p1[i + 1] = p1[i] * P % MOD1; p2[i + 1] = p2[i] * P % MOD2;
            h1[i + 1] = (h1[i] * P + (s[i] - 'a' + 1)) % MOD1;
            h2[i + 1] = (h2[i] * P + (s[i] - 'a' + 1)) % MOD2;
        }
    }
    pair<long long, long long> get(int l, int r) {
        long long x1 = (h1[r + 1] - h1[l] * p1[r - l + 1] % MOD1 + MOD1) % MOD1;
        long long x2 = (h2[r + 1] - h2[l] * p2[r - l + 1] % MOD2 + MOD2) % MOD2;
        return {x1, x2};
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; cin >> s;
    int n = s.size();
    FastHash H(s);

    auto check = [&](int len) {
        map<pair<long long, long long>, int> mp;
        for (int i = 0; i + len <= n; i++) {
            auto h = H.get(i, i + len - 1);
            if (mp.count(h)) return i;
            mp[h] = i;
        }
        return -1;
    };

    int lo = 1, hi = n, bestLen = 0, bestPos = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        int pos = check(mid);
        if (pos != -1) { bestLen = mid; bestPos = pos; lo = mid + 1; }
        else hi = mid - 1;
    }

    if (bestLen == 0) cout << -1 << '\n';
    else cout << s.substr(bestPos, bestLen) << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Find the longest substring of s that occurs at least twice (CSES 2106).
// - Approach: Binary Search on Answer length + Double Rolling Hash Map verification.
// - Intuition: If a repeating substring of length $L$ exists, then repeating substrings of length $< L$ also exist; predicate monotonicity enables binary search.
// - Complexity: Time: O(N \log^2 N), Space: O(N).