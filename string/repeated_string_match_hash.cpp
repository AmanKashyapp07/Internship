/**
 * Problem: Repeated String Match (Rabin-Karp)
 * Link: https://leetcode.com/problems/repeated-string-match/
 * Category: String
 * 
 * Description:
 * Find min repeats of a to contain b.
 * 
 * Logic/Approach:
 * Rabin-Karp polynomial hashing with FastHash.
 */

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;

#define all(x)   (x).begin(), (x).end()
#define rall(x)  (x).rbegin(), (x).rend()
#define pb       push_back
#define ff       first
#define ss       second

const int INF  = INT_MAX;
const ll  LINF = LLONG_MAX;
const ll  MOD  = 1e9 + 7;
const ll  P    = 31;

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
public:
    int repeatedStringMatch(string a, string b) {

        string s;
        int cnt = 0;

        while(s.size() < b.size()) {
            s += a;
            cnt++;
        }

        ll target = FastHash::hashWord(b);

        for(int extra = 0; extra <= 1; extra++) {

            FastHash H(s);

            int n = s.size();
            int m = b.size();

            for(int i = 0; i + m - 1 < n; i++) {
                if(H.get(i, i + m - 1) == target)
                    return cnt + extra;
            }

            s += a;
        }

        return -1;
    }
};
