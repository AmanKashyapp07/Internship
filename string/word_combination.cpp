/**
 * Problem: Word Combinations
 * Link: https://cses.fi/problemset/task/1731
 * Category: String
 * 
 * Description:
 * Count ways to build a text using dictionary words.
 * 
 * Logic/Approach:
 * Dynamic Programming with FastHash lookup.
 */

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <string_view>
#include <unordered_set>
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
 
int n;
 
vector<long long> dp;
vector<int> lengths;
 
unordered_map<int, unordered_set<ll>> words;
 
FastHash* textHash;
 
long long solve(int pos) {
 
    if (pos == n)
        return 1;
 
    if (dp[pos] != -1)
        return dp[pos];
 
    long long ans = 0;
 
    for (int len : lengths) {
 
        if (pos + len > n)
            continue;
 
        ll cur =
            textHash->get(pos, pos + len - 1);
 
        if (words[len].count(cur)) {
            ans += solve(pos + len);
            ans %= MOD;
        }
    }
 
    return dp[pos] = ans;
}
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    string s;
    cin >> s;
 
    n = s.size();
 
    int k;
    cin >> k;
 
    for (int i = 0; i < k; i++) {
 
        string word;
        cin >> word;
 
        int len = word.size();
 
        if (!words.count(len))
            lengths.push_back(len);
 
        FastHash h(word);
 
        words[len].insert(
            h.get(0, len - 1)
        );
    }
 
    FastHash hashObject(s);
    textHash = &hashObject;
 
    dp.assign(n + 1, -1);
 
    cout << solve(0) << '\n';
 
    return 0;
}