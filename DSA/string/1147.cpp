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
            pref[i + 1] = (pref[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }

    ll get(int l, int r) const {
        if (l > r) return 0;
        return (pref[r + 1]
              - pref[l] * power[r - l + 1] % MOD
              + MOD) % MOD;
    }
};

class Solution {
public:
    int longestDecomposition(string text) {
        int n = text.size();
        FastHash H(text);
        
        int k = 0;
        int i = 0;       // Left boundary of the remaining string
        int j = n - 1;   // Right boundary of the remaining string
        
        // Track the starting points of the current tentative chunks
        int left_start = i;
        int right_end = j;
        
        while (i < j) {
            // Get hashes of the current left and right tentative chunks
            ll left_hash = H.get(left_start, i);
            ll right_hash = H.get(j, right_end);
            
            if (left_hash == right_hash) {
                // Found a matching pair of chunks
                k += 2;
                left_start = i + 1;
                right_end = j - 1;
            }
            
            i++;
            j--;
        }
        
        // If there is an unmatched middle part left over
        if (left_start <= right_end) {
            k += 1;
        }
        
        return k;
    }
};