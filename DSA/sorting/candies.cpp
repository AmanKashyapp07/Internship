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
    vector<int> ratings(n);
    for (int i = 0; i < n; i++) cin >> ratings[i];
    vector<int> candies(n, 1); // Initialize all with 1 candy
    vector<int> left(n, 1), right(n, 1); // To store candies from left and right passes
    for(int i=0;i<n-1;i++)
    {
        if(ratings[i+1]>ratings[i])
            left[i+1]=left[i]+1;
    }
    for(int i=n-1;i>0;i--)
    {
        if(ratings[i-1]>ratings[i])
            right[i-1]=right[i]+1;
    }
    for(int i=0;i<n;i++)
    {
        candies[i]=max(left[i],right[i]); // as we need to satisfy both conditions, we take the maximum of both passes, we are doing in two pass because we cannot assign candies in one pass as we need to check both left and right conditions, so we do two passes and take the maximum of both passes.
    }
    long long totalCandies=0;
    for(int i=0;i<n;i++)
    {
        totalCandies+=candies[i];
    }
    cout << totalCandies << "\n";
    
    
    return 0;
}
