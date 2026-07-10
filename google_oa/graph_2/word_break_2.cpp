/**
 * LeetCode 140 - Word Break II
 *
 * Approach:
 * - Store every dictionary word as a polynomial rolling hash.
 * - DFS + memoization.
 * - At every index, try all possible dictionary word lengths.
 * - If the substring hash exists in the dictionary, recursively solve
 *   the remaining suffix.
 *
 * Time Complexity: O(number of generated sentences)
 * Worst case: O(2^N)
 *
 * Space Complexity: O(2^N)
 */

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
const long long P = 31;
const long long MOD = 1e9 + 7;
using namespace std;

class FastHash {
    vector<long long> power;
    vector<long long> pref;

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

    long long getHash(int l, int r) const {
        return (pref[r + 1]
                - pref[l] * power[r - l + 1] % MOD
                + MOD) % MOD;
    }

    static long long hashWord(const string &s) {
        long long h = 0;

        for (char c : s)
            h = (h * P + (c - 'a' + 1)) % MOD;

        return h;
    }
};

class Solution {
    string s;

    unordered_set<long long> dict;
    unordered_set<int> lengths;
    unordered_map<int, vector<string>> memo;

    vector<string> dfs(int idx, const FastHash &hash) {
        if (memo.count(idx))
            return memo[idx];

        vector<string> ans;

        for (int len : lengths) {
            if (idx + len > s.size())
                continue;

            long long hashValue = hash.getHash(idx, idx + len - 1);

            if (!dict.count(hashValue))
                continue;

            string word = s.substr(idx, len);

            if (idx + len == s.size()) {
                ans.push_back(word);
                continue;
            }

            vector<string> suffixes = dfs(idx + len, hash);

            for (const string &suffix : suffixes)
                ans.push_back(word + " " + suffix);
        }

        return memo[idx] = ans;
    }

public:
    vector<string> wordBreak(string str, vector<string> &wordDict) {
        s = move(str);

        dict.clear();
        lengths.clear();
        memo.clear();

        for (const string &word : wordDict) {
            dict.insert(FastHash::hashWord(word));
            lengths.insert(word.size());
        }

        FastHash hash(s);

        return dfs(0, hash);
    }
};