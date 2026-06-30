/**
 * CSES Problem Set
 *
 * Problem: CSES Solution Template
 * Link: https://cses.fi/
 * Category: Template
 *
 * Description:
 * A template file containing boilerplate code, fast I/O setup, and common macros/imports for solving CSES problems.
 *
 * Logic/Approach:
 * Provides basic imports, standard namespace, and empty main function with fast I/O configuration.
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
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

// this checks if s2 is a subsequence of s1. It returns true if s2 is a subsequence of s1, otherwise false.
// time complexity is O(n) where n is the length of s1. Space complexity is O(1) as we are using only a few variables.
class Solution
{
public:
    bool checkSubsequence(const string &s1, const string &s2)
    {
        int n = s1.size();
        int m = s2.size();
        int j = 0; // Pointer for s2
        int i = 0; // Pointer for s1
        while (i < n && j < m)
        {
            if (s1[i] == s2[j])
            {
                j++; // Move pointer in s2 if characters match
                i++; // Move pointer in s1
            }
            else
            {
                i++; // Move pointer in s1 if characters don't match
            }
        }

        return j == m; // If we have traversed all of s2, it's a subsequence
    }
    string findLongestWord(string s, vector<string> &dictionary)
    {
        int maxLength = 0;
        string longestWord = "";
        for (const string &word : dictionary)
        {
            if (checkSubsequence(s, word))
            {
                if (word.length() > maxLength || (word.length() == maxLength && word < longestWord))
                {
                    maxLength = word.length();
                    longestWord = word;
                }
            }
        }
        return longestWord;
    }
};
