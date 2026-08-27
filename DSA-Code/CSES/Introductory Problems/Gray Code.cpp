// Link: https://cses.fi/problemset/task/2205
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    for (int i = 0; i < (1 << n); i++) {
        int g = i ^ (i >> 1);
        string s = bitset<16>(g).to_string();
        cout << s.substr(16 - n) << '\n';
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Generate Gray code sequence of n bits where consecutive codes differ in exactly 1 bit (CSES 2205).
// - Approach: Binary-to-Gray Code Formula: G(i) = i ^ (i >> 1).
// - Intuition: Bitwise formula `i ^ (i >> 1)` guarantees adjacent numbers differ by exactly 1 bit in binary representation.
// - Complexity: Time: O(2^N \cdot N), Space: O(1).