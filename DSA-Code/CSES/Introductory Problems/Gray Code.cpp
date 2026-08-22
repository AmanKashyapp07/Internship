// Link: https://cses.fi/problemset/task/2205

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // Total number of elements in a Gray code sequence of length n is 2^n
    int total_strings = 1 << n; 

    for (int i = 0; i < total_strings; i++) {
        // Convert regular binary index i to its Gray code equivalent
        int gray_value = i ^ (i >> 1);
        
        // Print the binary representation of gray_value with leading zeros
        string bit_string = "";
        for (int j = n - 1; j >= 0; j--) {
            if ((gray_value >> j) & 1) {
                bit_string += '1';
            } else {
                bit_string += '0';
            }
        }
        cout << bit_string << "\n";
    }

    return 0;
}