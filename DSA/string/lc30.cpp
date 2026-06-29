#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

typedef long long ll;

// Define modulus and base for polynomial rolling hash
const ll MOD = 1e9 + 7;
const ll P = 31;

/**
 * FastHash class computes prefix hashes for a string to allow O(1) 
 * retrieval of the hash value of any substring [l, r].
 */
class FastHash {
private:
    vector<ll> power, pref;

public:
    FastHash(const string& s) {
        int n = s.size();
        power.assign(n + 1, 1);
        pref.assign(n + 1, 0);

        // Precompute powers of P and prefix hashes
        for (int i = 0; i < n; i++) {
            power[i + 1] = (power[i] * P) % MOD;
            pref[i + 1] = (pref[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }

    // Returns hash of substring s[l...r] in O(1)
    ll get(int l, int r) const {
        return (pref[r + 1] - (pref[l] * power[r - l + 1] % MOD) + MOD) % MOD;
    }

    // Utility to compute hash of a single word
    static ll hashWord(const string& s) {
        ll h = 0;
        for (char c : s) {
            h = (h * P + (c - 'a' + 1)) % MOD;
        }
        return h;
    }
};

class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        if (s.empty() || words.empty()) return {};

        int n = s.size(), m = words.size(), len = words[0].size();
        int total = m * len; // Total length of the concatenated string
        if (n < total) return {};

        FastHash H(s);
        unordered_map<ll, int> freq;
        // Pre-hash all words in the input array for quick comparison
        for (const string& w : words) freq[FastHash::hashWord(w)]++;

        vector<int> ans;

        // Iterate through each possible offset to handle all starting positions
        // that are not necessarily multiples of 'len'
        for (int offset = 0; offset < len; offset++) {
            unordered_map<ll, int> window;
            int left = offset;
            
            // Move right pointer by 'len' to evaluate blocks of words
            for (int right = offset; right + len <= n; right += len) {
                ll curHash = H.get(right, right + len - 1);
                window[curHash]++;

                // If current word frequency exceeds the allowed frequency, 
                // shrink the window from the left
                while (window[curHash] > freq[curHash]) {
                    ll leftHash = H.get(left, left + len - 1);
                    window[leftHash]--;
                    left += len;
                }

                // If window size equals the total length of all words combined
                if (right - left + len == total) {
                    ans.push_back(left);
                }
            }
        }
        return ans;
    }
};