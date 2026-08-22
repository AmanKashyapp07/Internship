#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MOD = 1000000007;
const ll P = 31;

struct FastHash {
    vector<ll> power, pref;
    
    FastHash(const string& s) {
        int n = s.size();
        power.resize(n + 1, 1);
        pref.resize(n + 1, 0);
        
        for (int i = 0; i < n; ++i) {
            power[i + 1] = power[i] * P % MOD;
            pref[i + 1] = (pref[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }
    
    ll get(int l, int r) const {
        return (pref[r + 1] - pref[l] * power[r - l + 1] % MOD + MOD) % MOD;
    }
    
};

ll hashWord(const string &s) {
    ll h = 0;
    for (char c : s) {
        h = (h * P + c - 'a' + 1) % MOD;
    }
    return h;
}

ll solve(int i, const string& s, const vector<int>& lengths,
         const unordered_set<ll>& dict, // <-- Clean flat set
         const FastHash& hasher, vector<ll>& dp) {
    
    if (i == s.size()) return 1;
    if (dp[i] != -1) return dp[i];
    
    ll ways = 0;
    for (int len : lengths) {
        if (i + len <= s.size()) {
            ll h = hasher.get(i, i + len - 1);
            // Simpler check: Does this hash exist in our dictionary?
            if (dict.count(h)) {
                ways = (ways + solve(i + len, s, lengths, dict, hasher, dp)) % MOD;
            }
        }
    }
    return dp[i] = ways;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s;
    cin >> s;
    int n = s.size();
    
    FastHash hasher(s);
    
    int k;
    cin >> k;
    
    unordered_set<ll> dict; // <-- Simplified container
    unordered_set<int> unique_lengths; // Temporary set to avoid duplicates in lengths vector
    vector<int> lengths;
    
    while (k--) {
        string word;
        cin >> word;
        
        // Track unique lengths so we don't iterate over the same length twice in DP
        if (unique_lengths.insert(word.size()).second) {
            lengths.push_back(word.size());
        }
        
        dict.insert(hashWord(word));
    }
    
    vector<ll> dp(n + 1, -1);
    
    cout << solve(0, s, lengths, dict, hasher, dp) << '\n';
    return 0;
}