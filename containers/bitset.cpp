/**
 * C++ STL Bitset Cheat Sheet for Online Assessments
 *
 * Description:
 *  A fixed-size sequence of N bits. Space-optimized (each bit occupies 1 bit of memory,
 *  meaning 8x memory reduction compared to char/bool arrays).
 *  Supports bitwise operations at hardware speed (64 bits processed in a single CPU instruction).
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * TIME & SPACE COMPLEXITY
 * ────────────────────────────────────────────────────────────────────────────────
 * - set() / reset() / flip() / test() : O(1)
 * - operator&, operator|, operator^   : O(N / 64)
 * - operator<<, operator>>            : O(N / 64)
 * - count() (popcount)                 : O(N / 64)
 * - to_string()                       : O(N)
 * - Space                             : N bits (N / 8 bytes)
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * WHEN TO USE IN OAs
 * ────────────────────────────────────────────────────────────────────────────────
 * 1. Bitset-Optimized Knapsack / Subset Sum (CRITICAL OA TECHNIQUE!)
 *    - Standard subset sum DP table: dp[i][j] = dp[i-1][j] | dp[i-1][j - wt]. Time: O(N * W).
 *    - Bitset-optimized DP: bitset<MAX_W> dp; dp |= (dp << wt);
 *      This shifts all active sums by 'wt' and bitwise-ORs them in O(W / 64) time!
 *      New complexity: O(N * W / 64). Can speed up programs by a factor of 64, making
 *      previously impossible constraints (e.g. N=1000, W=100,000, operations = 10^8) run in <10ms!
 * 2. Set Intersections / Querying Overlaps
 *    - Check if two sets share any common elements: (bitsetA & bitsetB).any() in O(N / 64).
 * 3. Transitive Closure / Reachability on Graphs
 *    - E.g. Find all nodes reachable from each node in a DAG.
 *      reach[u] = reach[u] | reach[v] for all neighbors v (processed in topological order).
 *
 * ────────────────────────────────────────────────────────────────────────────────
 * OA TIPS & TRICKS
 * ────────────────────────────────────────────────────────────────────────────────
 * - Compile-time Size Constraint:
 *   The size N of a `std::bitset<N>` MUST be a compile-time constant. You cannot pass a dynamic
 *   variable as the template argument. In OAs, define N as the maximum upper bound of the constraints
 *   (e.g., `const int MAXN = 100005; bitset<MAXN> bs;`).
 * - Built-in functions:
 *   - `bs.count()`: returns number of set bits (popcount).
 *   - `bs.any()`: returns true if at least one bit is set.
 *   - `bs.none()`: returns true if no bits are set.
 *   - `bs.all()`: returns true if all bits are set.
 */

#include <iostream>
#include <bitset>
#include <vector>
#include <string>

using namespace std;

// Maximum weight limit for Knapsack / Subset Sum
const int MAX_W = 100005;

/**
 * Solves Subset Sum using bitset optimization in O(N * Target / 64) time.
 * Returns true if a subset of 'weights' sums up exactly to 'target'.
 */
bool can_make_sum(const vector<int>& weights, int target) {
    if (target >= MAX_W) return false;

    // dp[x] = 1 if sum 'x' is reachable, 0 otherwise
    bitset<MAX_W> dp;
    
    // Base case: Sum 0 is always reachable (by choosing empty subset)
    dp[0] = 1; 

    for (int wt : weights) {
        // Shift all active sums by wt and combine.
        // E.g., if dp had sum 0 and 2 reachable: dp = 0101
        // Shift by 3: dp << 3 = 0101000
        // New dp = 0101 | 0101000 = 0101101 (sums 0, 2, 3, 5 are now reachable)
        dp |= (dp << wt);
    }

    return dp[target];
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. BASIC BITSET OPERATIONS
    // bitset of size 8 initialized to 00000000
    bitset<8> bs; 
    
    bs.set(1);       // 00000010 (sets index 1 to 1)
    bs.set(4, true); // 00010010 (sets index 4 to 1)
    bs.flip(1);      // 00010000 (flips index 1)
    
    cout << "Bit at index 4: " << bs.test(4) << "\n"; // Outputs 1 (true)
    cout << "Bit at index 1: " << bs.test(1) << "\n"; // Outputs 0 (false)
    cout << "Set bit count: " << bs.count() << "\n";   // Outputs 1

    bs.reset();      // Reset all to 0
    cout << "Is empty now: " << bs.none() << "\n";    // Outputs 1 (true)

    // 2. INITIALIZATION OPTIONS
    bitset<8> bs_from_string("10101010"); // LSB is at index 0 (rightmost is index 0)
    cout << "Decimal representation: " << bs_from_string.to_ulong() << "\n"; // Outputs 170

    // 3. BITSET-OPTIMIZED SUBSET SUM DEMO
    vector<int> weights = {2, 4, 8};
    cout << "Can make 6: " << can_make_sum(weights, 6) << "\n";   // Outputs 1 (true)
    cout << "Can make 5: " << can_make_sum(weights, 5) << "\n";   // Outputs 0 (false)

    // 4. GRAPH OVERLAP / INTERSECTION DEMO
    // Check if user A and user B share any mutual friends
    bitset<100> friendsA;
    bitset<100> friendsB;
    
    friendsA.set(5);
    friendsA.set(12);
    friendsA.set(45);

    friendsB.set(12);
    friendsB.set(67);
    
    // Intersection
    bitset<100> mutual = friendsA & friendsB;
    cout << "Do they share mutual friends: " << mutual.any() << "\n"; // Outputs 1 (true)
    cout << "Mutual friend ID: 12 is " << mutual.test(12) << "\n";    // Outputs 1 (true)

    return 0;
}
