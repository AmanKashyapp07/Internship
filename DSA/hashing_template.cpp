class FastHash {
    using ll = long long;

    static constexpr ll MOD = 1000000007;
    static constexpr ll P = 31;

    vector<ll> power, pref;

public:
    FastHash() {}

    FastHash(const string &s) {
        init(s);
    }

    void init(const string &s) {
        int n = s.size();

        power.assign(n + 1, 1);
        pref.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            power[i + 1] = power[i] * P % MOD;
            pref[i + 1] = (pref[i] * P + (s[i] + 1)) % MOD;
        }
    }

    // Returns hash of substring s[l...r] in O(1)
    ll getHash(int l, int r) const {
        return (pref[r + 1]
              - pref[l] * power[r - l + 1] % MOD
              + MOD) % MOD;
    }

    // Returns hash of an entire string
    static ll hashWord(const string &s) {
        ll h = 0;

        for (char c : s)
            h = (h * P + (c + 1)) % MOD;

        return h;
    }
};

/*
====================== HOW TO USE ======================

1. Build prefix hash

    FastHash H(s);

or

    FastHash H;
    H.init(s);

--------------------------------------------------------

2. Hash of any substring s[l...r]

    H.getHash(l, r);

Time : O(1)

--------------------------------------------------------

3. Hash of an entire word

    FastHash::hashWord(word);

Useful for dictionary hashing.

--------------------------------------------------------

4. Store dictionary words

    unordered_set<long long> dict;

    for (string &word : words)
        dict.insert(FastHash::hashWord(word));

--------------------------------------------------------

5. Check whether substring exists in dictionary

    if (dict.count(H.getHash(l, r))) {
        ...
    }

No need to create substr().

--------------------------------------------------------

6. Compare two substrings

    H.getHash(l1, r1) == H.getHash(l2, r2)

O(1)

--------------------------------------------------------

7. Compare substring with a word

    H.getHash(l, r) == FastHash::hashWord(word)

--------------------------------------------------------

Common OA Uses

✔ Word Break
✔ Concatenated Words
✔ Rabin-Karp
✔ Longest Duplicate Substring
✔ Distinct Substrings
✔ Repeated Substrings
✔ Pattern Matching

Complexities

Build Prefix Hash      : O(n)
Substring Hash Query   : O(1)
Whole Word Hash        : O(length)

========================================================
*/