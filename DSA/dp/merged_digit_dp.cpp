#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ================================================
// 1. Number of Digit One
// ================================================
// Problem: Given an integer n, count the total number of digit '1' 
// appearing in all non-negative integers less than or equal to n.
// LeetCode 233
class NumberOfDigitOne {
public:
    string s;
    int memo[20][20][2][2];

    long long dp(int pos, int cnt, bool started, bool tight) {
        if (pos == s.size()) return cnt;
        if (memo[pos][cnt][started][tight] != -1) 
            return memo[pos][cnt][started][tight];

        int up = tight ? s[pos] - '0' : 9;
        long long ans = 0;

        if (!started) {
            ans += dp(pos + 1, cnt, false, tight && (0 == up));
        }

        for (int d = (started ? 0 : 1); d <= up; ++d) {
            ans += dp(pos + 1, cnt + (d == 1), true, tight && (d == up));
        }

        return memo[pos][cnt][started][tight] = ans;
    }

    long long countDigitOne(long long n) {
        if (n < 0) return 0;
        s = to_string(n);
        memset(memo, -1, sizeof(memo));
        return dp(0, 0, false, true);
    }
};

// ================================================
// 2. Numbers At Most N Given Digit Set
// ================================================
// Problem: Given a set of digits, return how many positive integers 
// can be formed that are less than or equal to a given integer n.
// LeetCode 902
class NumbersAtMostNGivenDigitSet {
public:
    string s;
    vector<bool> allowed;
    int memo[20][2][2];

    long long dp(int pos, bool started, bool tight) {
        if (pos == s.size()) return started; // If we have started forming a number, count it as 1 valid number

        if (memo[pos][started][tight] != -1) return memo[pos][started][tight];

        int up = tight ? s[pos] - '0' : 9;
        long long ans = 0;

        if (!started) {
            ans += dp(pos + 1, false, tight && (0 == up));
        }

        for (int d = (started ? 0 : 1); d <= up; ++d) {
            if (allowed[d])
                ans += dp(pos + 1, true, tight && (d == up));
        }

        return memo[pos][started][tight] = ans;
    }

    int atMostNGivenDigitSet(vector<string>& digits, int n) {
        allowed.assign(10, false);
        for (auto& d : digits) allowed[d[0] - '0'] = true;

        s = to_string(n);
        memset(memo, -1, sizeof(memo));
        return dp(0, false, true);
    }
};

// ================================================
// 3. Count of Integers
// ================================================
// Problem: Given two integers num1 and num2, and two integers min_sum and max_sum,
// return the count of integers in the range [num1, num2] whose digit sum 
// lies between min_sum and max_sum (inclusive).
// LeetCode 2719
class CountOfIntegers {
public:
    string s;
    int minS, maxS;
    const int MOD = 1000000007;
    int memo[25][210][2][2];

    long long dp(int pos, int sum, bool started, bool tight) {
        if (sum > maxS) return 0;
        if (pos == s.size()) return (sum >= minS) ? 1 : 0;

        if (memo[pos][sum][started][tight] != -1) return memo[pos][sum][started][tight];

        int up = tight ? s[pos] - '0' : 9;
        long long ans = 0;

        if (!started) {
            ans = (ans + dp(pos + 1, sum, false, tight && (0 == up))) % MOD;
        }

        for (int d = (started ? 0 : 1); d <= up; ++d) {
            ans = (ans + dp(pos + 1, sum + d, true, tight && (d == up))) % MOD;
        }

        return memo[pos][sum][started][tight] = ans;
    }

    long long countUpTo(string num) {
        s = num;
        memset(memo, -1, sizeof(memo));
        return dp(0, 0, false, true);
    }

    int countStrings(string num1, string num2, int min_sum, int max_sum) {
        minS = min_sum; maxS = max_sum;
        long long res = countUpTo(num2) - countUpTo(num1);
        if (res < 0) res += MOD;

        int sum1 = 0;
        for (char c : num1) sum1 += c - '0';
        if (sum1 >= minS && sum1 <= maxS) res = (res + 1) % MOD;

        return res;
    }
};

// ================================================
// 4. Counting Numbers (No two adjacent digits same)
// ================================================
// Problem: Count the number of integers from 0 to x such that no two adjacent 
// digits are the same. ( we havt to count 0 also)
class CountingNumbers {
public:
    string s;
    int memo[20][11][2][2];

    long long dp(int pos, int prev, bool started, bool tight) {
        if (pos == s.size()) return 1;
        if (memo[pos][prev][started][tight] != -1) return memo[pos][prev][started][tight];
        int up = tight ? s[pos] - '0' : 9;
        long long ans = 0;
        if (!started) ans += dp(pos + 1, 10, false, tight && (0 == up));

        for (int d = (started ? 0 : 1); d <= up; ++d) {
            if (started && d == prev) continue; // if we have started and the current digit is same as previous, skip
            ans += dp(pos + 1, d, true, tight && (d == up));
        }

        return memo[pos][prev][started][tight] = ans;
    }

    long long countUpTo(long long x) {
        if (x < 0) return 0;
        s = to_string(x);
        memset(memo, -1, sizeof(memo));
        return dp(0, 10, false, true);
    }

    long long countValidNumbers(int a, int b) {
        return countUpTo(b) - countUpTo(a - 1);
    }
};

// ================================================
// 5. Numbers With Repeated Digits
// ================================================
// Problem: Given an integer n, return the count of numbers from 0 to n 
// that do NOT have any repeated digits.
// LeetCode 1012
class NumbersWithRepeatedDigits {
public:
    string s;
    int memo[20][2][2][1024];

    long long dp(int pos, bool started, bool tight, int mask) {
        if (pos == s.size()) return started;

        if (memo[pos][started][tight][mask] != -1) 
            return memo[pos][started][tight][mask];

        int up = tight ? s[pos] - '0' : 9;
        long long ans = 0;

        if (!started) {
            ans += dp(pos + 1, false, tight && (0 == up), mask);
        }

        for (int d = (started ? 0 : 1); d <= up; ++d) {
            if ((mask & (1 << d))) continue;
            ans += dp(pos + 1, true, tight && (d == up), mask | (1 << d));
        }

        return memo[pos][started][tight][mask] = ans;
    }

    int numDupDigitsAtMostN(int n) {
        s = to_string(n);
        memset(memo, -1, sizeof(memo));
        long long unique = dp(0, false, true, 0);
        return n + 1 - unique;
    }
};
