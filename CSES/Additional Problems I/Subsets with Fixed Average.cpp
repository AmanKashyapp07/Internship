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

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

const int MOD = 1e9 + 7;
const int OFFSET = 250000;
const int MAX_SUM = 500000;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, a;
    if (!(cin >> n >> a)) return 0;

    vector<int> x(n);
    for (int i = 0; i < n; i++) {
        int val;
        cin >> val;
        x[i] = val - a; // The balancing trick
    }

    // We only need two rows to track our states
    vector<int> prev_row(MAX_SUM + 1, 0);
    vector<int> curr_row(MAX_SUM + 1, 0);
    
    // Base case: The empty set sum is 0 (which sits at OFFSET)
    prev_row[OFFSET] = 1;

    // Go through each number in our array one by one
    for (int i = 0; i < n; i++) {
        int y = x[i];

        // Check every possible sum from 0 to 500,000
        for (int sum = 0; sum <= MAX_SUM; sum++) {
            
            // OPTION 1: Leave it
            // The number of ways to make 'sum' is at least the number of ways 
            // we could make it before looking at this number.
            curr_row[sum] = prev_row[sum];
            
            // OPTION 2: Take it
            // If we subtract our current number 'y' from 'sum', did we have a valid 
            // way to make that previous total?
            int prev_sum = sum - y;
            if (prev_sum >= 0 && prev_sum <= MAX_SUM) {
                curr_row[sum] = (curr_row[sum] + prev_row[prev_sum]) % MOD;
            }
        }
        
        // Our current row is finished. It becomes the previous row for the next number!
        prev_row = curr_row;
    }

    // Our answer is the number of ways to make 0 (OFFSET), minus 1 for the empty set
    long long ans = (prev_row[OFFSET] - 1 + MOD) % MOD;
    
    cout << ans << "\n";

    return 0;
}
