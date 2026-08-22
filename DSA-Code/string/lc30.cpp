#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

using ll = long long;

const ll MOD = 1e9 + 7;
const ll P = 31;

class Hash {
    vector<ll> p, h;

public:
    Hash(const string &s) {
        int n = s.size();
        p.assign(n + 1, 1);
        h.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            p[i + 1] = p[i] * P % MOD;
            h[i + 1] = (h[i] * P + s[i] - 'a' + 1) % MOD;
        }
    }

    ll get(int l, int r) const {
        return (h[r + 1] - h[l] * p[r - l + 1] % MOD + MOD) % MOD;
    }

};

ll hash_val(const string &s) {
    ll h = 0;
    for (char c : s) {
        h = (h * P + c - 'a' + 1) % MOD;
    }
    return h;
}

class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        if (s.empty() || words.empty()) return {};

        int n = s.size(), k = words[0].size(), m = words.size(), need = k * m;
        if (n < need) return {};

        Hash H(s);
        unordered_map<ll, int> cnt;

        for (auto &w : words) cnt[hash_val(w)]++;

        vector<int> ans;

        for (int off = 0; off < k; off++) { 
            unordered_map<ll, int> cur;
            int l = off; // left boundary of the current window

            for (int r = off; r + k <= n; r += k) { // all substrings of length k starting from off
                ll x = H.get(r, r + k - 1);
                cur[x]++;

                while (cur[x] > cnt[x]) {
                    cur[H.get(l, l + k - 1)]--;
                    l += k;
                }

                if (r - l + k == need) ans.push_back(l);
            }
        }

        return ans;
    }
};