/**
 * CSES Problem Set
 * 
 * Problem: Counting Bits
 * Link: https://cses.fi/problemset/task/2206
 * Category: Mathematics / Bit Manipulation
 * 
 * Description:
 * Count the total number of 1-bits in the binary representations of all integers from 1 to n.
 * 
 * Logic/Approach:
 * Iterate through bit positions. Count contribution of each bit based on full cycles of size 2^(i+1) and remainder.
 */

#include <iostream>

using namespace std;

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    int total_ones = 0;

    // Iterate through all possible bit positions (0 to 60)
    for (int i = 0; i < 60; i++) {
        // lengthCycle = 2^(i+1)
        int lengthCycle = 1LL << (i + 1);
        int totalCycles = (n + 1) / lengthCycle;
        int one = 1LL << i;
        int zeroes = 1LL << i; 
        total_ones += totalCycles * one;
        int remainder = (n + 1) % lengthCycle;
        // in the last block 
        if (remainder > zeroes) {
            total_ones += (remainder - zeroes);
        }
    }

    cout << total_ones << "\n";

    return 0;
}