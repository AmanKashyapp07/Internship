#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

using ll = long long;

const ll MOD1 = 1000000007;
const ll MOD2 = 1000000009;
const ll P1 = 31;
const ll P2 = 37;

struct Hash {
    ll h1, h2;

    bool operator==(const Hash &other) const {
        return h1 == other.h1 && h2 == other.h2;
    }
};

struct HashHasher {
    size_t operator()(const Hash &x) const {
        return (size_t)x.h1 ^ ((size_t)x.h2 << 1);
    }
};

struct FastHash {
    vector<ll> power1, power2;
    vector<ll> pref1, pref2;

    FastHash(const string &s) {
        int n = s.size();

        power1.assign(n + 1, 1);
        power2.assign(n + 1, 1);

        pref1.assign(n + 1, 0);
        pref2.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            power1[i + 1] = power1[i] * P1 % MOD1;
            power2[i + 1] = power2[i] * P2 % MOD2;

            pref1[i + 1] = (pref1[i] * P1 + (s[i] - 'a' + 1)) % MOD1;
            pref2[i + 1] = (pref2[i] * P2 + (s[i] - 'a' + 1)) % MOD2;
        }
    }

    Hash getHash(int l, int r) const {
        ll x1 = (pref1[r + 1] - pref1[l] * power1[r - l + 1] % MOD1 + MOD1) % MOD1;
        ll x2 = (pref2[r + 1] - pref2[l] * power2[r - l + 1] % MOD2 + MOD2) % MOD2;
        return {x1, x2};
    }
};

Hash hashWord(const string &s) {
    ll h1 = 0;
    ll h2 = 0;

    for (char c : s) {
        h1 = (h1 * P1 + (c - 'a' + 1)) % MOD1;
        h2 = (h2 * P2 + (c - 'a' + 1)) % MOD2;
    }

    return {h1, h2};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    FastHash fh(s);

    int q;
    cin >> q;

    // length -> set of double hashes
    unordered_map<int, unordered_set<Hash, HashHasher>> cache;

    while (q--) {
        string pat;
        cin >> pat;

        int m = pat.size();

        if (m > (int)s.size()) {
            cout << "NO\n";
            continue;
        }

        // Build cache for this length only once
        if (!cache.count(m)) {
            unordered_set<Hash, HashHasher> hashes;

            for (int i = 0; i + m <= (int)s.size(); i++) {
                hashes.insert(fh.getHash(i, i + m - 1));
            }

            cache[m] = move(hashes);
        }

        Hash h = hashWord(pat);

        cout << (cache[m].count(h) ? "YES\n" : "NO\n");
    }

    return 0;
}