#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

typedef long long ll;
const ll MOD = 1e9 + 7;
const ll P = 31;

class FastHash {
    vector<ll> power, pref;
public:
    FastHash(const string& s) {
        int n = s.size();
        power.assign(n + 1, 1);
        pref.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            power[i + 1] = (power[i] * P) % MOD;
            pref[i + 1] = (pref[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }
    ll get(int l, int r) const {
        return (pref[r + 1] - (pref[l] * power[r - l + 1] % MOD) + MOD) % MOD;
    }
};

class Solution {
public:
    string shortestPalindrome(string s) {
        if (s.empty()) return s;

        string rs = s;
        reverse(rs.begin(), rs.end());

        FastHash forward(s);
        FastHash backward(rs);

        int n = s.size();
        int ans = 0;

        // Find the longest prefix of s that is a palindrome
        // A prefix s[0...i] is a palindrome if it equals the suffix of rs 
        // starting at (n - 1 - i)
        for (int i = 0; i < n; i++) {
            // prefix of original string corresponds to suffix of reversed string
            int revL = n - 1 - i;
            int revR = n - 1;
            if(forward.get(0, i) == backward.get(revL, revR)) {
                ans = i + 1; // length of the longest palindromic prefix
            }
        }

        // The characters after the longest palindrome prefix need to be 
        // reversed and prepended to s
        string suffix = s.substr(ans); // getting all characters starting from index ans to the end of the string
        reverse(suffix.begin(), suffix.end()); // reversing the suffix to prepend it to the original string

        return suffix + s;
    }
};