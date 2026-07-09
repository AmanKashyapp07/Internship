/**
 * Problem: Finding Borders
 * Link: https://cses.fi/problemset/task/1732
 * Category: String
 * 
 * Description:
 * Find all border lengths of a string.
 * 
 * Logic/Approach:
 * Rabin-Karp polynomial hashing with FastHash.
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
    // Optimize Standard I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;
    int n = s.length();

    FastHash string_hash(s);

    // A border must be shorter than the whole string, so k goes from 1 to n - 1
    for (int k = 1; k < n; k++) {
        
        // Extract hash of the prefix of length 'k'
        ll prefix_hash = string_hash.get(0, k - 1);
        
        // Extract hash of the suffix of length 'k'
        ll suffix_hash = string_hash.get(n - k, n - 1);

        // If they match, 'k' is a valid border length
        if (prefix_hash == suffix_hash) {
            cout << k << " ";
        }
    }
    
    cout << "\n";

    return 0;
}