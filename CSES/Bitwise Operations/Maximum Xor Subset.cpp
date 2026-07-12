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

int maxXorSubsequences(vector<int>& nums) {
        vector<int> basis(32, 0);

        // Step 1: Build the basis
        for (int x : nums) {
            // Iterate from the most significant bit (31) down to 0
            for (int i = 31; i >= 0; --i) {
                // Check if the i-th bit of x is set
                if ((x >> i) & 1) {
                    // If the basis for this bit is not yet set, claim it
                    if (!basis[i]) {
                        basis[i] = x;
                        break; // Stop, as x is now fully incorporated into the
                               // basis
                    }
                    // If it is set, cancel out the i-th bit and continue
                    // downwards
                    x ^= basis[i];
                }
            }
        }

        // Step 2: Greedily construct the maximum XOR value
        int maxXor = 0;
        for (int i = 31; i >= 0; --i) {
            // If XORing with the current basis vector increases our overall
            // sum, do it. Because basis[i] has its MSB at position i, it will
            // increase the result ONLY if the i-th bit of maxXor is currently
            // 0.
            if ((maxXor ^ basis[i]) > maxXor) {
                maxXor ^= basis[i];
            }
        }

        return maxXor;
    }

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    int ans = maxXorSubsequences(a);
    cout << ans << endl;
}
