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
