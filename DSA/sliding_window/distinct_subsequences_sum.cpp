/**
 * ========================================================
 *          Sum of Distinct Values in All Subsequences
 * ========================================================
 *
 * Idea:
 * Treat a[i] as the last occurrence of its value in a subsequence.
 *
 * Contribution of a[i]:
 * - Same values to the right cannot be chosen.
 * - Every other element to the right can be chosen or skipped.
 *
 * Contribution = 2^(non-equal elements on the right)
 *
 * Time  : O(n log MOD)
 * Space : O(n)
 * ========================================================
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

long long modPow(long long a, long long b)
{
    long long res = 1;

    while (b)
    {
        if (b & 1)
            res = res * a % MOD;

        a = a * a % MOD;
        b >>= 1;
    }

    return res;
}

int main()
{
    int n;
    cin >> n;

    vector<int> a(n);
    unordered_map<int, int> freq;

    for (int &x : a)
    {
        cin >> x;
        freq[x]++;
    }

    long long ans = 0;

    for (int i = 0; i < n; i++)
    {
        freq[a[i]]--;

        int totalRight = n - i - 1;
        int equalRight = freq[a[i]];
        int otherRight = totalRight - equalRight;

        ans = (ans + modPow(2, otherRight)) % MOD;
    }

    cout << ans << '\n';
}