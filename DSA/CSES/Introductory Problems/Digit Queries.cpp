// Link: https://cses.fi/problemset/task/2431

#include <iostream>
#include <string>

using namespace std;

void solve() {
    long long k;
    cin >> k;

    long long length = 1;     // Tracks the number of digits (1-digit, 2-digit, etc.)
    long long count = 9;      // How many numbers exist with the current 'length'
    long long start = 1;      // The first number of the current length group (1, 10, 100...)

    // Step 1: Identify the digit-length group that contains the k-th digit
    while (k > length * count) {
        k -= length * count;
        length++;
        count *= 10;
        start *= 10;
    }

    // graphusting k to be 0-indexed for easier division/modulo math
    k--; 

    // Step 2: Find the exact number where the k-th digit resides
    long long target_number = start + (k / length);

    // Step 3: Find the specific digit inside target_number
    int digit_index = k % length;
    
    // Convert to string to easily pick out the character by index
    string s = to_string(target_number);
    
    cout << s[digit_index] << "\n";
}

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int q;
    cin >> q;
    while (q--) {
        solve();
    }

    return 0;
}