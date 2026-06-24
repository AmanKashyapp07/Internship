/**
 * Problem: Implement strStr()
 * Link: https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string/
 * Category: String
 * 
 * Description:
 * Find the index of the first occurrence of needle in haystack.
 * 
 * Logic/Approach:
 * Rabin-Karp single-hash polynomial comparison.
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
    int strStr(string haystack, string needle) {

        int n = haystack.size();
        int m = needle.size();

        if(m > n) return -1;

        FastHash H(haystack);
        ll target = FastHash::hashWord(needle);

        for(int i = 0; i + m - 1 < n; i++) {
            if(H.get(i, i + m - 1) == target)
                return i;
        }

        return -1;
    }
};
