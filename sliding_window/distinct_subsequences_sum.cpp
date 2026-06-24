/**
 * Problem: Distinct Subsequences Sum
 * Link: N/A
 * Category: Sliding Window
 * 
 * Description:
 * Sum of distinct values in all subsequences.
 * 
 * Logic/Approach:
 * Counts the rightmost occurrence contribution of each element using power of 2.
 */

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

const int MOD = 1e9 + 7;

// Helper function to calculate (base^exp) % MOD in O(log exp) time
long long power(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

int main() {
    // Optimize standard I/O operations for performance
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n);
    unordered_map<int, int> freq;
    for (int& x : a) { cin >> x; freq[x]++; }

    long long sum = 0;
    // Iterate through the array and calculate the contribution of each element a[i].
    // We treat a[i] as the last occurrence of this value in any chosen subsequence.
    // For elements to the right:
    // - Elements equal to a[i] cannot be chosen (to avoid duplicates).
    // - Elements not equal to a[i] can either be chosen or not (2 options each).
    for (int i = 0; i < n; i++) {
        freq[a[i]]--; // Remove current element from the right-side frequency map
        int totalRight = n - i - 1;
        int equalRight = freq[a[i]];
        int notEqualRight = totalRight - equalRight;
        sum = (sum + power(2, notEqualRight)) % MOD;
    }
    cout << sum << "\n";
    return 0;
}

