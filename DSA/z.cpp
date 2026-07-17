#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MOD = 1000000007;
const ll P = 31;

struct FastHash {
    vector<ll> power, hash;

    FastHash(const string &s) {
        int n = s.size();
        power.assign(n + 1, 1);
        hash.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            power[i + 1] = power[i] * P % MOD;
            hash[i + 1] = (hash[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }

    ll get(int l, int r) {
        return (hash[r + 1] - hash[l] * power[r - l + 1] % MOD + MOD) % MOD;
    }
};

vector<int> z_array(const string &s) {
    int n = s.size();
    vector<int> z(n, 0);

    if (n == 0) return z;

    FastHash fh(s);

    for (int i = 1; i < n; i++) {
        int lo = 0, hi = n - i, best = 0;

        while (lo <= hi) {
            int mid = (lo + hi) / 2;

            if (mid == 0 || fh.get(0, mid - 1) == fh.get(i, i + mid - 1)) {
                best = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        z[i] = best;
    }

    return z;
} // this returns a z vector where z[i] is the length of the longest substring starting from s[i] that is also a prefix of s.

vector<int> pi_array(const vector<int> &z) {
    int n = z.size();
    vector<int> pi(n, 0);
    // pi[i] is the length of the longest proper prefix of s[0..i] which is also a suffix of s[0..i].
    for (int i = 1; i < n; i++) {
        // for each i where z[i] > 0, it means that the substring starting at i matches the prefix of length z[i]. Therefore, for each j from 0 to z[i]-1, we can set pi[i + j] = j + 1, as the substring s[0..j] is a proper prefix that matches the suffix ending at position i + j.
        for (int len = z[i] - 1; len >= 0; --len) {
            if (pi[i + len] != 0) break;
            pi[i + len] = len + 1;
        } 
    }

    return pi;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    vector<int> z = z_array(s);
    vector<int> pi = pi_array(z);

    for (int x : z) cout << x << ' ';
    cout << '\n';

    for (int x : pi) cout << x << ' ';
    cout << '\n';

    return 0;
}