/**
 * C++ STL Bitset Cheat Sheet for Online Assessments & Interviews
 *
 * Description:
 *  A fixed-size sequence of N bits. Space-optimized (each bit occupies 1 bit,
 *  yielding an 8x memory reduction compared to char/bool arrays).
 *  Supports bitwise operations at hardware speed (64 bits processed per CPU instruction).
 *
 * Time & Space Complexity:
 * ┌──────────────────┬──────────────────┬──────────────────────────────────────────┐
 * │ Operation        │ Time Complexity  │ Notes                                    │
 * ├──────────────────┼──────────────────┼──────────────────────────────────────────┤
 * │ set() / reset()  │ O(1)             │ Set/reset specific bit                   │
 * │ flip() / test()  │ O(1)             │ Flip/test specific bit value             │
 * │ operator&, |, ^  │ O(N / 64)        │ Bitwise operations at hardware speed     │
 * │ operator<<, >>   │ O(N / 64)        │ Bitwise shifts                           │
 * │ count()          │ O(N / 64)        │ Popcount (number of set bits)            │
 * │ to_string()      │ O(N)             │ Conversion to string                     │
 * │ to_ulong()       │ O(N)             │ Conversion to unsigned long              │
 * └──────────────────┴──────────────────┴──────────────────────────────────────────┘
 * Space Complexity: N bits (N / 8 bytes)
 *
 * Typical Interview Usage:
 *  - Bitset-Optimized Knapsack / Subset Sum (CRITICAL OA TECHNIQUE!)
 *    Standard subset sum DP: dp[i][j] = dp[i-1][j] | dp[i-1][j-wt] runs in O(N * W).
 *    Bitset-optimized DP: `dp |= (dp << wt);` runs in O(N * W / 64)! This shifts all reachable sums by `wt`
 *    and bitwise-ORs them. This 64x speedup runs massive constraints (N=1000, W=100,000) in <10ms!
 *  - Set Intersections / Querying overlaps (mutual friends, common flags)
 *  - Transitive closure / reachability in DAGs
 *
 * Interview Tricks & Pitfalls:
 *  - Compile-time Size Constraint: The size N of `std::bitset<N>` MUST be a compile-time constant.
 *    You cannot use a dynamic variable. Define N as the maximum constraint upper bound.
 *  - Left Bit is LSB or MSB?: The rightmost character in string representation corresponds to index 0 (LSB).
 *    For example: `bitset<8> bs("00000010");` -> `bs.test(1)` is true, `bs.test(0)` is false.
 */

#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int MAX_W = 100005;

/*==========================================================
=            1. CONSTRUCTION & BASIC BITWISE OPERATIONS
==========================================================*/

void basicOperations() {
    cout << "--- 1. BASIC OPERATIONS ---\n";
    
    // Initialized to all zeros
    bitset<8> bs;

    // Set individual bits
    bs.set(1);       // 00000010 (sets index 1 to 1)
    bs.set(4, true); // 00010010 (sets index 4 to 1)
    bs.flip(1);      // 00010000 (flips index 1 back to 0)

    cout << "Bit at index 4: " << bs.test(4) << "\n"; // 1
    cout << "Bit at index 1: " << bs.test(1) << "\n"; // 0
    cout << "Set bit count: " << bs.count() << "\n";   // 1

    // Checks
    cout << "Any bits set? " << (bs.any() ? "Yes" : "No") << "\n";
    cout << "All bits set? " << (bs.all() ? "Yes" : "No") << "\n";
    cout << "No bits set?  " << (bs.none() ? "Yes" : "No") << "\n";

    // Reinitialization options
    bitset<8> bs_str("10101010"); // String representation (Index 0 is rightmost '0')
    cout << "Decimal of 10101010: " << bs_str.to_ulong() << "\n"; // 170

    bs.reset(); // Resets all to 0
    cout << "Is empty now? " << bs.none() << "\n\n";
}

/*==========================================================
=            2. BITWISE LOGIC & GRAPH INTERSECTIONS
==========================================================*/

void bitwiseIntersections() {
    cout << "--- 2. BITWISE LOGIC & INTERSECTIONS ---\n";
    // Check if user A and user B share any mutual friends in O(N/64)
    bitset<100> friendsA;
    bitset<100> friendsB;
    
    friendsA.set(5);
    friendsA.set(12);
    friendsA.set(45);

    friendsB.set(12);
    friendsB.set(67);

    // Bitwise Intersection
    bitset<100> mutual = friendsA & friendsB;

    cout << "Do they share mutual friends? " << (mutual.any() ? "Yes" : "No") << "\n";
    if (mutual.any()) {
        cout << "  Mutual friend ID: ";
        for (int i = 0; i < 100; ++i) {
            if (mutual.test(i)) cout << i << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

/*==========================================================
=            3. BITSET-OPTIMIZED SUBSET SUM (DP)
==========================================================*/

// Solves Subset Sum in O(N * Target / 64) time.
// Returns true if a subset of 'weights' sums up exactly to 'target'.
bool canMakeSum(const vector<int>& weights, int target) {
    if (target >= MAX_W) return false;

    // dp[x] = 1 if sum 'x' is reachable, 0 otherwise
    bitset<MAX_W> dp;
    
    // Base case: Sum 0 is reachable (empty subset)
    dp[0] = 1;

    for (int wt : weights) {
        // Shift all active sums by wt and combine.
        // e.g., if dp had sums 0 and 2 reachable: dp = 0101
        // Shift by 3: dp << 3 = 0101000 (sums 3 and 5 are now reachable)
        // Combine: dp |= (dp << 3) -> 0101101 (sums 0, 2, 3, 5 reachable)
        dp |= (dp << wt);
    }

    return dp[target];
}

void subsetSumDemo() {
    cout << "--- 3. BITSET-OPTIMIZED SUBSET SUM ---\n";
    vector<int> weights = {2, 4, 8};
    cout << "Can make sum 6? " << (canMakeSum(weights, 6) ? "Yes" : "No") << "\n";
    cout << "Can make sum 5? " << (canMakeSum(weights, 5) ? "Yes" : "No") << "\n\n";
}

/*==========================================================
=            4. DAG REACHABILITY / TRANSITIVE CLOSURE
==========================================================*/

void transitiveClosureDemo() {
    cout << "--- 4. TRANSITIVE CLOSURE ON DAG DEMO ---\n";
    // Find all reachable nodes from each node in a DAG of size 4
    int n = 4;
    vector<vector<int>> adj(n);
    adj[0] = {1, 2};
    adj[1] = {3};
    adj[2] = {3};
    adj[3] = {};

    // For reachability, we can process nodes in reverse topological order
    // reach[u] = reach[u] | reach[v] for all neighbors v
    vector<bitset<4>> reach(n);
    for (int i = 0; i < n; ++i) {
        reach[i].set(i); // Node can reach itself
    }

    // Process nodes in reverse topo order: 3, 2, 1, 0
    vector<int> topo_order = {3, 2, 1, 0};
    for (int u : topo_order) {
        for (int v : adj[u]) {
            reach[u] |= reach[v];
        }
    }

    for (int i = 0; i < n; ++i) {
        cout << "Node " << i << " can reach nodes: ";
        for (int j = 0; j < n; ++j) {
            if (reach[i].test(j)) cout << j << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

/*==========================================================
=            COMMON OPERATIONS REFERENCE
==========================================================*/

/**
 * Common Member Functions:
 * ┌──────────────────┬────────────────────────────┬─────────────────────────────┐
 * │ Function         │ Description                │ Complexity                  │
 * ├──────────────────┼────────────────────────────┼─────────────────────────────┤
 * │ bs.set(pos)      │ Set bit at position to 1   │ O(1)                        │
 * │ bs.reset(pos)    │ Reset bit at position to 0 │ O(1)                        │
 * │ bs.flip(pos)     │ Toggle bit value           │ O(1)                        │
 * │ bs.test(pos)     │ Returns bit value safely   │ O(1)                        │
 * │ bs.count()       │ Returns count of set bits  │ O(N / 64)                   │
 * │ bs.any()         │ Check if at least one set  │ O(N / 64)                   │
 * │ bs.none()        │ Check if no bits set       │ O(N / 64)                   │
 * │ bs.all()         │ Check if all bits set      │ O(N / 64)                   │
 * │ bs.to_ulong()    │ Convert to unsigned long   │ O(N)                        │
 * └──────────────────┴────────────────────────────┴─────────────────────────────┘
 */

/*==========================================================
=            MOST IMPORTANT TEMPLATES FOR OAs
==========================================================*/

// 1. Bitset Knapsack/Subset Sum DP
// bitset<MAX_TARGET> dp;
// dp[0] = 1;
// for (int wt : weights) {
//     dp |= (dp << wt);
// }

// 2. DAG Reachability
// vector<bitset<MAX_NODES>> reach(n);
// for (int i = 0; i < n; ++i) reach[i].set(i);
// for (int u : reverse_topo) {
//     for (int v : adj[u]) reach[u] |= reach[v];
// }

/*==========================================================
=            REMEMBER THESE (INTERVIEW SYNTAX)
==========================================================*/

/*
// Initialization (compile-time constant)
const int N = 1000;
bitset<N> bs;
bitset<N> bs_str("0011");

// Operations
bs.set(idx);
bs.reset(idx);
bs.flip(idx);

// Checking
if (bs.test(idx)) { ... }
int active_count = bs.count();

// Logic
bitset<N> common = bs1 & bs2;
*/

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Uncomment to test:
    // basicOperations();
    // bitwiseIntersections();
    // subsetSumDemo();
    // transitiveClosureDemo();

    return 0;
}
