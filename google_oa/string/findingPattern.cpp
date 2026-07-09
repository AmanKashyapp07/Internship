/**
 * Problem: Finding Pattern
 * Link: N/A
 * Category: String
 * 
 * Description:
 * Check if string patterns exist in a text.
 * 
 * Logic/Approach:
 * Rabin-Karp polynomial hashing matching pattern lengths.
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
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
    } // 

    static ll hashWord(const string& s) {

        ll h = 0;

        for(char c : s)
            h = (h * P + (c - 'a' + 1)) % MOD;

        return h;
    } // returns the hash of the substring s[l..r] in O(1) time using precomputed prefix hashes and powers of P. The hash is computed modulo MOD to avoid overflow and ensure a manageable size. The static function hashWord computes the hash of an entire string in O(m) time, where m is the length of the string, which is useful for comparing patterns with substrings of the main text.
};
// ---------------------------------------------------------------------------

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s; int k;
    if (!(cin >> s >> k)) return 0;
    
    int n = s.length();
    
    // Group patterns by length: groups[length] = list of {hash, original_index}
    vector<vector<pair<ll, int>>> groups(n + 1);
    for (int i = 0; i < k; i++) {
        string p; cin >> p;
        if (p.length() <= n) {
            groups[p.length()].push_back({FastHash::hashWord(p), i});
        }
    }

    FastHash text_hash(s);
    vector<bool> ans(k, false);
    vector<ll> text_subs;
    
    for (int len = 1; len <= n; len++) {
        if (groups[len].empty()) continue;
        
        // 1. Grab all hashes of this length from the main text
        text_subs.clear();
        for (int i = 0; i <= n - len; i++) {
            text_subs.push_back(text_hash.get(i, i + len - 1));
        }
        sort(text_subs.begin(), text_subs.end());
        
        // 2. Binary search our patterns against the text hashes
        for (const auto& pat : groups[len]) {
            if (binary_search(text_subs.begin(), text_subs.end(), pat.first)) {
                ans[pat.second] = true;
            }
        }
    }

    for (int i = 0; i < k; i++) {
        cout << (ans[i] ? "YES\n" : "NO\n");
    }

    return 0;
}

// binary_search(i,j,target) returns true if target is found in the sorted range [i,j). In this code, we use binary_search to check if the hash of a pattern exists in the list of hashes of substrings of the main text. If it does, we mark that pattern as found (YES), otherwise it remains not found (NO).