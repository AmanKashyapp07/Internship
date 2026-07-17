#include <iostream>

using namespace std;

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n;
    if (cin >> n) {
        for (long long k = 1; k <= n; ++k) {
            // Total ways to place 2 knights on a k * k board
            long long total_ways = (k * k * (k * k - 1)) / 2;
            
            // Ways two knights can attack each other
            long long attacking_ways = 4 * (k - 1) * (k - 2);
            
            // Safe ways
            cout << total_ways - attacking_ways << "\n";
        }
    }
    
    return 0;
}