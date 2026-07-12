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

    // Adjusting k to be 0-indexed for easier division/modulo math
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
