/**
 * Problem: String Hashing
 * Link: N/A
 * Category: String
 * 
 * Description:
 * Compute prefix hashes for O(1) substring hash queries.
 * 
 * Logic/Approach:
 * FastHash polynomial hash builder.
 */

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <string_view>

using namespace std;

// ------------------- FAST HASH TEMPLATE (CSES OPTIMIZED) -------------------
using ll = long long;

const ll MOD = 1e9 + 7;
const ll P   = 31;

class FastHash {
private:
    vector<ll> power, pref;

public:
    FastHash(const string& s) {

        int n = s.size();

        power.assign(n + 1, 1);
        pref.assign(n + 1, 0);

        for(int i = 0; i < n; i++) {

            power[i + 1] = power[i] * P % MOD;

            pref[i + 1] =
                (pref[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }

    ll get(int l, int r) const {

        return (pref[r + 1]
              - pref[l] * power[r - l + 1] % MOD
              + MOD) % MOD;
    }

    static ll hashWord(const string& s) {

        ll h = 0;

        for(char c : s)
            h = (h * P + (c - 'a' + 1)) % MOD;

        return h;
    }
};
// ---------------------------------------------------------------------------

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s, p;
    if (!(cin >> s >> p)) return 0;

    int n = s.length(), m = p.length();
    if (m > n) { cout << 0 << "\n"; return 0; }

    FastHash string_hash(s); // Hash object for the main string to compute substring hashes efficiently
    FastHash pattern_hash(p);

    // Get target signature cleanly
    ll target = pattern_hash.get(0, m - 1);
    int match_count = 0;

    for (int i = 0; i <= n - m; i++) {
        // Natural comparison operator usage (==)
        if (string_hash.get(i, i + m - 1) == target) {
            match_count++;
        }
    }

    cout << match_count << "\n";
    return 0;
}