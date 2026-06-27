/**
 * Problem: Randomized Tester Utility
 * Link: N/A
 * Category: Utilities
 * 
 * Description:
 * Generates randomized trees and graph query inputs for stress testing.
 * 
 * Logic/Approach:
 * Random number generator printing adjacency lists.
 */

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, x;
    cin >> n >> x;
    
    vector<int> a(n);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    
    // Step 1: Sort the weights
    sort(a.begin(), a.end());
    
    int gondolas = 0;
    int left = 0;          // Pointer to the lightest person
    int right = n - 1;     // Pointer to the heaviest person

    // Step 2 & 3: Use two pointers to pair people up
    while (left <= right)
    {
        if (left == right)
        {
            // Only one person left, they take the last gondola
            gondolas++;
            break;
        }

        if (a[left] + a[right] <= x)
        {
            // They fit together. Move both pointers.
            left++;
            right--;
        }
        else
        {
            // The heaviest person must ride alone. Move only the right pointer.
            right--;
        }
        
        // Either they rode together or the heavy person rode alone.
        gondolas++;
    }

    cout << gondolas << '\n';
    return 0;
}