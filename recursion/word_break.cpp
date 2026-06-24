#include <vector>
#include <string>
#include <unordered_set>
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

class Solution {
    vector<int> dp;
    unordered_set<ll> dictHash;
    unordered_set<int> lengths;

    bool solve(int i, const string& s, const FastHash& H) {

        int n = s.size();

        if(i == n) return true;

        if(dp[i] != -1) return dp[i];

        for(int len : lengths) {

            int j = i + len - 1;

            if(j >= n) continue;

            ll currHash = H.get(i, j);

            if(dictHash.count(currHash) &&
               solve(j + 1, s, H)) {

                return dp[i] = true;
            }
        }

        return dp[i] = false;
    }

public:
    bool wordBreak(string s, vector<string>& wordDict) {

        int n = s.size();

        FastHash H(s);

        for(auto& word : wordDict) {
            dictHash.insert(FastHash::hashWord(word));
            lengths.insert(word.size());
        }

        dp.assign(n, -1);

        return solve(0, s, H);
    }
};