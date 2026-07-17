// Link: https://cses.fi/problemset/task/3421

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Optimize standard I/O operations for speed
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // Sort the array to easily count frequencies of identical elements
    sort(arr.begin(), arr.end());

    long long ans = 1;
    long long MOD = 1e9 + 7;
    long long current_freq = 1;

    for (int i = 1; i < n; i++) {
        if (arr[i] == arr[i - 1]) {
            current_freq++;
        } else {
            // Multiply combinations for the previous distinct element
            ans = (ans * (current_freq + 1)) % MOD;
            // Reset frequency counter for the new element
            current_freq = 1; 
        }
    }
    
    // Don't forget to multiply the combinations for the very last distinct element
    if (n > 0) {
        ans = (ans * (current_freq + 1)) % MOD;
    }

    // Subtract 1 to exclude the empty subsequence. 
    // Adding MOD before applying % MOD prevents issues with negative numbers.
    ans = (ans - 1 + MOD) % MOD;

    cout << ans << "\n";

    return 0;
}