// Link: https://cses.fi/problemset/task/1731

#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

struct HashPair {
    long long h1, h2;

    bool operator==(const HashPair& other) const {
        return h1 == other.h1 && h2 == other.h2;
    }
};

struct HashFunction {
    size_t operator()(const HashPair& h) const {
        return h.h1 * 1000000007LL + h.h2;
    }
};

class StablePolyHash {
private:
    int n;

    static inline long long P1 = 0;
    static inline long long P2 = 0;

    const long long M1 = 1e9 + 7;
    const long long M2 = 1e9 + 9;

    vector<long long> pow1, pow2;
    vector<long long> h1, h2;

    static long long generate_random_base(long long l, long long r) {
        mt19937_64 rng(
            chrono::steady_clock::now().time_since_epoch().count()
        );

        uniform_int_distribution<long long> dist(l, r);

        long long base = dist(rng);

        return (base & 1) ? base : base + 1;
    }

public:
    StablePolyHash(string_view s) {
        n = s.size();

        if (P1 == 0) P1 = generate_random_base(150, 300);
        if (P2 == 0) P2 = generate_random_base(350, 500);

        pow1.assign(n + 1, 1);
        pow2.assign(n + 1, 1);

        h1.assign(n + 1, 0);
        h2.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            pow1[i + 1] = (pow1[i] * P1) % M1;
            pow2[i + 1] = (pow2[i] * P2) % M2;

            long long val = s[i] - 'a' + 1;

            h1[i + 1] = (h1[i] * P1 + val) % M1;
            h2[i + 1] = (h2[i] * P2 + val) % M2;
        }
    }

    HashPair get_hash(int L, int R) const {
        long long x1 =
            (h1[R + 1] - h1[L] * pow1[R - L + 1] % M1 + M1) % M1;

        long long x2 =
            (h2[R + 1] - h2[L] * pow2[R - L + 1] % M2 + M2) % M2;

        return {x1, x2};
    }
};

int n;

vector<long long> dp;
vector<int> lengths;

unordered_map<int, unordered_set<HashPair, HashFunction>> words;

StablePolyHash* textHash;

long long solve(int pos) {

    if (pos == n)
        return 1;

    if (dp[pos] != -1)
        return dp[pos];

    long long ans = 0;

    for (int len : lengths) {

        if (pos + len > n)
            continue;

        HashPair cur =
            textHash->get_hash(pos, pos + len - 1);

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

        StablePolyHash h(word);

        words[len].insert(
            h.get_hash(0, len - 1)
        );
    }

    StablePolyHash hashObject(s);
    textHash = &hashObject;

    dp.assign(n + 1, -1);

    cout << solve(0) << '\n';

    return 0;
}