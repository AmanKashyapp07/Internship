class Solution {
public:
    using ll = long long;

    static constexpr ll MOD = 1e9 + 7;
    static constexpr ll P = 31;

    class FastHash {
    public:
        vector<ll> power, pref;

        FastHash() {}

        void init(const string &s) {
            int n = s.size();

            power.assign(n + 1, 1);
            pref.assign(n + 1, 0);

            for (int i = 0; i < n; i++) {
                power[i + 1] = power[i] * P % MOD;
                pref[i + 1] = (pref[i] * P + (s[i] - 'a' + 1)) % MOD;
            }
        }

        ll getHash(int l, int r) const {
            return (pref[r + 1]
                  - pref[l] * power[r - l + 1] % MOD
                  + MOD) % MOD;
        }

        static ll hashWord(const string &s) {
            ll h = 0;

            for (char c : s)
                h = (h * P + (c - 'a' + 1)) % MOD;

            return h;
        }
    };

    string s;
    FastHash H;

    unordered_set<ll> dict;
    unordered_set<int> lengths;

    vector<int> memo;

    bool dfs(int idx) {

        // Successfully formed the entire word.
        if (idx == s.size())
            return true;

        if (memo[idx] != -1)
            return memo[idx];

        for (int len : lengths) {

            if (idx + len > s.size())
                continue;

            // Current substring is not a dictionary word.
            if (!dict.count(H.getHash(idx, idx + len - 1)))
                continue;

            // Check whether the remaining suffix can also be formed.
            if (dfs(idx + len))
                return memo[idx] = true;
        }

        return memo[idx] = false;
    }

    vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {

        // Store hashes of all words.
        for (string &word : words) {
            dict.insert(FastHash::hashWord(word));
            lengths.insert(word.size());
        }

        vector<string> ans;

        for (string &word : words) {

            s = word;
            H.init(s);

            // Remove current word so it cannot match itself.
            ll h = FastHash::hashWord(word);
            dict.erase(h);

            memo.assign(s.size() + 1, -1);

            // Check whether current word can be formed using other words.
            if (dfs(0))
                ans.push_back(word);

            // Restore for future iterations.
            dict.insert(h);
        }

        return ans;
    }
};