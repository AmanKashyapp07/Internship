/**
 * LeetCode 140 - Word Break II
 *
 * Description:
 * Given a string s and a dictionary of strings wordDict, add spaces in s to construct a sentence
 * where each word is a valid dictionary word. Return all such possible sentences.
 *
 * Approach:
 * - Use Polynomial Rolling Hash (FastHash) to represent words.
 * - Perform Depth-First Search (DFS) with memoization.
 * - For each index, check all valid word lengths. If the prefix matches a dictionary word, recursively find all matching sentences for the suffix.
 *
 * Time Complexity: O(2^N) in the worst case (e.g. s = "aaaa", dict = {"a", "aa", "aaa", "aaaa"}).
 * Space Complexity: O(2^N)
 */

class Solution {
    using ll = long long;

    static constexpr ll MOD = 1e9 + 7;
    static constexpr ll P = 31;

    class FastHash {
    private:
        vector<ll> power, pref;

    public:
        FastHash(const string &s) {
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
    FastHash *H;

    unordered_set<ll> dict;
    unordered_set<int> lengths;

    unordered_map<int, vector<string>> memo;

    vector<string> dfs(int idx) {

        // Already solved for this index.
        if (memo.count(idx))
            return memo[idx];

        vector<string> ans;

        // Try every possible word length.
        for (int len : lengths) {

            if (idx + len > s.size())
                continue;

            // Current substring is not in dictionary.
            if (!dict.count(H->getHash(idx, idx + len - 1)))
                continue;

            string word = s.substr(idx, len);

            // Current word reaches the end.
            // So this itself forms a complete sentence.
            if (idx + len == s.size()) {
                ans.push_back(word);
                continue;
            }

            // Get all sentences from the remaining suffix.
            vector<string> suffixes = dfs(idx + len);

            // Attach current word in front of every suffix.
            for (string &suffix : suffixes)
                ans.push_back(word + " " + suffix);
        }

        return memo[idx] = ans;
    }

public:
    vector<string> wordBreak(string str, vector<string> &wordDict) {

        s = str;

        for (string &word : wordDict) {
            dict.insert(FastHash::hashWord(word));
            lengths.insert(word.size());
        }

        FastHash hashObj(s);
        H = &hashObj;

        return dfs(0);
    }
};