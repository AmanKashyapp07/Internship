// Link: https://cses.fi/problemset/task/2107

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

vector<int> hash_z_algo(const string &s) {
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
}

vector<int> z_to_pi(const vector<int> &z) {
    int n = z.size();
    vector<int> pi(n, 0);

    for (int i = 1; i < n; i++) {
        for (int j = z[i] - 1; j >= 0 && pi[i + j] == 0; j--) {
            pi[i + j] = j + 1;
        }
    }

    return pi;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    vector<int> z = hash_z_algo(s);
    vector<int> pi = z_to_pi(z);

    for (int x : z) cout << x << ' ';
    cout << '\n';

    for (int x : pi) cout << x << ' ';
    cout << '\n';

    return 0;
}