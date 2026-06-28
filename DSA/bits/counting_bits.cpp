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

    long long n;
    cin >> n;

    long long total_ones = 0;

    // Iterate through all possible bit positions (0 to 60)
    for (int i = 0; i < 60; i++) {
        // cycle_length = 2^(i+1)
        long long cycle_length = 1LL << (i + 1);
        
        // Number of full cycles in (n + 1) numbers
        long long full_cycles = (n + 1) / cycle_length;
        
        // Each full cycle has 2^i ones
        long long ones_per_cycle = 1LL << i;
        long long zeroes_per_cycle = 1LL << i; 
        
        total_ones += full_cycles * ones_per_cycle;
        
        // Handle the remainder
        long long remainder = (n + 1) % cycle_length;
        // in the last block 
        if (remainder > ones_per_cycle) {
            total_ones += (remainder - zeroes_per_cycle);
        }
    }

    cout << total_ones << "\n";

    return 0;
}