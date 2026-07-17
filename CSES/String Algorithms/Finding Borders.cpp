#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <string_view>

using namespace std;

struct HashPair {
    long long h1, h2;
    bool operator==(const HashPair& other) const {
        return h1 == other.h1 && h2 == other.h2;
    }
};

class StablePolyHash {
private:
    int n;
    static inline long long P1 = 0, P2 = 0;
    const long long M1 = 1e9 + 7;
    const long long M2 = 1e9 + 9;
    vector<long long> pow1, pow2, h1, h2;

    static long long generate_random_base(long long min_val, long long max_val) {
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_int_distribution<long long> dist(min_val, max_val);
        long long base = dist(rng);
        return base % 2 == 0 ? base + 1 : base;
    }

public:
    StablePolyHash(string_view s) {
        n = s.length();
        if (P1 == 0) P1 = generate_random_base(150, 300);
        if (P2 == 0) P2 = generate_random_base(350, 500);

        pow1.resize(n + 1, 1); pow2.resize(n + 1, 1);
        h1.resize(n + 1, 0);   h2.resize(n + 1, 0);
        
        for (int i = 0; i < n; i++) {
            pow1[i + 1] = (pow1[i] * P1) % M1;
            pow2[i + 1] = (pow2[i] * P2) % M2;
            long long val = s[i] - 'a' + 1;
            h1[i + 1] = (h1[i] * P1 + val) % M1;
            h2[i + 1] = (h2[i] * P2 + val) % M2;
        }
    }

    HashPair get_hash(int L, int R) const {
        if (L > R || L < 0 || R >= n) return {0, 0};
        long long res1 = (h1[R + 1] - (h1[L] * pow1[R - L + 1]) % M1 + M1) % M1;
        long long res2 = (h2[R + 1] - (h2[L] * pow2[R - L + 1]) % M2 + M2) % M2;
        return {res1, res2};
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;

    int n = s.length();
    StablePolyHash hasher(s);

    // Check every possible border length k from 1 to n-1
    for (int k = 1; k < n; k++) {
        HashPair prefix_hash = hasher.get_hash(0, k - 1);
        HashPair suffix_hash = hasher.get_hash(n - k, n - 1);

        if (prefix_hash == suffix_hash) {
            cout << k << " ";
        }
    }
    cout << "\n";

    return 0;
}