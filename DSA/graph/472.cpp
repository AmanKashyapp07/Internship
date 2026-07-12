#include <vector>
#include <string>
#include <unordered_set>
using namespace std;

class RollingHash {
public:
    using ll = long long;

    static constexpr ll MOD = 1000000007;
    static constexpr ll BASE = 31;

    vector<ll> pref, power;

    RollingHash(const string &s) {
        int n = s.size();

        pref.assign(n + 1, 0);
        power.assign(n + 1, 1);

        for (int i = 0; i < n; i++) {
            power[i + 1] = power[i] * BASE % MOD;
            pref[i + 1] = (pref[i] * BASE + (s[i] - 'a' + 1)) % MOD;
        }
    }

    ll get(int l, int r) const {
        return (pref[r + 1] -
                pref[l] * power[r - l + 1] % MOD +
                MOD) % MOD;
    }

    static ll hash(const string &s) {
        ll h = 0;
        for (char c : s)
            h = (h * BASE + (c - 'a' + 1)) % MOD;
        return h;
    }
};

class Solution {
public:
    using ll = long long;

    unordered_set<ll> dict;
    unordered_set<int> lengths;
    vector<int> dp;

    bool dfs(const string &word, int i, const RollingHash &H) {

        if (i == word.size())
            return true;

        if (dp[i] != -1)
            return dp[i];

        for (int len : lengths) {

            if (i + len > word.size())
                continue;

            if (!dict.count(H.get(i, i + len - 1)))
                continue;

            if (dfs(word, i + len, H))
                return dp[i] = true;
        }

        return dp[i] = false;
    }

    vector<string> findAllConcatenatedWordsInADict(vector<string> &words) {

        for (auto &word : words) {
            dict.insert(RollingHash::hash(word));
            lengths.insert(word.size());
        }

        vector<string> ans;

        for (auto &word : words) {

            dict.erase(RollingHash::hash(word));

            RollingHash H(word);
            dp.assign(word.size(), -1);

            if (dfs(word, 0, H))
                ans.push_back(word);

            dict.insert(RollingHash::hash(word));
        }

        return ans;
    }
};