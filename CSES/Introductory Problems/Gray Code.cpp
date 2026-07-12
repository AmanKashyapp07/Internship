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
