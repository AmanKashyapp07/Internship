/**
 * Problem: Palindrome Queries
 * Link: https://cses.fi/problemset/task/2420
 * Category: String
 * 
 * Description:
 * Support dynamic point updates and range palindromic checks.
 * 
 * Logic/Approach:
 * Fenwick tree / Segment tree storing forward and backward polynomial hashes.
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

using ll = long long;

const ll MOD = 1e9 + 7;
const ll P = 31;

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
        return (pref[r + 1]
              - pref[l] * power[r - l + 1] % MOD
              + MOD) % MOD;
    }
};

bool isPalindrome(int l, int r, const FastHash& H1, const FastHash& H2, int n) {
    ll hash1 = H1.get(l, r);
    ll hash2 = H2.get(n - 1 - r, n - 1 - l);
    return hash1 == hash2;
}

int main() {

    string s = "abacaba";
    int n = s.size();

    string rev = s;
    reverse(rev.begin(), rev.end());

    FastHash H1(s);
    FastHash H2(rev);


    cout << isPalindrome(0, 2, H1, H2, n) << '\n'; // "aba" -> 1
    cout << isPalindrome(1, 3, H1, H2, n) << '\n'; // "bac" -> 0
    cout << isPalindrome(2, 4, H1, H2, n) << '\n'; // "aca" -> 1
    cout << isPalindrome(0, 6, H1, H2, n) << '\n'; // "abacaba" -> 1
}
