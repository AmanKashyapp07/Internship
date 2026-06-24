/**
 * CSES Problem Set
 *
 * Problem: CSES Solution Template
 * Link: https://cses.fi/
 * Category: Template
 *
 * Description:
 * A template file containing boilerplate code, fast I/O setup, and common macros/imports for solving CSES problems.
 *
 * Logic/Approach:
 * Provides basic imports, standard namespace, and empty main function with fast I/O configuration.
 */

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second




int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    vector<int> nums(n,0);
    for (int i = 0; i < n; i++) cin >> nums[i];
    int i=1; // pointer for the next unique element , 0 to i-1 are unique elements
    // j is pointer for traversing the array
    for(int j=1;j<n;j++)
    {
        if(nums[j]!=nums[i-1]) // if current element is not equal to the last unique element
        {
            nums[i]=nums[j]; // place the current element at the next unique position
            i++; // increment the pointer for the next unique element
        }
    }

    for (int k = 0; k < i; k++) {
        cout << nums[k] << " ";
    }
    return 0;
}
