#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MOD = 1000000007;
const ll BASE = 911382323;

struct Fenwick {
    int n;
    vector<ll> bit;

    Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void update(int idx, ll val) {
        idx++;

        while (idx <= n) {
            bit[idx] = (bit[idx] + val) % MOD;
            if (bit[idx] < 0)
                bit[idx] += MOD;

            idx += idx & -idx;
        }
    }

    ll query(int idx) {
        ll res = 0;
        idx++;

        while (idx > 0) {
            res = (res + bit[idx]) % MOD;
            idx -= idx & -idx;
        }

        return res;
    }

    ll query(int l, int r) {
        ll res = query(r);

        if (l)
            res = (res - query(l - 1) + MOD) % MOD;

        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    string s;
    cin >> s;

    auto val = [](char c) {
        return 1LL * (c - 'a' + 1);
    };

    vector<ll> pw(n + 1);
    pw[0] = 1;

    for (int i = 1; i <= n; i++)
        pw[i] = pw[i - 1] * BASE % MOD;

    // Forward hash over the original string.
    Fenwick fw(n);

    // Forward hash over the reversed string.
    Fenwick bw(n);

    /*

    We maintain two Fenwick trees.

    1. fw (forward):

       Stores the polynomial hash of the original string.

       Character s[i] contributes

           value(s[i]) * BASE^i

       Using this tree, we can obtain the hash of any substring

       s[l...r] in O(log n).

    2. bw (backward):

       Stores the polynomial hash of the reversed string.

       Instead of actually reversing the string, character s[i] is

       inserted at index (n - 1 - i).

       Thus, querying bw over the corresponding reversed interval gives

       the hash of the reversed substring.

    Why do we need both?

    To check whether s[l...r] is a palindrome, we need to compare

        substring == reverse(substring)

    The forward tree gives

        hash(s[l...r])

    while the backward tree gives

        hash(reverse(s[l...r]))

    If these two hashes (after exponent alignment) are equal,

    then the substring is a palindrome.

    A single Fenwick tree can only answer hashes in one direction.

    It cannot efficiently produce the hash of the reversed substring,

    which is why a second tree is required.

*/

    // Build both Fenwick trees.
    for (int i = 0; i < n; i++) {
        fw.update(i, val(s[i]) * pw[i] % MOD);
        int revIdx = n - 1 - i;
        bw.update(revIdx, val(s[i]) * pw[revIdx] % MOD);
    }

    while (m--) {
        int type;
        cin >> type;

        if (type == 1) {
            int k;
            char c;
            cin >> k >> c;
            k--;

            ll delta = val(c) - val(s[k]);
            fw.update(k, delta * pw[k] % MOD);
            int revIdx = n - 1 - k;
            bw.update(revIdx, delta * pw[revIdx] % MOD);
            s[k] = c;
        }
        else {
            int l, r;
            cin >> l >> r;
            l--, r--;

            ll forwardHash = fw.query(l, r);

            int revL = n - 1 - r;
            int revR = n - 1 - l;

            ll backwardHash = bw.query(revL, revR);

            /*
                The polynomial hash stores each character as:

                    s[i] * BASE^i

                Therefore, the exponent depends on the ORIGINAL index.

                Example:
                    s = "abacabae"

                    Query substring [2, 6] (0-indexed):
                        "acaba"

                    Forward hash:
                        a*BASE^2 + c*BASE^3 + a*BASE^4 +
                        b*BASE^5 + a*BASE^6

                    In the reversed string, this same substring occupies
                    indices [1, 5], so its hash is

                        a*BASE^1 + b*BASE^2 + a*BASE^3 +
                        c*BASE^4 + a*BASE^5

                    The characters are the same, but every exponent is
                    shifted by 1.

                    Since polynomial hashes can only be compared when the
                    exponents match, we multiply the hash that starts earlier
                    by BASE^(difference in starting indices).

                    After shifting:

                        a*BASE^2 + b*BASE^3 + a*BASE^4 +
                        c*BASE^5 + a*BASE^6

                    Now both hashes are aligned and can be compared directly.
            */

            if (l < revL) forwardHash = forwardHash * pw[revL - l] % MOD;
            else if (revL < l) backwardHash = backwardHash * pw[l - revL] % MOD;
            cout << (forwardHash == backwardHash ? "YES\n" : "NO\n");
        }
    }
}