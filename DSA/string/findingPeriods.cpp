/**
 * Problem: Finding Periods
 * Link: https://cses.fi/problemset/task/1733
 * Category: String
 * 
 * Description:
 * Find all period lengths of a string.
 * 
 * Logic/Approach:
 * Polynomial hashing comparison of substring segments.
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

    // Check every possible period length 'k' from 1 to n
    for (int k = 1; k <= n; k++) {
        
        // The full string itself is always a valid period
        if (k == n) {
            cout << n << " ";
            continue;
        }

        // A period of length 'k' leaves an overlapping segment of length (n - k)
        int overlap_len = n - k;

        // Extract the hashes in O(1)
        ll prefix_hash = string_hash.get(0, overlap_len - 1); // Hash of the prefix of length (n - k)
        ll suffix_hash = string_hash.get(k, n - 1); // Hash of the suffix of length (n - k)
        // we are comparing both the prefix and suffix of length (n - k) to check if they are identical. If they are, it means that the string can be constructed by repeating a substring of length k, making k a valid period length.
        // If the overlapping prefix and suffix are identical, 'k' is a valid period
        if (prefix_hash == suffix_hash) {
            cout << k << " ";
        }
    }
    
    cout << "\n";

    return 0;
}