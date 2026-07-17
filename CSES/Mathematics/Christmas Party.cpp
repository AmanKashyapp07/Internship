// Link: https://cses.fi/problemset/task/1717

#include <iostream>
#include <vector>

using namespace std;

int main() {
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // Handle base cases directly
    if (n == 1) {
        cout << 0 << "\n";
        return 0;
    }
    if (n == 2) {
        cout << 1 << "\n";
        return 0;
    }

    long long MOD = 1e9 + 7;
    long long prev2 = 0; // Represents D_1
    long long prev1 = 1; // Represents D_2
    long long current = 0; // Represents D_i

    // Compute derangements iteratively from 3 up to n
    for (int i = 3; i <= n; i++) {
        current = (i - 1) * (prev1 + prev2) % MOD;
        
        // Slide the tracking variables forward
        prev2 = prev1;
        prev1 = current;
    }

    cout << current << "\n";
    return 0;
}
