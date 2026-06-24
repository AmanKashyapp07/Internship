/**
 * Problem: String Template
 * Link: N/A
 * Category: String
 * 
 * Description:
 * Polynomial string hashing template (FastHash) for O(1) substring hash queries.
 * 
 * Logic/Approach:
 * Precomputes prime powers for prefix hash evaluations.
 */

#include <vector>
#include <string>
using namespace std;

#define ll long long

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

            pref[i + 1] = (pref[i] * P + (s[i] - 'a' + 1)) % MOD; // calculating the prefix hash for the substring s[0..i] using the formula: pref[i + 1] = (pref[i] * P + (s[i] - 'a' + 1)) % MOD. This allows us to compute the hash of any substring in O(1) time later on.
        }
    }

    ll get(int l, int r) const {

        return (pref[r + 1]
              - pref[l] * power[r - l + 1] % MOD
              + MOD) % MOD;
    } // maths : (pref[r + 1] - pref[l] * power[r - l + 1]) % MOD because pref[r + 1] = (s[0]*P^r + s[1]*P^(r-1) + ... + s[r]) % MOD and pref[l] = (s[0]*P^(l-1) + s[1]*P^(l-2) + ... + s[l-1]) % MOD. To get the hash of substring s[l..r], we subtract the contribution of the prefix s[0..l-1] from pref[r + 1] and adjust for the powers of P accordingly.

    static ll hashWord(const string& s) {

        ll h = 0;

        for(char c : s) h = (h * P + (c - 'a' + 1)) % MOD;

        return h;
    }
};

// ============================================================================